#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <bitset>
#include <ctime>
using namespace std;


vector<uint8_t> resultb;
vector<char> result;

std::vector<int> VBDecode(vector<char> vec){
    char c;
    int num;
    int p;
    vector<int> resultD;

    for(vector<char>::iterator it = vec.begin(); it != vec.end(); it++){
        c = *it;
        bitset<8> byte(c);
        num = 0;
        p = 0;
        while(byte[7] == 1){
            byte.flip(7);
            num += byte.to_ulong()*pow(128, p);
            p++;
            it ++;
            c = *it;
            byte = bitset<8>(c);
        }
        num += (byte.to_ulong())*pow(128, p);
        //cout << "num: " << num << endl;
        resultD.push_back(num);
    }
    return resultD;
}
void split(const std::string &s, char delim) {
    std::stringstream ss;
    vector<char> v;
    vector<int> de;
    ss.str(s);
    std::string item;
    int j =0;
    while (std::getline(ss, item, delim)) {
        cout<<"ITEM:"<<item<<endl;
            v.push_back(item[0]);
    }
    de = VBDecode(v);
    for(vector<int>::iterator it = de.begin(); it != de.end(); it++){
        cout<<"VBdecode return: "<< *it<<endl;
        //ofile << *it << ' ';
    }

//    de.clear();
//    for(int k =0;k<5;k++){
//    de = VBDecode(v[k]);
//    for(vector<int>::iterator it = de.begin(); it != de.end(); it++){
//        //cout<<"VBdecode return: "<< *it<<endl;
//        ofile << *it << ' ';
//    }
//    }
    
}


int main(){
   
    fstream efile;
    efile.open("sample.txt");
    efile<<'A'<<'\t'<<'B'<<'\t'<<'C'<<endl;
    efile.close();
    fstream iefile;
    string line;
    iefile.open("sample.txt");
    std::getline(iefile, line);
    split(line, '\t');
    cout<<"After split"<<endl;
    iefile.close();
    return 0;
}
