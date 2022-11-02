#include <fstream>
#include <string>
#include<vector>
#include <sstream>
#include <random>
#include <iostream>
#include<map>
#include<vector>
using namespace std;
 
int main()
{   
    int maxnode = 10000;
    map<int,bool*> Matrix;
    for(int i=0;i<maxnode;i++){
        bool temp[maxnode];
        Matrix[i]=temp;
    }
    cout<<Matrix[1]<<endl;    
    cout<<"内存分配成功"<<endl;
}