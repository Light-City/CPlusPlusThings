# Friend and Friend Class

## About Author：


![](../img/wechat.jpg)

## 0.Summary

Friends provide a mechanism for ordinary functions or class member functions to access private or protected members in another class.In other words, there are two forms of friends:

（1）Friend Function：Ordinary functions access a private or protected member of a class.

（2）Friend Class：Member functions in class a access private or protected members in class B

Advantages: improve the efficiency of the program.

Disadvantages: it destroys the encapsulation of classes and the transparency of data.
Conclusion：
- Access to private members
- Breaking encapsulation
- Friendship is not transitive
- The unidirectionality of friend relationship
- There are no restrictions on the form and number of friend declarations

## 1.Friend function

It is declared in any region of the class declaration, and the definition is outside the class.

```
friend <type><name>(<Parameter table>);
```

Note that the friend function is only a common function, not a class member function of this class. It can be called anywhere. In the friend function, private or protected members of the class can be accessed through the object name.

Code：[friend_func.cpp](friend_func.cpp)

```c++
#include <iostream>

using namespace std;

class A
{
public:
    A(int _a):a(_a){};
    friend int geta(A &ca);  ///< Friend function
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
