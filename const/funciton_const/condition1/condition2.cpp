#include<iostream>
using namespace std;


int main(){
    const int p = 10;
    const void *vp = &p;
    void *vp = &p; //error
}
