//例题ch.cppi
#include <iostream>
using namespace std;
class X{
public:
	X &operator = (const X & x) 
	{cout << "a:"; return *this;};
};
int main ()
{
	X obj1, obj2, obj3;
	obj1 = obj2;          //调用重载“=”
	obj1.operator= (obj2); //调用重载“=”
	obj1 = obj2 = obj3;    //调用重载“=”
    system("pause");
}
