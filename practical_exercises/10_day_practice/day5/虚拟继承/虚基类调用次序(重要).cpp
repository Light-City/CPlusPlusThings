//живЊ!!!
#include <iostream>
using namespace std;
class A {
    int a;
public: 
    A(){ cout<<"Constructing A"<<endl; }
}; 
class B {
public: 
    B(){ cout<<"Constructing B"<<endl;}
}; 
class B1:virtual public B ,virtual public A{
public: 
    B1(int i){ cout<<"Constructing B1"<<endl; }
}; 
class B2:public A,virtual public B {
public: 
    B2(int j){ cout<<"Constructing B2"<<endl; }
};
class D: public B1, public B2 {
public:
    D(int m,int n): B1(m),B2(n){ cout<<"Constructing D"<<endl; }
    A a;
}; 
    
int main(){
    D d(1,2);
    system("pause");
    return 0;
}
