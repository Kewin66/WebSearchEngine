//
//  main.cpp
//  InvertedINdex
//
//  Created by Kewin Srinath on 10/8/21.
//

#include <iostream>
#include <map>
#include<vector>
#include <fstream>
using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...

    
    fstream fp;
    fp.open ("sample.txt", ios::in | ios::out);
    string s;
    
    while(!getline(fp, s)){
        cout<<"Line is : "<<s<<endl;
    }
    fp.close();
    return 0;
}
