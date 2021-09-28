import queue
import urllib

import time
import threading

import heapdict as heapdict
import os
from googlesearch import search
import validators
import http.client
import bs4 as bs
from urllib.parse import urlparse
from urllib.parse import urljoin
from bs4 import SoupStrainer
import urllib.robotparser
import socket
import logging

import ssl
ssl._create_default_https_context = ssl._create_unverified_context

f = open("Kewin1_ParisTexas.txt", "w")
f.write("TIME, WEB URL CRAWLED,  STATUS , SIZE")
f.close()
headers = {
    'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) ..',
    'From': 'kewinsrinath03@gmail.com'  # This is another valid field
}
# Table entries of crawled pages
crawl_queue = queue.Queue()
queue_lock = threading.Lock()

seen = set()

logging.basicConfig(filename="Kewin1_ParisTexas.txt",
                            filemode='a',
                            format='%(asctime)s,%(msecs)d %(message)s',
                            datefmt='%H:%M:%S',
                            level=logging.DEBUG)

# logging.info("TIME , WEB URL CRAWLED,  STATUS , SIZE")
thread_pool = []
error_links = []

# All URL paths are grouped according to their domain name
pages_crawled = 0
num_err = 0
forbidden_err = 0

output = ""


def init_seed_page():
    # query = input("Enter the Query: ")
    query = input("Enter query: ")
    count = 1
    for j in search(query, num=10, stop=10, pause=2):
        crawl_queue.put(j)


def robot_parser(link):
    socket.setdefaulttimeout(1)
    domain = urlparse(link).netloc
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



class WebCrawler(threading.Thread):
    def __init__(self,crawl_queue,seen, queue_lock):
        threading.Thread.__init__(self)
        self.links_to_crawl = crawl_queue
        self.have_visited = seen
        self.url_lock = queue_lock

    def run(self):
        global output

        while True:
            queue_lock.acquire()
            link = crawl_queue.get()
            queue_lock.release()
            if link in seen:
                break
            if link is None:
                break
            url = link
            ip = link
            path = urlparse(url).path
            domain = urlparse(url).netloc
            socket.setdefaulttimeout(1)
            try:

                conn = http.client.HTTPSConnection(domain)
                conn.request("GET", path, headers=headers)
                res = conn.getresponse()

                data = res.read()

                crawled_links = list()
                if res.status == 200:
                    output += link
                    output += " , "
                    output += "     200     "
                    output += " , "
                    output += str(len(data))
                    print(output)
                    logging.info(output)
                    output = ""
                    for link in bs.BeautifulSoup(data, 'html.parser',
                                                 parseOnlyThese=SoupStrainer('a')):
                        if link.has_attr('href'):
                            # print(link['href'])
                            # print("After URLJOIN")
                            temp_link = urljoin(ip, link['href'])
                            if validators.url(temp_link):
                                crawled_links.append(temp_link)

                    for link in crawled_links:
                        # print("Link got from Pq-WP:" + link)
                        can_crawl = robot_parser(link)
                        if not can_crawl:
                            # print("LINK NOT ALLOWED TO CRAWL BY ROBOTS.TXT..." + link)
                            crawled_links.remove(link)
                        else:
                            crawl_queue.put(link)
                else:
                    output += link
                    output += " , "
                    output += str(res.status)
                    output += " , "
                    print(output)
                    logging.info(output)
                    output = ""

                seen.add(link)

            except:
                # print("ERROR")
                output += "ERROR"
                logging.info(output)
                output = ""
                error_links.append(link)

            finally:
                crawl_queue.task_done()




if __name__ == '__main__':
    init_seed_page()
    ip_a = input("Enter number of threads: ")
    num_threads = int(ip_a)

    for i in range(num_threads):
        spider = WebCrawler(crawl_queue,seen,queue_lock)
        spider.start()
        thread_pool.append(spider)

    for spider in thread_pool:
        spider.join()



