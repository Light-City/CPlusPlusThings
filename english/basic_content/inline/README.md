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
 * 编译器对 inline 函数的处理步骤
 * 将 inline 函数体复制到 inline 函数调用点处；
 * 为所用 inline 函数中的局部变量分配内存空间；
 * 将 inline 函数的的输入参数和返回值映射到调用方法的局部变量空间中；
 * 如果 inline 函数有多个返回点，将其转变为 inline 函数代码块末尾的分支（使用 GOTO）。
 */

```

内联能提高函数效率，但并不是所有的函数都定义成内联函数！内联是以代码膨胀(复制)为代价，仅仅省去了函数调用的开销，从而提高函数的执行效率。

- 如果执行函数体内代码的时间相比于函数调用的开销较大，那么效率的收货会更少！

- 另一方面，每一处内联函数的调用都要复制代码，将使程序的总代码量增大，消耗更多的内存空间。

以下情况不宜用内联：

（1）如果函数体内的代码比较长，使得内联将导致内存消耗代价比较高。

（2）如果函数体内出现循环，那么执行函数体内代码的时间要比函数调用的开销大。

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



