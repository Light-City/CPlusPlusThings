#include<iostream>
using namespace std;
class A
{
    public:
        static void f(A a);
    private:
        int x;
};
void A::f(A a)
{
   
    //静态成员函数只能引用属于该类的静态数据成员或静态成员函数。
    // cout<<x; //对x的引用是错误的
    cout<<a.x;  //正确
}

int main(int argc, char const *argv[])
{
    A a;
    a.f(A());
    system("pause");
    return 0;
}
