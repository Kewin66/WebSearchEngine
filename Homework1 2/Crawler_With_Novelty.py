import logging
import urllib


import os
from googlesearch import search

import requests
from heapdict import heapdict
import validators
import http.client
import bs4 as bs
from urllib.parse import urlparse
from urllib.parse import urljoin
from bs4 import SoupStrainer
import urllib.robotparser
import socket
import datetime
from prettytable import PrettyTable
import ssl
ssl._create_default_https_context = ssl._create_unverified_context

seen = set()
headers = {
    'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) ..',
    'From': 'kewinsrinath03@gmail.com'  # This is another valid field
}
# Table entries of crawled pages
x = PrettyTable()
x.field_names = ["CRAWLED WEB PAGE", "PRIORITY SCORE","STATUS", "TIME"]
output = ""
# Novelty is with domain as key
novelty = {}

# Importance is for complete URL
importance = {}

# URL path is key and linear combination of Novelty and Importance is the Score
priority_webpage = heapdict()

# All URL paths are grouped according to their domain name
mapping = {"": [1, 2, 4]}
pages_crawled = 0
num_err = 0
forbidden_err = 0

# Set containing robots.txt for the domain
domain_robot = dict()

# Get the number of pages crawled on particular domain
crawled_score = dict()
f = open('Kewin1_BrooklynUnion_with_Novelty.txt', 'w')
f.write("(Brooklyn Union- with Novelty)TIME, WEB URL CRAWLED, PRIORITY SCORE,  STATUS , SIZE")
f.close()

logging.basicConfig(filename="Kewin1_BrooklynUnion_with_Novelty.txt",
                            filemode='a',
                            format='%(asctime)s,%(msecs)d %(message)s',
                            datefmt='%H:%M:%S',
                            level=logging.DEBUG)
def compute_priority(URL, CurrentDomainName):
    ps = novelty[CurrentDomainName] + importance[URL]
    priority_webpage[URL] = ps * -1


def init_seed_page():
    # query = input("Enter the Query: ")
    query = input("Enter Query: ")
    count = 10
    for j in search(query):
        print(j)
        domain_name = urlparse(j).netloc
        novelty[domain_name] = 1
        crawled_score[domain_name] = 0
        importance[j] = 1
        mapping[domain_name] = [j]
        compute_priority(j, domain_name)
    # print("Get all novelty of seed pages"
    # )
    print("*************DOWNLOADED SEED PAGES******************")
    for x in novelty:
        print(x + " :" + str(novelty[x]))
    # print("Getting the importance of seed pages")
    for y in importance:
         print(y + " :" + str(importance[y]) + " Priority: " + str(priority_webpage[y]))

def UpdateNovelty(front_url, front_url_domain):
    print("Updating Novelty")
    if front_url_domain in crawled_score:
        k = crawled_score[front_url_domain]
    else:
        k = 0
    calc_novelty = 1/(1+k)
    if front_url_domain in mapping:
        novelty[front_url_domain] = calc_novelty
        return
    to_update_URL = mapping[front_url_domain]
    for link in to_update_URL:
        print("Novelty update for link---" + str(link))
        d = urlparse(link).netloc
        novelty[d] = novelty
        priority_webpage[link] = importance[link]
    # print("Novelty update: " + to_update_URL)
    # print("Displaying novelty after updating...")
    # for x in novelty:
    # print(x + " :" + str(novelty[x]))


def true_frontier(front_url, front_url_domain):
    global output
    UpdateNovelty(front_url, front_url_domain)
    current_front_ps = priority_webpage.peekitem()[1] * -1
    # current_front_ps = novelty[current_front] + importance[current_front]
    front_url_ps = novelty[front_url_domain] + importance[front_url]
    if front_url_ps >= current_front_ps:
        output+=str(front_url)
        output+=" ,"
        output+=str(front_url_ps)
        output+=" ,"
        return True
    else:
        priority_webpage[front_url] = front_url_ps * -1
    return False


def robot_parser(link):
    socket.setdefaulttimeout(1)
    domain = urlparse(link).netloc
    # print(domain)
    rp = urllib.robotparser.RobotFileParser()
    robot_url = "https://" + domain
    rp.set_url(robot_url)
    try:
        rp.read()
    except:
        return False
    rp.crawl_delay("*")
    result = rp.can_fetch("*", link)

    rp.modified()
    rp.mtime()
    return result


def update_importance(link):
    if link in importance:
        value = importance[link]
        importance[link] = value + 1
    else:
        importance[link] = 1


def compute_priority_new_page(link, domain):
    if domain not in novelty:
        novelty[domain] = 1
    priority_webpage[link] = novelty[domain] + importance[link]


def process_url(ip):
    # Download URL using request and update importance score of other webpages in the queue.

    global output
    socket.setdefaulttimeout(2)
    url = ip
    path = urlparse(url).path
    domain = urlparse(url).netloc
    try:
        conn = http.client.HTTPSConnection(domain)
        conn.request("GET", path, headers=headers)
        res = conn.getresponse()
    except:
        print("Can't retrieve page" + str(path))
        output+="(ERROR)"
        output+=" , "
        output += str(datetime.datetime.now().time())
        output = ""
        return

    print(res.status, res.reason)

    data = res.read()

    crawled_links = list()
    if res.status == 200:
        output+="200"
        output+=" , "
        for ll in bs.BeautifulSoup(data, 'html.parser',
                                     parseOnlyThese=SoupStrainer('a')):
            if ll.has_attr('href'):
                temp_link = urljoin(ip, ll['href'])
                if validators.url(temp_link):
                    if temp_link in seen:
                        continue
                    seen.add(temp_link)
                    crawled_links.append(temp_link)

        for link in crawled_links:
            can_crawl = robot_parser(link)
            if not can_crawl:
                crawled_links.remove(link)
            else:
                update_importance(link)
                domain = urlparse(link).netloc

                if domain in mapping:
                    # print(mapping)
                    mapping[domain].append(link)
                    priority_webpage[link] = novelty[domain] + importance[link]
                else:
                    mapping[domain] = [link]
                    compute_priority_new_page(link, domain)
    else:
        output+=str(res.status)
        print("Status Code: " + str(res.status))
        output+=" , "
        output+=str(len(data))
        output+=" , "

    output += str(len(data))
    output += " , "
    output += str(datetime.datetime.now().time())
    print(output)
    logging.info(output)
    output = ""

def begin_crawl():
    count = 0
    global pages_crawled
    while len(priority_webpage):
        ip = priority_webpage.popitem()[0]
        count += 1
        print("Link popped: " + ip)
        print("################################")
        print("Web page Crawled: " + str(pages_crawled) + "Time: " + str(datetime.datetime.now().time()))
        print("################################")
        ip_domain = urlparse(ip).netloc
        if true_frontier(ip, ip_domain):
            # print("Popped URL is the real front with highest priority: " + str(ip))
            process_url(ip)
            pages_crawled += 1
            temp = crawled_score[ip_domain]
            crawled_score[ip_domain] = temp + 1
            # print("Popped URL is True frontier,  crawled score: " + str(crawled_score))
            # print("Crawled page: " + str(pages_crawled))
            # print("Downloading URL: " + str(ip))
        else:
            print("********Novelty made popped URL to enqueue into Pool********")
            print(ip)
            # seen.remove(ip)
            continue



if __name__ == '__main__':
    init_seed_page()
    print("Beginning to crawl...")
    begin_crawl()
    print("Webpages without crawling: " + str(len(priority_webpage)))
    print("Crawling complete")

