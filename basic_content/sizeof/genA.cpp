/**
 * @file genA.cpp
 * @brief 普通成员函数，大小为1,一个类中，虚函数本身、成员函数（包括静态与非静态）和静态数据成员都是不占用类对象的存储空间。
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */

#include<iostream>


using namespace std;

class A
{
    public:
        A();
        ~A();
        static int a;
        static void fun3();
        void fun();
        void fun1();
};

int main()
{
    cout<<sizeof(A)<<endl; // 1
    return 0;
}
