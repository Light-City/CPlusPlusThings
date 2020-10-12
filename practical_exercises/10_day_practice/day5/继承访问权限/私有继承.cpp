#include <iostream>
using namespace std;
class Base{
    int x;
public:
    void setx(int n){x=n; }
    int getx(){return x; }
    void showx(){cout<<x<<endl; }
};
//私有继承
//基类的中的public成员在派生类中是private, private成员在派生类中不可访问。
class derived:private Base{
	int y;
public:
	void sety(int n){y=n;	}
	void sety(){	y=getx();    }
	void showy()	{	cout<<y<<endl;   }
};
int main(){
	derived obj;
	obj.setx(10);//cannot access
	obj.sety(20);
	obj.showx();//cannot access
	obj.showy();	
}
