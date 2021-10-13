
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

#define BUF_SIZE 1000000
int doc_no = 1;
int op_file = 1;



using namespace std;
//bool cmp(pair<pair<string, int>, int>& a, pair<pair<string, int>, int>& b)
//{
//
//    return a.first.first > b.first.first;
//
//}
int main(int argc, const char * argv[]) {
    // insert code here...
    map<pair<string, int>, int> mp;
    map<int, string> page_table;
    string op = "misc_" + to_string(op_file) + ".txt";
    std::regex word_regex("(\\d+)");
    std::regex pattern("[a-zA-Z]+");
    cout<<"op: "<< op<<endl;
    fstream fp;
    fp.open ("sample.txt");
    fstream op_fp;
    fstream pt_fp;
    op_fp.open(op, fstream::in | fstream::out | fstream::trunc);
    pt_fp.open("page_table.txt", ios_base::app);
    if(!op_fp) cout<<"op_fp is NULL"<<endl;
    string s = "Kewin";
    int count = 0;
    bool valid_string = false;
    string separator(" \t\r\n,.!?;()—\"\'@*=:%><#&+-\\\/_$^{}[]|");
    regex r(
    "https?:\\/\\/(www\\.)?"\
    "[-a-zA-Z0-9@:%._\\+~#=]{1,256}"\
    "\\.[a-zA-Z0-9()]{1,6}\\b([-a-z"\
    "A-Z0-9()@:%_\\+.~#?&//=]*)");
    clock_t start = clock();
    struct non_alpha {
        bool operator()(char c) {
            return !std::isalpha(c);
        }
    };
    while(getline(fp, s)){
        //cout<<"String: "<<s<<endl;
        if(mp.size()< BUF_SIZE){
        if(s.find("<TEXT>") != string::npos) {
            //cout<<"TEXT found line: "<<count<<endl;
            getline(fp, s);
            if(s.find("http") != string::npos)
            {
                page_table[doc_no] = s;
                pt_fp<< doc_no << ": "<< s <<endl;
                getline(fp, s);
            }
            
            while(s != "</TEXT>")
            {
//                stringstream ss(s);
//                string word;

                size_t e,start=0;            // s = offset of next word, e = end of next word
                do {

                    
                    start = s.find_first_not_of(separator,start);  // skip leading separators
                    if (start==string::npos)                  // stop if no word left
                        break;
                    e=s.find_first_of(separator, start);   // find next separator
                    string word(s.substr(start,e-start));
                    auto words_begin =
                        std::sregex_iterator(word.begin(), word.end(), word_regex);
                    auto words_end = std::sregex_iterator();
                    start=e+1;
                    if(std::distance(words_begin, words_end)>0) {
                        
                       continue;}
                        // construct the word
                    valid_string = std::find_if(word.begin(),word.end(), non_alpha()) != word.end();
                    
                    if(valid_string){
                    //cout<<" word:" << word << "word size: "<<word.length()<<endl;
                        continue;
                    }
                    if((word.length() >3)) {
                        mp[make_pair(word, doc_no)]++;
                        
                    }
                    
                                                   // position after the separator
                } while (e!=string::npos);
//                while(ss >> word){
//                    mp[word]++;
//                    //op_fp<< word << " "<< mp[word] << endl;
//                }
                
                
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
            doc_no++;
            //cout<<"DOCNO found line: "<<count<<"Doc_NO: "<<doc_no<<endl;
            
        }
        count++;
        }
        else {
            //sort(mp.begin(),mp.end(), cmp);
            for(auto it = mp.begin(); it != mp.end(); ++it)
            {
                op_fp << it->first.first << "       "<< it->first.second << "," << it->second << endl;
            }
            mp.clear();
            op_fp.close();
            op_file++;
            op = "misc_"+ to_string(op_file) + ".txt";
            //op_fp(op);
            op_fp.open(op, fstream::in | fstream::out | fstream::trunc);
            cout<<"op after size overflow: "<<op<<endl;
            
        }
        
    }
    fp.close();
    
//    for(auto it = page_table.begin(); it != page_table.end(); ++it)
//    {
//        pt_fp << it->first << "---" << it->second << endl;
//    }
    cout.precision(10);
        cout << fixed <<  float(clock() - start)/CLOCKS_PER_SEC  << endl;
    pt_fp.close();
    return 0;
}


