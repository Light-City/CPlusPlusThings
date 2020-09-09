# Class size calculation

First of all, let's make a summary, and then we will give an actual example

- The size of the empty class is 1 byte
- In a class, the virtual function itself, member functions (including static and non-static) and static data members do not occupy the storage space of class objects
- For classes containing virtual functions, no matter how many virtual functions there are, there is only one virtual pointer, the size of VPTR
- Ordinary inheritance, derived class inherits all the functions and members of the base class, and the size is calculated according to byte alignment
- Virtual function inheritance, whether single inheritance or multi inheritance, inherits the VPTR of the base class(32 bit operating system 4 bytes, 64 bit operating system 8 bytes)！
- Virtual inheritance inherits the VPTR of the base class

## 1.Rule 1

```c++
/**
 * @file blackclass.cpp
 * @brief The size of the empty class is 1 byte
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */
#include<iostream>
using namespace std;
class A{};
int main()
{
    cout<<sizeof(A)<<endl;
    return 0;
}
```

## 2.Rule 2

```c++
/**
 * @file static.cpp
 * @brief
 * 
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */
#include<iostream>
using namespace std;
class A
{
    public:
        char b;
        virtual void fun() {};
        static int c;
        static int d;
        static int f;
};

int main()
{
    /**
     * @brief 16 
     */
    cout<<sizeof(A)<<endl; 
    return 0;
}
```

## 3.Rule 3

```c++
/**
 * @file morevir.cpp
 * @brief 
 * @author 
 * @version v1
 * @date 2019-07-21
 */
#include<iostream>
using namespace std;
class A{
    virtual void fun();
    virtual void fun1();
    virtual void fun2();
    virtual void fun3();
};
int main()
{
    cout<<sizeof(A)<<endl; // 8
    return 0;
}
```

## 4.Rule 4 and 5

```c++
/**
 * @file geninhe.cpp
 * @brief 
 * 
 * 
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */

#include<iostream>

using namespace std;

class A
{
    public:
        char a;
        int b;
};

/**
 * @brief 
 * char a
 * int b
 * short a
 * long b
 * 根据字节对齐4+4=8+8+8=24
 */
class B:A
{
    public:
        short a;
        long b;
};
class C
{
    A a;
    char c;
};
class A1
{
    virtual void fun(){}
};
class C1:public A1
{
};

int main()
{
    cout<<sizeof(A)<<endl; // 8
    cout<<sizeof(B)<<endl; // 24
    cout<<sizeof(C)<<endl; // 12
    /**
     * @brief 
     */
    cout<<sizeof(C1)<<endl; // 8 
    return 0;
}
```

## 5.Rule 6

```c++
/**
 * @file virnhe.cpp
 * @brief 
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */

#include<iostream>
using namespace std;
class A
{
    virtual void fun() {}
};
class B
{
    virtual void fun2() {}
};
class C : virtual public  A, virtual public B
{
    public:
        virtual void fun3() {}
};

int main()
{
    /**
     * @brief 8 8 16  The derived class inherits multiple virtual functions and inherits the VPTR of all virtual functions
     */
    cout<<sizeof(A)<<" "<<sizeof(B)<<" "<<sizeof(C);

    return 0;
}
```

