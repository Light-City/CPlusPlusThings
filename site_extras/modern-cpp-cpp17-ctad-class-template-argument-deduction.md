> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp17/ctad-class-template-argument-deduction/) 的文章，由站点同步脚本导出。

# C++17 类模板参数推导 CTAD

## **引入**

C++模板元编程一直是C++开发者们熟知的一项功能，无论是初学者还是高级开发者都能够通过它编写具有类型参数的通用代码。随着C++17引入的类模板参数推导（CTAD），处理类模板的过程变得更加直观和灵活。

本节所有代码也会更新于社群中。

C++17引入CTAD会带来如下好处：

例如：这么复杂的代码

```
std::vector<FooBar<int, const char*>> obj{a, b, c}; 
```

我们只需要：

```
 std::vector obj{a, b, c};
```

再比如：

```
std::tuple<int>
```

可以简化为：

```
std::tuple t1{1};
```

所以，咱们今天来看看CTAD。

CTAD全称是Class template argument deduction (CTAD)，类模版参数推导，你给定编译器一个推导指南(deduction guide)，我们便可以使用这个特性了。

如果不给定，如下面例子：

```
#include <iostream>
template <typename T, typename U>
struct MyPair {
  T first{};
  U second{};
};

int main() {
  MyPair<int, int> p1{1, 2};
  MyPair p2{1, 2};
  std::cout << p1.first << ", " << p1.second << std::endl;
  std::cout << p2.first << ", " << p2.second << std::endl;
  return 0;
}
```

对于p2，我们便会报错：

no viable constructor or deduction guide for deduction of template arguments of 'MyPair'

那么对于怎么修改呢？

只需要添加deduction guid即可，如下写法即可。

```
template <typename T, typename U>
MyPair(T, U) -> MyPair<T, U>;
```

类模板参数推导（CTAD）通过允许编译器从构造函数参数中推导出模板参数，简化了类模板的实例化过程。在引入CTAD之前，开发者在实例化时必须明确指定模板参数。然而，通过CTAD，这种明确的指定变得不再必要，从而使代码更易读、易维护。

```
template <typename T>
class Add{
    private:
        T first;
        T second;
    public:
        Add() = default;
        Add(T first, T second): first_{first}, second_{second} {}
        T result() const { return first + second; }
};

int main(){
    Add one(1,2);               // T被推导为int
    Add two{1.245, 3.1415};     // T被推导为double
    Add three = {0.24f, 0.34f}; // T被推导为float
}
```

在上面的示例中，CTAD允许类型`T`基于构造函数参数被推导，消除了显式类型指定的需求。

## **非静态成员初始化**

但是有个场景用不了CTAD，那就是非静态成员初始化。例如：Test类中使用外面的MyPair就必须写全。

```
template <typename T = int, typename U = int>
struct MyPair {
  T first{};
  U second{};
};

template <typename T, typename U>
MyPair(T, U) -> MyPair<T, U>;

class Test {
  MyPair p4;   // use of class template 'MyPair' requires template arguments; argument
               // deduction not allowed in non-static class member
};
```

在上面的例子中，我们定义了一个模板类`MyPair`，并在其中使用了CTAD。然而，当我们尝试在类`Test`中使用`MyPair`作为非静态成员时，由于非静态成员初始化的上下文，CTAD将无法正常工作，导致编译错误。在这种情况下，我们必须显式指定`MyPair`的模板参数。

我们可以看到在非静态成员初始化的上下文中，CTAD无法推导模板参数。因此，在类模板作为非静态成员时，开发者需要显式指定所有的模板参数。这一点需要在实际开发中特别注意，以确保代码的正确性和可维护性。