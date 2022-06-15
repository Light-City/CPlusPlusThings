#include "Test.h"
#include <iostream>
using namespace std;

Test::Test()
{
   cout<<"无参构造"<<endl;
}

Test::Test(int a)
{
    cout<<"类型转换构造器  有参构造"<<endl;
}

Test::Test(const Test &t)
{
    cout<<"拷贝构造"<<endl;
}

Test::~Test()
{
    cout<<"析构函数"<<endl;
}

void Test::fun() const
{
    a = 1;  //mutable修饰的成员变量可以被修改
    //b = 2;  //非mutable修饰的变量禁止被修改 
}
