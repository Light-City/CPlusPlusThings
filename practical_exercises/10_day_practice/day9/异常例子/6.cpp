//Eg10-6.cpp
#include<iostream>
using namespace std;
void Errhandler(int n)throw(){
    try{
        if(n==1) throw n;
        if(n==2) throw "dx";
        if(n==3) throw 1.1;
    }
    catch(...){cout<<"catch an exception..."<<endl;}
}
int main(){
    Errhandler(1);
    Errhandler(2);
    Errhandler(3);
    system("pause");
    return 0;
} 
