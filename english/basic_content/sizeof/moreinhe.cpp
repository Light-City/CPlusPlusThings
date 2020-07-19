/**
 * @file moreinhe.cpp
 * @brief 普通多继承与虚函数多继承
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

class B
{
    public:
        short a;
        long b;
};

/**
 * @brief 8+16+8=32
 */
class C:A,B
{
    char c;
};


int main()
{
    cout<<sizeof(A)<<endl; // 8
    cout<<sizeof(B)<<endl; // 16
    cout<<sizeof(C)<<endl; // 32

    return 0;
}
