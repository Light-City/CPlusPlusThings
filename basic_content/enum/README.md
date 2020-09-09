# 从初级到高级的enum那些事

## 关于作者：

个人公众号：

![](../img/wechat.jpg)


## 传统行为


枚举有如下问题：

- 作用域不受限,会容易引起命名冲突。例如下面无法编译通过的：

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
- 会隐式转换为int
- 用来表征枚举变量的实际类型不能明确指定，从而无法支持枚举类型的前向声明。

具体实现见：[tradition_color.cpp](tradition_color.cpp)

## 经典做法

解决作用域不受限带来的命名冲突问题的一个简单方法是，给枚举变量命名时加前缀，如上面例子改成 COLOR_BLUE 以及 FEELING_BLUE。

一般说来，为了一致性我们会把所有常量统一加上前缀。但是这样定义枚举变量的代码就显得累赘。C 程序中可能不得不这样做。不过 C++ 程序员恐怕都不喜欢这种方法。替代方案是命名空间:
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

这样之后就可以用 `Color::Type c = Color::RED;` 来定义新的枚举变量了。如果 `using namespace Color` 后，前缀还可以省去，使得代码简化。不过，因为命名空间是可以随后被扩充内容的，所以它提供的作用域封闭性不高。在大项目中，还是有可能不同人给不同的东西起同样的枚举类型名。

更“有效”的办法是用一个类或结构体来限定其作用域，例如：定义新变量的方法和上面命名空间的相同。不过这样就不用担心类在别处被修改内容。这里用结构体而非类，是因为本身希望这些常量可以公开访问。

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

具体实现见：[classic_practice.cpp](classic_practice.cpp)

## C++11 的枚举类

上面的做法解决了第一个问题，但对于后两个仍无能为力。庆幸的是，C++11 标准中引入了“枚举类”(enum class)，可以较好地解决上述问题。

- 新的enum的作用域不在是全局的
- 不能隐式转换成其他类型

```c++
/**
 * @brief C++11的枚举类
 * 下面等价于enum class Color2:int
 */
enum class Color2
{
    RED=2,
    YELLOW,
    BLUE
};
r2 c2 = Color2::RED;
cout << static_cast<int>(c2) << endl; //必须转！
```

- 可以指定用特定的类型来存储enum

```c++
enum class Color3:char;  // 前向声明

// 定义
enum class Color3:char 
{
    RED='r',
    BLUE
};
char c3 = static_cast<char>(Color3::RED);
```

具体实现见：[classic_practice.cpp](classic_practice.cpp)

## 类中的枚举类型

有时我们希望某些常量只在类中有效。 由于#define 定义的宏常量是全局的，不能达到目的，于是想到实用const 修饰数据成员来实现。而const 数据成员的确是存在的，但其含义却不是我们所期望的。

const 数据成员只在某个对象生存期内是常量，而对于整个类而言却是可变的，因为类可以创建多个对象，不同的对象其 const 数据成员的值可以不同。 

不能在类声明中初始化 const 数据成员。以下用法是错误的，因为类的对象未被创建时，编译器不知道 SIZE 的值是什么。(c++11标准前)

```c++
class A 
{
  const int SIZE = 100;   // 错误，企图在类声明中初始化 const 数据成员 
  int array[SIZE];  // 错误，未知的 SIZE 
}; 
```

正确应该在类的构造函数的初始化列表中进行：

```c++
class A 
{
  A(int size);  // 构造函数 
  const int SIZE ;    
}; 
A::A(int size) : SIZE(size)  // 构造函数的定义
{ 

} 
A  a(100); // 对象 a 的 SIZE 值为 100 
A  b(200); // 对象 b 的 SIZE 值为 200 
```

怎样才能建立在整个类中都恒定的常量呢？

别指望 const 数据成员了，应该用类中的枚举常量来实现。例如:

```c++
class Person{
public:
    typedef enum {
        BOY = 0,
        GIRL
    }SexType;
};
//访问的时候通过，Person::BOY或者Person::GIRL来进行访问。
```

枚举常量不会占用对象的存储空间，它们在编译时被全部求值。

枚举常量的缺点是：它的隐含数据类型是整数，其最大值有限，且不能表示浮点。
