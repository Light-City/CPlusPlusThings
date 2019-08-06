/**
 * @file c_examp.c
 * @brief C实现多态
 * @author 光城
 * @version v1
 * @date 2019-08-06
 */

#include <stdio.h>

/// 重定义一个函数指针类型
typedef void (*pf) ();

/**
 * @brief 父类 
 */ 
typedef struct _A
{
    pf _f;
}A;


/**
 * @brief 子类
 */
typedef struct _B
{ 
    A _b; ///< 在子类中定义一个基类的对象即可实现对父类的继承。 
}B;

void FunA() 
{
    printf("%s\n","Base A::fun()");
}

void FunB() 
{
    printf("%s\n","Derived B::fun()");
}


int main() 
{
    A a;
    B b;

    a._f = FunA;
    b._b._f = FunB;

    A *pa = &a;
    pa->_f();
    pa = (A *)&b;   /// 让父类指针指向子类的对象,由于类型不匹配所以要进行强转 
    pa->_f();
    return 0;
}
