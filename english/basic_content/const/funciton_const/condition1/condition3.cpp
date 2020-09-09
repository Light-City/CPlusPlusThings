#include<iostream>
using namespace std;

int main(){
    const int *ptr;
    int val = 3;
    ptr = &val; //ok
    int *ptr1 = &val;
    *ptr1=4;
    cout<<*ptr<<endl;

}
