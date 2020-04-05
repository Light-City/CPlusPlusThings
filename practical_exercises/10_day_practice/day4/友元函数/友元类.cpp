#include<iostream>
using namespace std;
/*
若一个类为另一个类的友元，则此类的所有成员都能访问对方类的私有成员。
声明语法：将友元类名在另一个类中使用friend修饰说明。
*/

/*
如果声明B类是A类的友元，B类的成员函数就可以访问A类的私有和保护数据，
但A类的成员函数却不能访问B类的私有、保护数据。
*/
class A{
  friend class B;
  public:
    void Display(){
      cout<<x<<endl;
    }
    private:
      int x;
};
class B
{   public:
      void Set(int i);
      void Display();
    private:
      A a;
};
void B::Set(int i)
{
   a.x=i;
}
void B::Display()
{
   a.Display();
}

int main(int argc, char const *argv[])
{
    B b;
    b.Set(10);
    b.Display();

    system("pause");
    return 0;
}

/*
如果声明B类是A类的友元，B类的成员函数就可以访问A类的私有和保护数据，但A类的成员函数却不能访问B类的私有、保护数据
*/