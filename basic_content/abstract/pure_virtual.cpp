/**
 * @file pure_virtual.cpp
 * @brief 纯虚函数：没有函数体的虚函数
 * 抽象类：包含纯虚函数的类
 * 
 * @author 光城
 * @version v1
 * @date 2019-07-20
 */

#include<iostream>

using namespace std;
class A
{
private:
    int a;
public:
    virtual void show()=0; // 纯虚函数
};


int main()
{
    /*
     * 1. 抽象类只能作为基类来派生新类使用
     * 2. 抽象类的指针和引用->由抽象类派生出来的类的对象！
     */
    A a; // error 抽象类，不能创建对象

    A *a1; // ok 可以定义抽象类的指针

    A *a2 = new A(); // error, A是抽象类，不能创建对象
}
