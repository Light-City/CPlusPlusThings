#include<iostream>
#include<iomanip>
using namespace std;

int main(int argc, char const *argv[])
{
    char s[20]="this is a string";
    double digit=-36.96656;
    cout<<setw(30)<<left<<setfill('*')<<s<<endl;
    cout<<dec<<setprecision(4)<<digit<<endl;
    cout<<dec<<15<<endl;
    //setbase(int x)设置进制后，后面所有操作都是按照这个进制来计算！
    cout<<setbase(10)<<15<<endl;
    //四舍五入,并保留2位有效数组
    float x=6.6937;
    cout<<float(int(x*1000+0.5)/1000.0)<<endl;
    system("pause");
    return 0;
}
