#include <iostream>
#include "Test.h"
using namespace std;


void fun(const Test& t){
    t.a = 10;
}

void fun2(const Test& t){
    t.a = 10;
}

void fun3(const Test& t){
    t.a = 10;
}
int main()
{
    Test t_m;
    t_m.a = 2;

    cout<<"==================================="<<endl;

    cout<<"调用fun前:"<<t_m.a<<endl;//2
    fun(2);//隐式转换 隐式类型转换调用类型转换构造函数 发生类型转换const引用再函数中生产临时对象
    cout<<"调用fun后:"<<t_m.a<<endl;//2

    cout<<"==================================="<<endl;

    cout<<"调用fun2前:"<<t_m.a<<endl;//2
    fun2(Test(t_m));//显示转换 显式转换调用拷贝构造函数 发生类型转换const引用再函数中生产临时对象
    cout<<"调用fun2后:"<<t_m.a<<endl;//2

    cout<<"==================================="<<endl;

    cout<<"调用fun3前:"<<t_m.a<<endl;//2
    fun3(t_m);//没有发生类型转换 不产生临时变量
    cout<<"调用fun3后:"<<t_m.a<<endl;//10

    return 0;
}
