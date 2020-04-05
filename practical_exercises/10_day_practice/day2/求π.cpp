#include<iostream>
using namespace std;
double arctan(double x);
int main(int argc, char const *argv[])
{
    double a = 16.0*arctan(1.0/5.0);
    double b = 4.0*arctan(1.0/239.0);
    double pi = a-b;
    cout<<pi<<endl;
    system("pause");
    return 0;
}
double arctan(double x)
{
    double head=x;
    int tail=1;
    double art=0;
    while(double(head/tail)>1e-15)
    {
        art=(tail%4==1)? art+head/tail: art-head/tail;
        head*=x*x;
        tail+=2;
        cout<<"---------------"<<endl;
        cout<<tail<<endl;
        cout<<"---------------"<<endl;
    }
    return art;
}
