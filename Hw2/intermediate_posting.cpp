
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#define BUF_SIZE 1000
int doc_no = 0;
int op_file = 1;



using namespace std;
int main(int argc, const char * argv[]) {
    // insert code here...
    map<string, int> mp;
    string op = to_string(op_file) + ".txt";
    cout<<"op: "<< op<<endl;
    fstream fp;
    fp.open ("sample.txt");
    fstream op_fp;
    op_fp.open(op, fstream::in | fstream::out | fstream::trunc);
    if(!op_fp) cout<<"op_fp is NULL"<<endl;
    string s = "Kewin";
    int count = 0;
    string separator(" \t\r\n,.!?;:");
    while(getline(fp, s)){
        if(mp.size()< BUF_SIZE){
        if(s.find("<TEXT>") != string::npos) {
            cout<<"TEXT found line: "<<count<<endl;
            getline(fp, s);
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
                    string word(s.substr(start,e-start));      // construct the word
                    mp[word]++;
                    start=e+1;                                // position after the separator
                } while (e!=string::npos);
//                while(ss >> word){
//                    mp[word]++;
//                    //op_fp<< word << " "<< mp[word] << endl;
//                }
                
                
                getline(fp, s);
            }
            
        }
        else if(s.find("<DOC>") != string::npos) {
            cout<<"DOC found line: "<<count<<endl;
            
        }
        else if(s.find("<DOCNO>") != string::npos) {
            doc_no++;
            cout<<"DOCNO found line: "<<count<<"Doc_NO: "<<doc_no<<endl;
            
        }
        count++;
        }
        else {
            for(auto it = mp.begin(); it != mp.end(); ++it)
            {
                op_fp << it->first << "," << it->second << endl;
            }
            mp.clear();
            op_fp.close();
            op_file++;
            op = to_string(op_file) + ".txt";
            //op_fp(op);
            op_fp.open(op, fstream::in | fstream::out | fstream::trunc);
            cout<<"op after size overflow: "<<op<<endl;
            
        }
        
    }
    fp.close();
    return 0;
}

