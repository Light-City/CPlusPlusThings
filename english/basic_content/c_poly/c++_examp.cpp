/**
 * @file c++_examp.cpp
 * @brief c++中的多态
 * @author 光城
 * @version v1
 * @date 2019-08-06
 */

#include <iostream>

using namespace std;
class A
{
    public:
        virtual void f()//Implement a virtual function
        {
            cout << "Base A::f() " << endl;
        }
};

class B:public A    // 必须为共有继承，否则后面调用不到，class默认为私有继承！
{
    public:
        virtual void f()//Virtual function implementation, virtual keyword in subclass can not be appearence
        {
            cout << "Derived B::f() " << endl;
        }
};


int main() 
{
    A a;//Base class object
    B b;//an object of derived type

    A* pa = &a;//The parent class pointer points to the parent object
    pa->f();//Call the function of the parent class
    pa = &b; //The parent class pointer points to the subclass object, which is implemented in polymorphism


    pa->f();//Call the function with the same name of the derived class
    return 0;
}
