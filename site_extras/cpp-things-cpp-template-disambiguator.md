> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-things/cpp-template-disambiguator/) 的文章，由站点同步脚本导出。

# C++ 模板消歧

## 1.背景

最近看到一段代码：

```cpp
 auto chunk_left = first_sort_key.template GetChunk<ArrayType>(left);
```

请问，这里的`.template`代表什么意义？

本节将从实际例子出发，探讨这个意义。

## 2.template disambiguator

在C++中，当使用模板的依赖名称（dependent names）时，有时需要使用模板消除符（template disambiguator）来帮助编译器区分这些名称。所谓"dependent names"是指依赖于模板参数的类型或值，编译器不能在实例化模板之前确定它们的具体含义。这可能会导致编译器在解析这些名称时产生二义性。

模板消除符(template disambiguator)使用关键字"template"来明确告诉编译器一个特定的名称是一个模板。这样编译器就可以正确地解析该名称，而不会产生二义性。

关键字template只能在操作符::(作用域解析)、->(通过指针访问成员)和.(成员访问)，以下都是有效的示例

- T::template foo<X>();
- s.template foo<X>();
- this->template foo<X>();
- typename T::template iterator<int>::value_type v;

官方资料：

> https://en.cppreference.com/w/cpp/language/dependent_name

假设有下面这个代码，我们如果不加.template会得到什么？

```cpp
template <typename T>
auto foo_is_not_a_template() {
  // ((T::foo) < 34) > (::val())
  return T::foo<34>::val();
}
```

上述会被解析为：((T::foo) < 34) > (::val())

对应的.template例子为：

```cpp
template <typename T>
auto foo_is_a_template() {
  return T::template foo<34>::val();
}
```

这样便调用会正确解析为模版，foo<34>。

本节完整代码将会在星球公布，感兴趣加入即可，谢谢！