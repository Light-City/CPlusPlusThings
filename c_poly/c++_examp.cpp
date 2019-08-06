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
        virtual void f()//虚函数实现
        {
            cout << "Base A::f() " << endl;
        }
};

class B:public A    // 必须为共有继承，否则后面调用不到，class默认为私有继承！
{
    public:
        virtual void f()//虚函数实现,子类中virtual关键字可以没有
        {
            cout << "Derived B::f() " << endl;
        }
};


int main() 
{
    A a;//基类对象
    B b;//派生类对象

    A* pa = &a;//父类指针指向父类对象
    pa->f();//调用父类的函数

    pa = &b; //父类指针指向子类对象，多态实现
    pa->f();//调用派生类同名函数
    return 0;
}
