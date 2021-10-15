/*
 meta.cpp
 
 This file constructs the inverted list for each word term and stores the location of each list in the lexicon.
 Also, it compresses the inverted list using varbyte compression method.
 
 Input:  intermediate_postings.txt
 Output: lexicon.txt, inverted_list.dat
 
 
 
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <bitset>
#include <ctime>
#include <map>
using namespace std;

// container to store the encoded stream of single DOCID used to write into invertedlist file
vector<char> result;

// Dynamic memory size that holds stream of docID for each word term.
#define BUFSIZE 10000

// writeC outputs 1 byte of each encoded byte of single DocID for the given word term.
int writeC(vector<uint8_t> num, fstream& ofile){
    string s;
    int num_bytes = 0;
    
    for(vector<uint8_t>::iterator it = num.begin(); it != num.end(); it++){
        //result.push_back(*it);
        ofile.write(reinterpret_cast<const char*>(&(*it)), 1);
        num_bytes++;
    }
    num.clear();
    return num_bytes;
    
}
// Varbyte compression method used to encode each docID.
int VBEncode(unsigned int num, fstream& ofile){
    
    uint8_t b;
    vector<uint8_t> resultb;
    //cout<< "\nNum : "<< num;
    int num_bytes = 0;
    while(num >= 128){
        int a = num % 128;
        bitset<8> byte(a);
        byte.flip(7);
        num = (num - a) / 128;
        b = byte.to_ulong();
        resultb.push_back(b);
    }
    int a = num % 128;
    bitset<8> byte(a);
    b = byte.to_ulong();
    resultb.push_back(b);
    num_bytes = writeC(resultb, ofile);
    resultb.clear();
    return num_bytes;
        
}
// Create entry in the lexicon for each word term in the container, and
void populatemetadata(string word, vector<pair<int, int> >& did, fstream& ofile, fstream& mfile){
    
    //vector<char> encoded_stream;
    streampos startdid_pos = ofile.tellg();
    int num_bytes_did = 0;
    int num_bytes_freq = 0;
    int num_postings = did.size();
    cout<<"Did: "<<did[0].first<<"Freq: "<<did[0].second<<endl;
    sort(did.begin(),did.end());
    int num = did[0].first;
    num_bytes_did+=VBEncode(num, ofile);

    for(int j =1; j<did.size();j++)
    {
        num_bytes_did+=VBEncode(did[j].first-did[j-1].first, ofile);
        //cout<<"Did: "<<did[j].first<<"Freq: "<<did[j].second<<endl;
    }
    streampos startfreq_pos = ofile.tellg();
    for(int j =1; j<did.size();j++)
    {
        num_bytes_freq+=VBEncode(did[j].second, ofile);
        //cout<<"Did: "<<did[j].first<<"Freq: "<<did[j].second<<endl;
    }
    cout<<"##########   Writing into lexican  ################"<<endl;
   
    mfile<<word<<" "<<num_postings <<" "<<int(startdid_pos)<<" "<<num_bytes_did<<" "<<int(startfreq_pos)<<" "<<num_bytes_freq<<endl;
    cout<<word<<" "<<num_postings <<" "<<int(startdid_pos)<<" "<<num_bytes_did<<" "<<int(startfreq_pos)<<" "<<num_bytes_freq<<endl;

}

//Method to validate if string is good enough to create an entry in lexicon.
bool process (string s)
{   bool flag = false;
    if(s.length()>40) return false;
    
    return true;
//    for(int i =0;i<s.length();i++)
//    {
//        if(s[i]!=s[i+1]){
//            flag = true;
//            break;
//        }
//    }
//    return flag;
    
}
int main(){
    map<string, vector<pair<int, int> > > md; // Container that holds steam of pair of docID and frequency for each word term in the intermediate postings.
    fstream iefile,mfile,ofile;
    string line;
    iefile.open("intermediate_postings.txt");
    mfile.open("lexicon.txt"); // Text file that contains metadata entry for each word term.
    mfile<<"TERM    #POSTINGS  StartDid   SizeDid  StartFreq   SizeFreq"<<endl;
    ofile.open("invertedlist.dat", ios::out | ios::binary | ios::in);
    if(!ofile) {
       cout << "Cannot open file!" << endl;
       return 1;
    }
               
    string d= ",";
    size_t pos;
    string prev = "START";
    while(getline(iefile, line)){
    //split(line, '\w');
        
 
        if(line != "\0")
        {
            stringstream ss(line);
            string word, no1, no2;
            while (ss >> word)
            {
                // print the read word
                //cout << word << "--";
                if((md.size()>BUFSIZE) && (prev!=word)){
                    //cout<<"\nSafe to write into file"<<endl;
                    //cout<<"\nCurrent Word: "<< word<<" Prev word: "<<prev<<endl;
                    prev = word;
                    //cout<<"Map contents.....";
                    for(auto it = md.begin();it!= md.end();++it)
                    {
                        cout<<"******Compressing for word: "<<it->first<<"********\n";
                        string w = it->first;
                        if(process(w)){
                        
                        populatemetadata(it->first, it->second, ofile, mfile);
                        }
                        
                    }
                    cout<<"Clearing map contents...."<<endl;
                    md.clear();
                }
                else {
                    prev = word;
                    
                }
                getline(ss, no1,',');
                //md[word].push_back(stoi(no1));
                getline(ss, no2,',');
                md[word].push_back(make_pair(stoi(no1),stoi(no2)));
                break;
            }
        }

        //cout<<endl;
    }
    ofile.close();
    mfile.close();
    cout<<"############File END#############"<<endl;
    
    //mf.clear();
    iefile.close();
    return 0;
}


