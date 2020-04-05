//Eg9-2.cpp
#include <iostream>
using namespace std;
template <class T>
T Max(T a,T b) {
		return (a>b)?a:b;
}
/*
C++在实例化函数模板的过程中，只是简单地将模板参数替换成调用实参的类型，并以此生成模板函数，不会进行参数类型的任何转换。
*/
int main(){
    double a=2,b=3.4;
    float  c=5.1,d=3.2;
    //在模板调用时进行参数类型的强制转换
    cout<<"2, 3.2    的最大值是："<<Max(double(2),3.2)<<endl;
    cout<<"a, c    的最大值是："<<Max(float(a),c)<<endl;
    //显示指定函数模板实例化的参数类型
    cout<<"'a', 3    的最大值是："<<Max<int>('a',3)<<endl;
    system("pause");
}

