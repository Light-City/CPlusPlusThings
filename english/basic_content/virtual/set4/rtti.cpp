/**
 * @file rtti.cpp
 * @brief 在面向对象程序设计中，有时我们需要在运行时查询一个对象是否能作为某种多态类型使用。与Java的instanceof，以及C#的as、is运算符类似，C++提供了dynamic_cast函数用于动态转型。相比C风格的强制类型转换和C++ reinterpret_cast，dynamic_cast提供了类型安全检查，是一种基于能力查询(Capability Query)的转换，所以在多态类型间进行转换更提倡采用dynamic_cast
 * @author 光城
 * @version v1
 * @date 2019-07-24
 */

// CPP program to illustrate  
// // Run Time Type Identification  
#include<iostream>
#include<typeinfo>
using namespace std; 
class B { virtual void fun() {} }; 
class D: public B { }; 

int main() 
{ 
    B *b = new D;  // 向上转型
    B &obj = *b;
    D *d = dynamic_cast<D*>(b);   // 向下转型
    if(d != NULL) 
        cout << "works"<<endl; 
    else
        cout << "cannot cast B* to D*"; 
 
    try {
        D& dobj = dynamic_cast<D&>(obj);  
        cout << "works"<<endl; 
    } catch (bad_cast bc) { // ERROR
        cout<<bc.what()<<endl;
    }
    return 0;
}
