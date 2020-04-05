#include<iostream>
using namespace std;
class X
{public:
	int operator() (int i=0)
	{ cout << "X::operator(" << i << ")" << endl; return i; };
	int operator() (int i, int j)
	{ cout << "X::operator(" << i << "," << j << ")" << endl; 
			return i; };
	int operator[] (int i)
	{ cout << "X::operator[" << i << "]" << endl; return i; };
	int operator[] (char * cp)
	{ cout << "X::operator[" << cp << "]" << endl; return 0; };
};
int main (void)
{	X obj;	int i = obj (obj (1), 2);
	int a = obj[i];	int b = obj["abcd"];
    cout<<"a="<<a<<endl;
    cout<<"b="<<b<<endl;
    system("pause");
}
