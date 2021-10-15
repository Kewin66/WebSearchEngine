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

int main(){
    
    fstream ip;
    ip.open("input.dat", ios::binary|ios::out| ios::in);
    ip<<"100";
    ip<<"200";
    ip<<"390";
    streampos pos = ip.tellg();
    cout<<"Pointer at initial: "<<pos<<endl;
    ip.seekg(3, ios::beg);
    pos = ip.tellg();
    cout<<"Pointer after seeking 3 from beg: "<<pos<<endl;
    char op[4];
    ip.read(op,4);
    //ip<<"YOUGOTIT";
    cout<<"4th char after seekg 3 from beg: "<<op[3];
    pos = ip.tellp();
    ip.seekg(pos, ios::beg);
    ip.read(op,1);
    cout<<"\nPointer tellp after read: "<<op[0]<<endl;
    ip.seekg(0, ios::end);
    ip<<"YOUGOTIT";
    ip.close();
    
    
    
    return 0;
}
