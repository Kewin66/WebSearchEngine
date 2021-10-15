#define INDEX "/home/qi/forwardIndex/compressedIndex"
#define INFO "/home/qi/forwardIndex/docInfo"
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
void writeC(vector<uint8_t> num, fstream& efile){
    for(vector<uint8_t>::iterator it = num.begin(); it != num.end(); it++){
        result.push_back(*it);
        efile.write(reinterpret_cast<const char*>(&(*it)), 1);
        efile<<'\t';
    }
    
}
void VBEncode(unsigned int num, fstream& efile){
    
    uint8_t b;
    cout<< "Num : "<< num;
    while(num >= 128){
        int a = num % 128;
        bitset<8> byte(a);
        byte.flip(7);
        num = (num - a) / 128;
        b = byte.to_ulong();
        result.push_back(b);
    }
    int a = num % 128;
    bitset<8> byte(a);
    b = byte.to_ulong();
    resultb.push_back(b);
    writeC(resultb, efile);
    //cout<<" Encode value:";
//    for(vector<uint8_t>::iterator it = result.begin(); it != result.end(); it++){
//        cout<<(reinterpret_cast<const char*>(&(*it)));
//    }
//    cout<<endl;
    
}

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
void split(const std::string &s, char delim, ofstream& ofile) {
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
        ofile << *it << ' ';
    }
//    for(int k =0;k<5;k++){
//    de = VBDecode(v[k]);
//    for(vector<int>::iterator it = de.begin(); it != de.end(); it++){
//        //cout<<"VBdecode return: "<< *it<<endl;
//        ofile << *it << ' ';
//    }
//    }
    
}

void decomp_index(){
   // ifstream index;
   // ifstream info;
    ofstream ofile;
    fstream efile;
    efile.open("encode.txt");
//    index.open(INDEX);
//    info.open(INFO);
    ofile.open("uncomp");
//    string line;
//    string value;
    vector<string> vec;
    char c;
    vector<char> cv;
    

//    for(int i = 0; i < 10000; i ++){
//        vec.clear();
//        getline(info, line);
//        stringstream lineStream(line);
//        while(lineStream >> value){
//            vec.push_back(value);
//        }
//        index.seekg(stoi(vec[2]));
//        while(index.tellg() != (stoi(vec[2]) + stoi(vec[3]))){
//            index.get(c);
//            cv.push_back(c);
//        }
        VBEncode(16,efile);
        
        VBEncode(18,efile);
    VBEncode(13,efile);
    VBEncode(19, efile);
    VBEncode(15, efile);
    efile.close();
    ifstream iefile;
    iefile.open("encode.txt");
    string line;
    while (std::getc(iefile, line) != EOF)
    {
    ;
        split(line, '\t', ofile);
        cout<<"After split"<<endl;
    
     }
        


    ofile << endl;
        //cv.clear();
//}
    ofile.close();
//    index.close();
//    info.close();

}

int main(){
    int start=clock();
    decomp_index();
    int finish=clock();
    cout << "Decompressed in: " << (finish-start)/double(CLOCKS_PER_SEC) << " seconds." << endl;
    return 0;
}
