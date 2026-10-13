> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-idioms/mixin-idiom/) 的文章，由站点同步脚本导出。

# C++ Mixin 惯用法

大家好，我是光城，今天给大家分享C++那些事里面的一个惯用法：mixin

混合（Mixins）是Lisp中的一个概念。混合是类的一部分，意味着它旨在与其他类或混合组合在一起。
常规独立类（例如Person）与混合的区别在于，混合模拟了一些小功能片段（例如打印或显示），并不用于独立使用。相反，它应该与需要此功能的其他类（例如Person）组合在一起。

因此，混合的目的是允许类似于多重继承的东西。

在C++汇总mixin的基本写法如下：

```cpp
template <class Super>
class Mixin : public Super {
. /* mixin body */
};
```

或者

```cpp
template<typename... Super>
class Mixin : public Super... {
  public:
    Mixin() :  Super...() {}
  // ...
};
```

**注：本节的所有示例代码可从星球获取。**

## 示例1：缩放与旋转

假设我们要对一个长方形/正方形进行缩放、旋转、添加边框等，这些操作都会影响其宽度与高度，我们可以使用mixin来实现。

首先，我们可以定义好正常形与长方形作为mixin的基类。

```cpp
// 正方形类
class Square : public Shape {
public:
    Square(int sideLength) : sideLength(sideLength) {}
    int GetWidth() const override { return sideLength; }
    int GetHeight() const override { return sideLength; }
private:
    int sideLength;
};

// 长方形类
class Rectangle : public Shape {
public:
    Rectangle(int width, int height) : width(width), height(height) {}
    int GetWidth() const override { return width; }
    int GetHeight() const override { return height; }
private:
    int width;
    int height;
};
```

随后使用mixin子类去拓展功能，例如：一个缩放的mixin我们可以自定义一个类，它的宽度与高度等于scale乘以对应的宽高。

```cpp
// 缩放的Mixin
template <class Base, int ScaleX, int ScaleY>
class Scale : public Base {
 public:
  int GetScaledWidth() const { return Base::GetWidth() * ScaleX; }
  int GetScaledHeight() const { return Base::GetHeight() * ScaleY; }
};

```

像这种场景非常适合使用mixin。

使用的时候：

```cpp
using ModifiedShape = Border<RotateBy90<Scale<Square, 2, 1>>, 10>;
```

## 示例2：redo与undo

完整示例模拟一个redo与undo操作，我们便可以在数据之间来回的切换。`Redoable<Undoable<Number>> `，在这里我们可以将这些类混合到一起使用了。

```cpp
#include <iostream>

using namespace std;

struct Number {
    typedef int value_type;
    int n;
    void set(int v) {
        n = v;
    }
    int get() const {
        return n;
    }
};

template <typename BASE, typename T = typename BASE::value_type>
struct Undoable : public BASE {
    typedef T value_type;
    T before;
    void set(T v) {
        before = BASE::get();
        BASE::set(v);
    }
    void undo() {
        BASE::set(before);
    }
};

template <typename BASE, typename T = typename BASE::value_type>
struct Redoable : public BASE {
    typedef T value_type;
    T after;
    void set(T v) {
        after = v;
        BASE::set(v);
    }
    void redo() {
        BASE::set(after);
    }
};

typedef Redoable<Undoable<Number>> ReUndoableNumber;

int main() {
    ReUndoableNumber mynum;
    mynum.set(42);
    mynum.set(84);
    cout << mynum.get() << '\n';  // Output: 84
    mynum.undo();
    cout << mynum.get() << '\n';  // Output: 42
    mynum.redo();
    cout << mynum.get() << '\n';  // Output: back to 84
}
```

## 示例3：重复打印

再举一个拼积木的例子：重复打印。

首先，有一个类，我们可以print这个人的名字。

```cpp
class Name
{
public:
    Name(std::string firstName, std::string lastName)
      : firstName_(std::move(firstName))
      , lastName_(std::move(lastName)) {}
    
    void print() const
    {
        std::cout << lastName_ << ", " << firstName_ << '\n';
    }
    
private:
    std::string firstName_;
    std::string lastName_;
};
```

那如果我想重复打印呢？

我们可以这样玩，使用mixin，将Name类传递进去。

```cpp
template<typename Printable>
struct RepeatPrint : Printable
{
    explicit RepeatPrint(Printable const& printable) : Printable(printable) {}
    void repeat(unsigned int n) const
    {
        while (n-- > 0)
        {
            this->print();
        }
    }
};

template<typename Printable>
RepeatPrint<Printable> repeatPrint(Printable const& printable)
{
    return RepeatPrint<Printable>(printable);
}
```

使用：

```
Name ned("Eddard", "Stark");    
repeatPrint(ned).repeat(10);
```

此时，便得到重复n次的打印结果。

那么mixin比较适合什么场景呢？

- 保持原来的类不变，
- 客户端代码不直接使用原始类，它需要将其包装到 mixin 中才能使用增强功能，

## 标准库

在标准库当中有一个使用mixin技术：std::nested_exception。

std::nested_exception 是一个多态 mixin 类，它可以捕获并存储当前异常，从而可以在彼此之间嵌套任意类型的异常。

```cpp
template <typename _Except>
struct _Nested_exception : public _Except, public nested_exception
{}
```

在源码中我们可以看到使用了nested_exception作为mixin的基类。

## 参考

> https://cedanet.com.au/ceda/Xc++/$mixin/mixins-in-straight-c++.php
>
> https://www.fluentcpp.com/2017/12/12/mixin-classes-yang-crtp/
>
> https://stackoverflow.com/questions/18773367/what-are-mixins-as-a-concept