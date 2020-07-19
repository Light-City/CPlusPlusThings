/**
 * @file morevir.cpp
 * @brief 对于包含虚函数的类，不管有多少个虚函数，只有一个虚指针,vptr的大小。
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */

#include<iostream>

using namespace std;

class A{

    virtual void fun();
    virtual void fun1();
    virtual void fun2();
    virtual void fun3();
};
int main()
{
    cout<<sizeof(A)<<endl;
    return 0;
}
