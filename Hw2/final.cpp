#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <bitset>
#include <ctime>
using namespace std;

void split(const std::string &s, char delim) {
    std::stringstream ss;
    vector<char> v;
    ss.str(s);
    std::string item;
    int j =0;
    while (std::getline(ss, item, delim)) {
        if(item!= " ")cout<<"ITEM:"<<item<<endl;
    }
    
}


int main(){
    map<string, vector<int> > md;
    map<string, vector<int> > mf;
    fstream iefile;
    string line;
    iefile.open("intermediate_postings.txt");
    string d= ",";
    size_t pos;
    while(getline(iefile, line)){
    //split(line, '\w');
        
 
        if(line != "\0")
        {
            stringstream ss(line);
            string word, no1, no2;
            while (ss >> word)
            {
                // print the read word
                cout << word << "--";
                getline(ss, no1,',');
                cout<<"DocID: "<<stoi(no1)<<"  ";
                md[word].push_back(stoi(no1));
                getline(ss, no2,',');
                cout<<"Freq: "<<stoi(no2)<<endl;
                mf[word].push_back(stoi(no1));
                break;
            }
        }

        //cout<<endl;
    }
    cout<<"############File END#############"<<endl;
    iefile.close();
    return 0;
}

