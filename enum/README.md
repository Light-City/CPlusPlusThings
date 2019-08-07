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

更“有效”的办法是用一个类或结构体来限定其作用域，例如：定义新变量的方法和上面命名空间的相同。不过这样就不用担心类在别处被修改内容。这里用结构体而非类，一是因为本身希望这些常量可以公开访问，二是因为它只包含数据没有成员函数。

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
