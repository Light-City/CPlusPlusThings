# Friend and Friend Class

## About Author：


![](../img/wechat.jpg)

## 0.Summary

Friends provide a mechanism for ordinary functions or class member functions to access private or protected members in another class.In other words, there are two forms of friends:

（1）Friend Function：Ordinary functions access a private or protected member of a class.

（2）Friend Class：Member functions in class a access private or protected members in class B

Advantages: improve the efficiency of the program.

缺点：破坏了类的封装性和数据的透明性。

总结：
- 能访问私有成员
- 破坏封装性
- 友元关系不可传递
- 友元关系的单向性
- 友元声明的形式及数量不受限制

## 1.友元函数

在类声明的任何区域中声明，而定义则在类的外部。

```
friend <类型><友元函数名>(<参数表>);
```

注意，友元函数只是一个普通函数，并不是该类的类成员函数，它可以在任何地方调用，友元函数中通过对象名来访问该类的私有或保护成员。

具体代码见：[friend_func.cpp](friend_func.cpp)

```c++
#include <iostream>

using namespace std;

class A
{
public:
    A(int _a):a(_a){};
    friend int geta(A &ca);  ///< 友元函数
private:
    int a;
};

int geta(A &ca) 
{
    return ca.a;
}

int main()
{
    A a(3);    
    cout<<geta(a)<<endl;

    return 0;
}
```

## 2.Friend Class
The declaration of a friend class is in the declaration of the class, and the implementation is outside the class.

```
friend class <friend class name>;
```

Class B is a friend of class A, so class B can directly access private members of A.
Code：[friend_class.cpp](friend_class.cpp)
```c++
#include <iostream>

using namespace std;

class A
{
public:
    A(int _a):a(_a){};
    friend class B;
private:
    int a;
};

class B
{
public:
    int getb(A ca) {
        return  ca.a; 
    };
};

int main() 
{
    A a(3);
    B b;
    cout<<b.getb(a)<<endl;
    return 0;
}
```


## 3.Attention
- Friendship has no inheritance
If class B is a friend of class A and class C inherits from Class A, then friend class B cannot directly access private or protected members of class C.
- Friendship is not transitive
If class B is a friend of class A and class C is a friend of class B, then friend class C cannot directly access private or protected members of class A, that is, there is no such relationship as "friend of friend".
