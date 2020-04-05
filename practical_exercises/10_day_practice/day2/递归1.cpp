#include<iostream>
using namespace std;

int f(int n);
int main(int argc, char const *argv[])
{   
    cout<<"input x:";
    int x;
    cin>>x;
    cout<<f(x)<<endl;
    system("pause");
    return 0;
}

int f(int n)
{
    if(n==0)
    {
        return 1;
    }
    else
    {
        return n*f(n-1);
    }
}