//Eg9-1.cpp
#include <iostream>
//注意一点，max与min使用的时候，容易引起冲突，如果写了下面这一行代码，则要改变函数模板名字，否则直接使用std::cout与std::endl
using namespace std;
/*
不要把这里的class与类的声明关键字class混淆在一起，虽然它们由相同的字母组成，但含义是不同的。
这里的class表示T是一个类型参数，可以是任何数据类型，如int、float、char等，或者用户定义的struct、enum或class等自定义数据类型。
*/
template <class T>
T Min(T a,T b) {
	return (a<b)?a:b;
}
/*
为了区别类与模板参数中的类型关键字class，标准C++提出?了用typename作为模板参数的类型关键字，同时也支持使用class。
比如，把min定义的template <class T>写成下面的形式是完全等价的：
*/
template <typename T>
T myMin(T a, T b){
    return (a<b)?a:b;
}

/*
模板实例化发生在调用模板函数时。当编译器遇到程序中对函数模板的调用时，
它才会根据调用语句中实参的具体类型，确定模板参数的数据类型，
并用此类型替换函数模板中的模板参数，生成能够处理该类型的函数代码，即模板函数。
当多次发生类型相同的参数调用时，只在第1次进行实例化。编译器只在第1次调用时生成模板函数，
当之后遇到相同类型的参数调用时，不再生成其他模板函数，它将调用第1次实例化生成的模板函数。
*/
int main(){
    double a=2,b=3.4;
    float  c=2.3,d=3.2;
    cout<<"2，3    的最小值是："<<Min<int>(2,3)<<endl; //显式调用
    cout<<"2，3.4  的最小值是："<<Min(a,b)<<endl;//隐式调用
    cout<<"'a'，'b'	  的最小值是："<<Min('a','b')<<endl;
    cout<<"2.3，3.2的最小值是："<<Min(c,d)<<endl;
    cout<<"2.3，3.2的最大值是："<<std::min(c,d)<<endl;//引用命名空间内部的最小值函数
    cout<<"2.3，3.2的最小值是："<<myMin(c,d)<<endl;//更换class为typename
    // cout<<"2，'a'    的最小值是："<<Min(2,'a')<<endl; //报错,不同类型无法处理，请看9-3-1.cpp
    system("pause");
    return 0;
}


