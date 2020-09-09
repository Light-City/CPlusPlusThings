# Pure virtual functions and abstract classes

## About Author：



![](../img/wechat.jpg)

## 1.Pure virtual function and abstract class


Pure virtual functions (or abstract functions) in C + + are virtual functions that we have not implemented!We just need to state it! 
example:
```cpp
// abstract class
Class A {
public: 
    virtual void show() = 0; // pure virtual function
    /* Other members */
}; 
```

 * Pure virtual function: virtual function without function body
 * Abstract classes: classes containing pure virtual functions

Abstract classes can only be used as base classes to derive new classes. Objects, pointers and references of abstract classes cannot be created->An object of a class derived from an abstract class!

> Code example：[test.cpp](./test.cpp)、[pure_virtual.cpp](./pure_virtual.cpp)

## 2.Implement abstract classes

Abstract class：Pure virtual functions can be called within member functions.Pure virtual functions cannot be used inside constructors / destructors.

If a class derives from an abstract class, it must implement all pure virtual functions in the base class to become a non abstract class.
```cpp
// A is abstract class
class A {
public:
    virtual void f() = 0;  // pure virtual function
    void g(){ this->f(); }
    A(){}  // 构造函数
};

class B : public A{
public:
    void f(){ cout<<"B:f()"<<endl;}  // 实现了抽象类的纯虚函数
};
```

> Code Example：[abstract.cpp](./abstract.cpp)

## 3.Key point

- [Pure virtual functions make a class abstract](./interesting_facts1.cpp)
```cpp
// Abstract class contains at least one pure virtual function
class Base{
public: 
    virtual void show() = 0; // 纯虚函数
    int getX() { return x; } // 普通成员函数

private:
     int x; 
}; 
```

- [Pointers and references to abstract class types](./interesting_facts2.cpp)
```cpp
class Derived : public Base { 
public: 
    void show() { cout << "In Derived \n"; } // 实现抽象类的纯虚函数
    Derived(){} // 构造函数
}; 

int main(void) 
{ 
    //Base b;  // error! 不能创建抽象类的对象
    //Base *b = new Base(); error!
    
    Base *bp = new Derived(); // 抽象类的指针和引用 -> 由抽象类派生出来的类的对象
    bp->show();
    return 0; 
}
```

- [If we do not override pure virtual functions in the derived class, the derived class will also become an abstract class](./interesting_facts3.cpp)
```cpp
// Derived为抽象类
class Derived: public Base 
{ 
public: 
//    void show() {}
}; 
```

- [Abstract classes can have constructors](./interesting_facts4.cpp)
```cpp
// abstract class
class Base { 
    protected: 
        int x; 
    public: 
        virtual void fun() = 0; 
        Base(int i) { x = i; }  // constructor function
}; 
// 派生类
class Derived: public Base 
{ 
    int y; 
public: 
    Derived(int i, int j) : Base(i) { y = j; } // constructor function
    void fun() { cout << "x = " << x << ", y = " << y; }
}; 
```

- [A constructor cannot be a virtual function, and a destructor can be a virtual destructor](./interesting_facts5.cpp)
```cpp
// 抽象类
class Base  {
public:
    Base(){ cout << "Constructor: Base" << endl; }
    virtual ~Base(){ cout << "Destructor : Base" << endl; }
    
    virtual void func() = 0;
};

class Derived: public Base {
public:
    Derived(){ cout << "Constructor: Derived" << endl; }
    ~Derived(){ cout << "Destructor : Derived" << endl;}
    
    void func(){cout << "In Derived.func()." << endl;}
};
```
>When the base class pointer points to a derived class object and removes the object, we may want to call the appropriate destructor.
>The destructor can only be called if it is not a virtual destructor.

## 4.Complete example

Abstract classes are inherited and implemented by derived classes!

> Code Example：[derived_full.cpp](./derived_full.cpp)
