/*
派生方式为protected的继承称为保护继承，在这种继承方式下，
基类的public成员在派生类中会变成protected成员，
基类的protected和private成员在派生类中保持原来的访问权限
*/
#include <iostream>
using namespace std;
class Base{
    int x;
protected:
    int getx(){ return x; }
public:
    void setx(int n){ x=n; }
    void showx(){ cout<<x<<endl; }
};
class Derived:protected Base{
    int y;
public:
    void sety(int n){ y=n; }
    void sety(){ y=getx();}   //访问基类的保护成员
    void showy(){ cout<<y<<endl; }
};
int main(){
    Derived obj;
    obj.setx(10);    //错误
    obj.sety(20);
    obj.showx();	 //错误， 
    obj.showy(); 
    system("pause");   
}

/*
解释：
如最上面文字所示：保护继承会将基类的public变为protected，而对于protected成员，
外部去使用保护成员的时候，会报错，所以setx与showx访问错误，而对于派生类，则可直接访问基类的保护成员，
在派生类中，y=getx()可正常访问！
*/