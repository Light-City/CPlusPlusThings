/*
基类中protected的成员
类内部：可以访问
类的使用者：不能访问
类的派生类成员：可以访问
*/
#include<iostream>
class B
{	
    private: 
        int i;
	protected: 
        int j;
	public: 
        int k;
};
class D: public B
{
public:
	void f() 
	{	
        i=1;//cannot access 派生类不可访问基类私有成员
		j=2;//派生类可以访问基类保护成员	
        k=3;	
    }
};
int main()
{	
    B b; 
	b.i=1;//cannot access 私有成员，类的使用者不能访问
	b.j=2; //cannot access 保护成员，类的使用者不能访问
	b.k=3;
    system("pause");
    return 0;
}

