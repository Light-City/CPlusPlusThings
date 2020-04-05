//Eg9-6.cpp
#include <iostream>
#include<cstring>
using namespace std;
template <class T>
T Max(T a,T b) {
    return (a>b)?a:b;
}
//特化
//template <> 返回类型 函数名<特化的数据类型>(参数表) {}
template<>char * Max<char *>(char *a,char *b) {
    return (strcmp(a,b)>=0)?a:b;
}
int main(){
    float  c=5.1,d=3.2;
    cout<<"2,3的最大值是："<<Max(3,2)<<endl;   
    cout<<"c,d的最大值是："<<Max(c,d)<<endl;   
    cout<<Max("xbv","xyce")<<endl;         
    system("pause");
}
/*
① 当程序中同时存在模板和它的特化时，特化将被优先调用；
② 在同一个程序中，除了函数模板和它的特化外，还可以有同名的普通函数。其区别在于C++会对普通函数的调用实参进行隐式的类型转换，
但不会对模板函数及特化函数的参数进行任何形式的类型转换。

*/