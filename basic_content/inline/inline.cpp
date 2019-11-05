#include <iostream>
#include "inline.h"



using namespace std;

/**
 * @brief inline要起作用,inline要与函数定义放在一起,inline是一种“用于实现的关键字,而不是用于声明的关键字”
 *
 * @param x
 * @param y
 *
 * @return 
 */
int Foo(int x,int y);  // 函数声明
inline int Foo(int x,int y) // 函数定义
{
    return x+y;
}



// 定义处加inline关键字，推荐这种写法！
inline void A::f1(int x){


}



/**
 * @brief 内联能提高函数效率，但并不是所有的函数都定义成内联函数！内联是以代码膨胀(复制)为代价，仅仅省去了函数调用的开销，从而提高函数的执行效率。
 * 如果执行函数体内代码的时间相比于函数调用的开销较大，那么效率的收货会更少！另一方面，每一处内联函数的调用都要复制代码，将使程序的总代码量增大，消耗更多的内存空间。
 * 以下情况不宜用内联：
 *  （1） 如果函数体内的代码比较长，使得内联将导致内存消耗代价比较高。
 *   (2)  如果函数体内出现循环，那么执行函数体内代码的时间要比函数调用的开销大。
 *
 * @return 
 */
int main()
{

    
    cout<<Foo(1,2)<<endl;

}
/**
 * 编译器对 inline 函数的处理步骤
 * 将 inline 函数体复制到 inline 函数调用点处；
 * 为所用 inline 函数中的局部变量分配内存空间；
 * 将 inline 函数的的输入参数和返回值映射到调用方法的局部变量空间中；
 * 如果 inline 函数有多个返回点，将其转变为 inline 函数代码块末尾的分支（使用 GOTO）。
 */



