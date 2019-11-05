#include<iostream>
using namespace std;

int main(){

    const int p = 3;
    const int * const ptr = &p;
    cout<<*ptr<<endl;

}
