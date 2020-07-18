# Object oriented features of C + + implemented by C


## About Author：


![](../img/wechat.jpg)

## 1.C++ 

Polymorphism in C ++ :In C++, we usually maintain a virtual function table. According to the assignment compatibility rule, we know that the pointer or reference of the parent class can point to the child class object



If the pointer or reference of a parent class calls the virtual function of the parent class, the pointer of the parent class will look up its function address in its own virtual function table.If the pointer or reference of the parent object points to an object of a subclass, and the subclass has overridden the virtual function of the parent class, the pointer will call the overridden virtual function of the subclass.

Code Example:[c++_examp.cpp](./c++_examp.cpp)



## 2.C Implement

- Encapsulation

There is no concept of class class in C language. But with struct structure, we can use struct to simulate；

Encapsulating properties and methods into structures using function pointers.



- Inherit

Structure nesting


- Polymorphic

Class and subclass methods have different function pointers

There is no member function in the structure of C language. How to realize the common function of parent structure and child structure? We can consider using function pointers to simulate. But there is a flaw in this approach：The function pointer between the parent and child does not point to the virtual function table maintained in C ++, but a piece of physical memory. If there are too many simulated functions, it will not be easy to maintain.

In order to simulate polymorphism, function pointer variables must be aligned(They are completely consistent in content and alignment of variables).Otherwise, the parent class pointer points to the child class object, and the operation crashes!



Code example :[c_examp.c](./c_examp.c)
