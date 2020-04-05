#include<iostream>
using namespace std;
class base{
	int x;
public:
	void setx(int n){	x=n;	}
	int getx(){	return x;  }
	void showx()	{	cout<<x<<endl;  }
};
//派生类
class derived:public base{
	int y;
public:
	void sety(int n){	y=n;	}
	void sety(){	y=getx();    }
	void showy()
	{	cout<<y<<endl;   }
};
//派生类不可直接访问基类的private成员，可通过基类的共有成员函数访问
int main()
{	derived obj;
	obj.setx(10);	
    obj.sety(20);
	obj.showx();	
    obj.showy();
	obj.sety();
	obj.showx();	
    obj.showy();
    system("pause");
}
