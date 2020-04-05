#include <iostream>
using namespace std;
class A { 
public: 
    A(){ cout<<"Constructing A"<<endl; } 
    ~A(){ cout<<"Destructing A"<<endl; }
};
class B:public A {
public: 
   ~B(){ cout<<"Destructing B"<<endl; }
};
int main(){
    B b;
    system("pause");
}
