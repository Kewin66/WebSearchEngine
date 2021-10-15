#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <bitset>
#include <ctime>
using namespace std;

void write(vector<uint8_t> num, fstream& ofile){
    for(vector<uint8_t>::iterator it = num.begin(); it != num.end(); it++){
        ofile.write(reinterpret_cast<const char*>(&(*it)), 1);
    }
}

void VBEncode(unsigned int num, fstream& ofile){
    vector<uint8_t> result;
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
    result.push_back(b);
    //cout<<" Encode value:";
//    for(vector<uint8_t>::iterator it = result.begin(); it != result.end(); it++){
//        cout<<(reinterpret_cast<const char*>(&(*it)));
//    }
//    cout<<endl;
    write(result, ofile);
}

void encode(){

    
    fstream ofile;
    ofile.open("test_comp");
    ofstream of;
    of.open("test_uncompress.txt");
    string line;
    string value;
    
    for(int i =0;i<500;i++)
    {
        
        of<< i << " ";
        
    }
    of<<endl;
    of.close();
    ifstream ifile;
    ifile.open("test_uncompress.txt");
    getline(ifile, line);
    cout<<"Line: "<<line<<endl;
    stringstream lineStream(line);
    while(lineStream >> value){
        VBEncode(stoi(value), ofile);
    }
    ofile.close();
    ifile.close();

}

int main(){
    int start=clock();
    encode();
    int finish=clock();
    cout << "Compressed in: " << (finish-start)/double(CLOCKS_PER_SEC) << " seconds." << endl;
    return 0;
}
