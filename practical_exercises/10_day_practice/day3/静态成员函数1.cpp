/*
知识点：
静态成员函数
类外代码可以使用类名和作用域操作符来调用静态成员函数。
静态成员函数只能引用属于该类的静态数据成员或静态成员函数。
*/
#include<iostream>
using namespace std;
class Application
{ 
public:
    static void f(); 
    static void g();
private:
    static int global;
};
int Application::global=0;
void Application::f()
{  global=5;}
void Application::g()
{  cout<<global<<endl;}

int main()
{
    Application::f();
    Application::g();
    system("pause");
    return 0;
}

