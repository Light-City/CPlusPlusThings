//Eg12-5.cpp
#include<iostream>
#include<iomanip>
using namespace std;
int main(){
    char c[30]="this is string";
    double d=-1234.8976;
    cout<<setw(30)<<left<<setfill('*')<<c<<"----L1"<<endl;
    cout<<setw(30)<<right<<setfill('*')<<c<<"----L2"<<endl;
    //showbase显示数值的基数前缀
    cout<<dec<<showbase<<showpoint<<setw(30)<<d<<"----L3"<<"\n";
    //showpoint显示小数点
    cout<<setw(30)<<showpoint<<setprecision(10)<<d<<"----L4"<<"\n";
    //setbase(8)设置八进制
    cout<<setw(30)<<setbase(16)<<100<<"----L5"<<"\n";
    system("pause");
}
