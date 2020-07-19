/**
 * @file interesting_facts4.cpp
 * @brief 抽象类可以有构造函数
 * @author 光城
 * @version v1
 * @date 2019-07-20
 */

#include<iostream> 
using namespace std; 

// An abstract class with constructor 
class Base 
{ 
    protected: 
        int x; 
    public: 
        virtual void fun() = 0; 
        Base(int i) { x = i; } 
}; 

class Derived: public Base 
{ 
    int y; 
    public: 
    Derived(int i, int j):Base(i) { y = j; } 
    void fun() { cout << "x = " << x << ", y = " << y; } 
}; 

int main(void) 
{ 
    Derived d(4, 5); 
    d.fun(); 
    return 0; 
} 
