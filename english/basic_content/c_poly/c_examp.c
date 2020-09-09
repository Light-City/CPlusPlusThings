/**
 * @file c_examp.c
 * @brief C实现多态
 * @author 光城
 * @version v1
 * @date 2019-08-06
 */

#include <stdio.h>

/// Define a pointer of function
typedef void (*pf) ();

/**
 * @brief parent class
 */ 
typedef struct _A
{
    pf _f;
}A;


/**
 * @brief child class
 */
typedef struct _B
{ 
    A _b; ///< The inheritance of the parent class can be realized by defining an object of the base class in the subclass 
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
    pa = (A *)&b;   /// The parent class pointer points to the object of the subclass. Because of the type mismatch, it needs to be forced
    pa->_f();
    return 0;
}
