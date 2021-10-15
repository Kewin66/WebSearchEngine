#include <iostream>

struct Ournode {

  char x, y, z;

};

 
using namespace std;
int main() {

    vector<uint8_t> u8;
    u8.push_back(15);
    u8.push_back(9);
    vector<char> cp;
    int i =0;
    for(auto e: u8)
    {
        cp[i] = e;
        i++;
    }
    cout<<cp[0]<<endl;


  return 0;

}
