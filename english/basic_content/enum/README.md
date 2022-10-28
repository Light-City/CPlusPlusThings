# From beginner to Master

## About author：


![](../img/wechat.jpg)


## Traditional


Enum has the following problems:

- The scope is not limited, and it is easy to cause naming conflicts. For example：

```c++
#include <iostream>
using namespace std;

enum Color {RED,BLUE};
enum Feeling {EXCITED,BLUE};

int main() 
{
    return 0;
}
```
- Implicit conversion to int

- The actual type used to represent an enumerated variable cannot be explicitly specified, Therefore, forward declaration of enumeration types cannot be supported.
Implementation：[tradition_color.cpp](tradition_color.cpp)

## Classic Method

A simple way to solve the problem of naming conflicts caused by unlimited scope is to prefix the enumeration variables. Change the above example to COLOR_BLUE and FEELING_BLUE。

Generally speaking, we usually prefix all constants for uniformity.But the code for defining enumeration variables is cumbersome.This may have to be done in the C program. But C++ coder do not like this method。Alternatives is namespace:
```c++
namespace Color 
{
    enum Type
    {
        RED=15,
        YELLOW,
        BLUE
    };
};
```

Then you can use `Color::Type c = Color::RED;` to define the new enumeration。If after`using namespace Color` ，the prefix can also be omitted to simplify the code.However, the scope closure provided by a namespace is not high because it can be subsequently extended.In large projects, it is still possible for different people to give different things the same enumeration type names.

A more "effective" approach is to limit its scope with a class or struct.For example：The new variable is defined in the same way as in the namespace above. This way, you don't have to worry about the class being modified elsewhere.We use structs instead of classes because we want these constants to be publicly accessible.

```c++
struct Color1
{
    enum Type
    {
        RED=102,
        YELLOW,
        BLUE
    };
};
```

Implementation：[classic_practice.cpp](classic_practice.cpp)

## C++11 Enum class

The above approach solves the first problem, but it can not do anything for the latter two.Fortunately，C ++ 11 standard introduces enum class. It can solve the above problems.

- The scope of the new enum is no longer global

- Cannot be implicitly converted to another type


```c++
/**
 * @brief C++11 enum class
 * Equals to enum class Color2:int
 */
enum class Color2
{
    RED=2,
    YELLOW,
    BLUE
};
Color2 c2 = Color2::RED;
cout << static_cast<int>(c2) << endl; //！
```

- You can specify a specific type to store enum

```c++
enum class Color3:char;  // Forward statement

// Definition
enum class Color3:char 
{
    RED='r',
    BLUE
};
char c3 = static_cast<char>(Color3::RED);
```

Implementation：[classic_practice.cpp](classic_practice.cpp)

## Enum types in class



Sometimes we want certain constants to work only in classes. Because the macro constant defined by a is global, it can not achieve the purpose, so we want to use const to modify data members.The const data member does exist, but its meaning is not what we expected. 
Data members are constants only for the lifetime of an objec. However, it is variable for the whole class, because the class can create multiple objects, and the values of const data members of different objects can be different.



Cannot be initialized in a class declaration const data memeber。The following usage is incorrect，Because the compiler does not know what the value of size is when the object of the class is not created.(c++11)

```c++
class A 
{
  const int SIZE = 100;   // Error，Attempt to initialize const data member in class declaration 
  int array[SIZE];  // Error，Unknown size 
}; 
```

This should be done in the initialization list of the class's constructor:

```c++
class A 
{
  A(int size);  // Constructor 
  const int SIZE ;
}; 
A::A(int size) : SIZE(size)  // The definition of Struct
{ 

} 
A  a(100); // The size of Object A is 100 
A  b(200); // The size of Object B is 200 
```

How can I establish constants that are constant throughout a class?

It should be implemented with enumeration constants in the class. Such as:

```c++
class Person{
public:
    typedef enum {
        BOY = 0,
        GIRL
    }SexType;
};
//Access via Person::BOY or Person::GIRL.
```

Enum constants do not take up the storage space of the object . They are all evaluated at compile time


Drawback of Enum：Its implied data type is an integer, the maximum is limited, and it cannot represent floating point.
