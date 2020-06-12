/**
 * @file derived_full.cpp
 * @brief 完整示例！抽象类由派生类继承实现！
 * @author 光城
 * @version v1
 * @date 2019-07-20
 */

#include<iostream> 
using namespace std; 

class Base 
{ 
    int x; 
    public: 
    virtual void fun() = 0; 
    int getX() { return x; } 
}; 

class Derived: public Base 
{ 
    int y; 
    public: 
    void fun() { cout << "fun() called"; }  // 实现了fun()函数
}; 

int main(void) 
{ 
    Derived d; 
    d.fun(); 
    return 0; 
} 
