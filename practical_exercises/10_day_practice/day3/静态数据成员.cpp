/*
学习知识：
静态数据成员
用关键字static声明
该类的所有对象维护该成员的同一个拷贝
必须在类外定义和初始化，用(::)来指明所属的类。
*/
#include <iostream>
using namespace std;
class Point	
{
public:	
	Point(int xx=0, int yy=0) {X=xx; Y=yy; countP++; } 
    Point(Point &p);	
	int GetX() {return X;}
	int GetY() {return Y;}
	void GetC() {cout<<" Object id="<<countP<<endl;}
private:	
	int X,Y;
    //静态数据成员，必须在外部定义和初始化，内部不能直接初始化！
	static int countP;
};
Point::Point(Point &p)
{	X=p.X;
	Y=p.Y;
	countP++;
}
//必须在类外定义和初始化，用(::)来指明所属的类。
int Point::countP=0; 
int main()	
{	Point A(4,5);	
	cout<<"Point A,"<<A.GetX()<<","<<A.GetY();
	A.GetC();	
	Point B(A);	
	cout<<"Point B,"<<B.GetX()<<","<<B.GetY();
	B.GetC();	
    system("pause");
    return 0;
}
