/**
 * @file geninhe.cpp
 * @brief 1.普通单继承,继承就是基类+派生类自身的大小(注意字节对齐)
 * 注意：类的数据成员按其声明顺序加入内存，无访问权限无关，只看声明顺序。
 * 2.虚单继承，派生类继承基类vptr
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */

#include<iostream>

using namespace std;

class A
{
    public:
        char a;
        int b;
};

/**
 * @brief 此时B按照顺序：
 * char a
 * int b
 * short a
 * long b
 * 根据字节对齐4+4+8+8=24
 *
 * 或编译器优化
 * char a
 * short a
 * int b
 * long b
 * 根据字节对齐2+2+4+8=16
 */
class B:A
{
    public:
        short a;
        long b;
};
/**
* 把A的成员拆开看，char为1，int为4，所以是1+（3）+4+1+（3）=12，（）为字节补齐
*/
class C
{
    A a;
    char c;
};

class A1
{
    virtual void fun(){}
};
class C1:public A
{
};


int main()
{
    cout<<sizeof(A)<<endl; // 8
    cout<<sizeof(B)<<endl; // 16
    cout<<sizeof(C)<<endl; // 12

    /**
     * @brief 对于虚单函数继承，派生类也继承了基类的vptr，所以是8字节
     */
    cout<<sizeof(C1)<<endl; // 8 
    return 0;
}
