> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-guides/interesting-cpp-code-snippet/) 的文章，由站点同步脚本导出。

# 一段有趣的 C++ 代码

## 0.导语

目标：如何获取参数lambda或者functor的参数个数呢？

本节将从模版 + `::*` + decay + decltype来实现！

## 1.实现

最终效果：

```cpp
auto c1 = [](int x, double y) { return x * y; };
std::cout << argument_count<decltype(c1)>::value << std::endl;
MyClass f;
std::cout << argument_count<decltype(f)>::value << std::endl;
```

输出：2

我们思考一下这个问题，需要直到这几个点：

- 如何定位到functor与lambda的位置？
- 如何获取到这个函数签名的参数数量？

对于第一个问题，我们可以使用`::*`便可以指向成员声明，对于functor与lambda都是`&std::decay<F>::type::operator())`。简单来说都是获取operator()这个声明的地址。

这里使用decay来移除cv限定符，以上述MyClass为例，到这里先通过`std::decay<F>::type`获取到MyClass类型，随后获取对应operator()的地址，然后传递给argument_count_impl。

```cpp
template <typename F>
using argument_count = decltype(argument_count_impl(&std::decay<F>::type::operator()));
```

argument_count_impl的实现也比较简单，使用R (F::*)(A...)签名获取到对应的函数，这里使用了可变参数模版，`::*`，sizeof是用来获取参数数量，integral_constant则表示这个编译器常量类型为int，值为sizeof...(A)。

```cpp
template <typename F, typename R, typename... A>
static std::integral_constant<int, sizeof...(A)> argument_count_impl(R (F::*)(A...));
```

基于此，便实现了获取Lambda or Functor参数个数的功能~