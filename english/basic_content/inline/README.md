# Story About Inline

## About Author：



![](../img/wechat.jpg)

## 1.Inline in Class

Declaration method in header file


```c++

class A
{
public:
    void f1(int x); 

    /**
     * @brief The function defined in the class is an implicit inline function. If you want to be an inline function, you must add the inline keyword at the implementation (definition)
     *
     * @param x
     * @param y
     */
    void Foo(int x,int y) ///< The definition is implicit inline function
    {
    
    };
    void f1(int x); ///< To be an inline function after declaration, you must add the inline keyword to the definition  
};
```

The inline function is defined in the implementation file:


```c++
#include <iostream>
#include "inline.h"

using namespace std;

/**
 * @brief To work, inline should be placed with function definition. Inline is a kind of "keyword for implementation, not for declaration"
 *
 * @param x
 * @param y
 *
 * @return 
 */
int Foo(int x,int y);  // Function declaration


inline int Foo(int x,int y) // Function definition
{
    return x+y;
}

// It is recommended to add the keyword "inline" to the definition！
inline void A::f1(int x){

}

int main()
{

    
    cout<<Foo(1,2)<<endl;

}
/**
 * Compiler's processing steps for inline function
 * Copy the body of the inline function to the call point of the inline function;
 * Allocate memory space for local variables in the inline function used
 * The input parameters and return values of the inline function are mapped to the local variable space of the calling method
 * If the inline function has multiple return points, convert it to a branch at the end of the inline function code block (using goto)
 */

```

Inline can improve the efficiency of functions, but not all functions are defined as inline functions！

-If the execution time of the code in the function body is higher than the cost of function call, the efficiency of receiving goods will be less！

- On the other hand, every call of inline function will copy the code, which will increase the total code of the program and consume more memory space

Inline is not suitable for the following situations：

（1）If the code in the function body is long, it will lead to high memory consumption

（2）If there is a loop in the function body, it takes more time to execute the code in the function body than the cost of the function call。

## 2.Could virtual be inline function?

- Virtual functions can be inline functions, which can modify virtual functions, but cannot be inlined when they are polymorphic
- Inline means that the compiler is advised to inline at compile time. However, due to the polymorphism of virtual functions, the compiler cannot know which code to call at runtime. Therefore, when the virtual function is polymorphic (runtime), it cannot be inlined.
- `inline virtual` The only time you can inline is when the compiler knows which class the object is calling（such as: `Base::who()`），This only happens if the compiler has an actual object instead of a pointer or reference to the object.

```c++
#include <iostream>  
using namespace std;
class Base
{
public:
    inline virtual void who()
    {
        cout << "I am Base\n";
    }
    virtual ~Base() {}
};
class Derived : public Base
{
public:
    inline void who()  //
    {
        cout << "I am Derived\n";
    }
};

int main()
{
    // 
    Base b;
    b.who();

    // 
    Base *ptr = new Derived();
    ptr->who();

    // 
    delete ptr;
    ptr = nullptr;

    system("pause");
    return 0;
} 
```



