/*
 
 generate_postings.cpp
 
 This file generates the intermediate postings of the form (WORD TERM, DOCID, FREQUENCY) into file misc_*.
 
 Input: fulldocs-new.trec 22GB
 Output: 976 Misc files each ~20MB
 

 */
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <algorithm>
#include <cctype>
#include <functional>
#include <time.h>

// Dynamic size to alter the size of current memory containing the postings.
#define BUF_SIZE 1000000
int doc_no = 1;
int op_file = 1;



using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    map<pair<string, int>, int> mp; // Container to store the occurences of each word term present in the document.
    map<int, string> page_table; // Container to hash the document id with URL of respective document.
    string op = "misc_" + to_string(op_file) + ".txt"; // If buffer size is reached, map container flushed all of its elements into misc file.
    std::regex word_regex("(\\d+)"); // Regex to remove numbers.
    std::regex pattern("[a-zA-Z]+"); // Regex to ignore non-english characters.
    cout<<"op: "<< op<<endl;
    fstream fp;
    fp.open ("fulldocs-new.trec");
    fstream op_fp;
    fstream pt_fp;
    op_fp.open(op, fstream::in | fstream::out | fstream::trunc);
    pt_fp.open("page_table.txt", ios_base::app); // Page table file where DocID to URL mapping is created.
    if(!op_fp) cout<<"op_fp is NULL"<<endl;
    string s = "Kewin";
    int count = 0;
    bool valid_string = false;
    string separator(" \t\r\n,.!?;()—\"\'@*=:%><#&+-\\\/_$^{}[]|");
    regex r(
    "https?:\\/\\/(www\\.)?"\
    "[-a-zA-Z0-9@:%._\\+~#=]{1,256}"\
    "\\.[a-zA-Z0-9()]{1,6}\\b([-a-z"\
    "A-Z0-9()@:%_\\+.~#?&//=]*)"); // Regex to filter URL of each document.
    struct non_alpha {
        bool operator()(char c) {
            return !std::isalpha(c);
        }
    };
    clock_t start = clock();
    while(getline(fp, s)){
        //cout<<"String: "<<s<<endl;
        if(mp.size()< BUF_SIZE){
        if(s.find("<TEXT>") != string::npos) { // Extract the TEXT body of document
            getline(fp, s);
            if(s.find("http") != string::npos)
            {
                page_table[doc_no] = s;
                pt_fp<< doc_no << ": "<< s <<endl;
                getline(fp, s);
            }
            
            while(s != "</TEXT>") // Loop until the end of each text body of document.
            {

                size_t e,start=0;
                do {

                    start = s.find_first_not_of(separator,start);
                    if (start==string::npos)
                        break;
                    e=s.find_first_of(separator, start);
                    string word(s.substr(start,e-start));
                    auto words_begin =
                        std::sregex_iterator(word.begin(), word.end(), word_regex);
                    auto words_end = std::sregex_iterator();
                    start=e+1;
                    if(std::distance(words_begin, words_end)>0) {
                       continue;
                    }
                        // construct the word
                    valid_string = std::find_if(word.begin(),word.end(), non_alpha()) != word.end();
                    
                    if(valid_string){
                        continue;
                    }
                    if((word.length() >3)) {
                        mp[make_pair(word, doc_no)]++; // Count the occurences of word term in the document.
                        
                    }
                    
                } while (e!=string::npos);

                getline(fp, s);
            }
            
        }
        else if(s.find("<DOC>") != string::npos) {
            //cout<<"DOC found line: "<<count<<endl;
            
        }
        else if(s.find("</DOC>") != string::npos) {
            cout<<"Document last: "<<doc_no<<endl;

            
        }
        else if(s.find("<DOCNO>") != string::npos) {
            doc_no++; // Increment the counter for the DOCID numbered consecutively.
            //cout<<"DOCNO found line: "<<count<<"Doc_NO: "<<doc_no<<endl;
        }
        count++;
        }
        else {
            for(auto it = mp.begin(); it != mp.end(); ++it)
            {
                /*
                 Output the intermediate posting (Word Term, DOCID, FREQUENCY) to misc file when buffer size is reached.
                 
                 */
                op_fp << it->first.first << "       "<< it->first.second << "," << it->second << endl;
            }
            mp.clear(); // Reset the container to extract other word terms from the document.
            op_fp.close();
            op_file++;
            op = "misc_"+ to_string(op_file) + ".txt";
            op_fp.open(op, fstream::in | fstream::out | fstream::trunc);
            cout<<"op after size overflow: "<<op<<endl;
            
        }
        
    }
    
    fp.close();
    
    cout.precision(10);
        cout<<"Time taken to generate intermediate postings: " << fixed <<  float(clock() - start)/CLOCKS_PER_SEC<<" seconds"  << endl;
    
    pt_fp.close();
    return 0;
}


