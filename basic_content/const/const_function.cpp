#include<iostream>
using namespace std;

void f(const int i){
    i=10; // error: assignment of read-only parameter ‘i’
    cout<<i<<endl;
}

int main(){
    f(1);
}
