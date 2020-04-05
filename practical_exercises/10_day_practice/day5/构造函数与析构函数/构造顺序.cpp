#include<iostream>
using namespace std;

/*
先构造成员
再构造自身（调用构造函数）
*/

class A { 
public: 
	A() { cout<<"Constructing A"<<endl;} 
	~A(){ cout<<"Destructing A"<<endl;}
};
class B {
public: 
	B() { cout<<"Constructing B"<<endl;}
	~B(){ cout<<"Destructing B"<<endl;}
};

class C 
{
public: 
	C() { cout<<"Constructing C"<<endl;}
	~C(){ cout<<"Destructing C"<<endl;}
	B b;
	A a;
};

int main()
{	
    C c; 
    system("pause");
}

/*
执行结果：
Constructing B
Constructing A
Constructing C
Destructing C
Destructing A
Destructing B
*/