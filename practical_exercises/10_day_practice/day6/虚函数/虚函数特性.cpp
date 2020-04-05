#include <iostream>
#include<string>
using namespace std;
class A { 
public: 
    void f(int i){cout<<"…A"<<endl;}; 
};
class B: public A { 
public:
    virtual void f(int i){cout<<"…B"<<endl;}
};
class C: public B {
public: 
    void f(int i){cout<<"…C"<<endl;}
};
//一旦将某个成员函数声明为虚函数后，它在继承体系中就永远为虚函数了 
class D: public C{
public:
    void f (int){cout<<"…D"<<endl;}
};
int main(){
    A *pA,a;
    B *pB, b;    C c;    D d;
    pA=&a;    pA->f(1);		//调用A::f
    pB=&b;    pB->f(1);		//调用A::f
    pB=&c;    pB->f(1);		//调用A::f
    pB=&d;    pB->f(1);		//调用A::f
    system("pause");
    return 0;
}
