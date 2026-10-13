> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-things/cpp-multiple-return-values/) 的文章，由站点同步脚本导出。

# C++ 多返回值写法

[TOC]

今天再提出一个比较经典的面试题：函数如何返回多个结果？你能想到几种办法？

其实这道题目考察的知识点非常的多，涉及到：

- 指针
- 引用
- lambda
- functional
- tie
- tuple
- struct bingding
- template deduce
- 等等

本篇文章将会给大家用不同的方法来解答这道题目，首先我们转化一下这个题目，使用`a / b`来模拟(下面不考虑被除数为0的场景)，返回商与余数。简单来说转化为一个函数返回商与余数有多少种办法？

下面进入正文，注：本节的所有代码已首发于星球，感兴趣戳下方二维码即可。

## 1. 指针与引用

对接口进行更改，添加两个变量，使用指针与引用。

```cpp
void divideWithReferences(int dividend, int divisor, int& quotient, int& remainder) {
  quotient = dividend / divisor;
  remainder = dividend % divisor;
}

void divideWithPointers(int dividend, int divisor, int* quotient, int* remainder) {
  if (quotient) *quotient = dividend / divisor;
  if (remainder) *remainder = dividend % divisor;
}

```

使用这种方式的缺点也明显可见，当返回值不止两个，接口就非常的长。此外，如果面试官要求你不允许更改函数接口，那么这个办法用不了了。

## 2. Tuple + Tie

Tuple和tie的组合为函数返回多个值提供了一种简洁的方式。通过使用`std::tie`，我们可以将tuple中的元素解包到不同的变量中，提高代码的清晰度。

这种方法算是标准答案之一，比较简单清晰，如果返回值是两个pair也可以。

```cpp
std::tuple<int, int> divide(int dividend, int divisor) {
	return std::make_tuple(dividend / divisor, dividend % divisor);
}


std::tie(quotient, remainder) = divide(14, 3);
std::cout << quotient << ", " << remainder << std::endl;
```

## 3. Struct Binding

对上面进行改造，C++17引入的结构化绑定进一步简化了多值返回的代码。使用auto和结构化绑定，使得代码更加直观易懂。

如果面试官要你进一步的对上面的方法进行改造，那么可以变为下面这个代码示例，使用结构化绑定使代码更加优雅。

```cpp
auto divide(int dividend, int divisor) {
  struct result {
    int quotient;
    int remainder;
  };
  return result{dividend / divisor, dividend % divisor};
}
auto [quotient, remainder] = divide(14, 3);
```

## 4. 函数 Callback

在日常项目中，特别是一些开源项目，callback是一种常用的手段，通过传递处理返回值的callback，让用户自定义处理，这样便实现了返回多个值，实现更加灵活的代码结构。这对于异步编程和事件处理等场景非常有用。

```cpp
void divide(int dividend, int divisor, std::function<void(int, int)> callback) {
  callback(dividend / divisor, dividend % divisor);
}
```

## 5. 自定义 Out

自定义out是一种通过结构体包装的方式，将输出参数作为结构体的成员。这种方式提高了代码的可读性，尤其适用于需要返回多个值的函数。

```cpp
template <class T>
struct out {
  std::function<void(T)> target;

  out(T* t)
      : target([t](T&& in) {
          if (t) *t = std::move(in);
        }) {}

  template <class... Args>
  void emplace(Args&&... args) {
    target(T(std::forward<Args>(args)...));
  }

  template <class X>
  void operator=(X&& x) {
    emplace(std::forward<X>(x));
  }

  template <class... Args>
  void operator()(Args&&... args) {
    emplace(std::forward<Args>(args)...);
  }
};

void divide(int dividend, int divisor, out<int>& quotient_out, out<int>& remainder_out) {
  quotient_out.emplace(dividend / divisor);
  remainder_out.emplace(dividend % divisor);
}
```

## 6. 模版推导

C++的模版推导为开发者提供了更为灵活和简洁的代码编写方式。通过模版推导，我们可以处理不同类型的数据而无需显式指定类型。

```cpp
template <typename T1, typename T2>
struct many {
  T1 quotient;
  T2 remainder;
};

template <class T1, class T2>
many(T1, T2) -> many<T1, T2>;

many<int, int> divide(int dividend, int divisor) {
  return many{
      dividend / divisor,
      dividend % divisor,
  };
}

auto [quotient, remainder] = divide(14, 3);
```

总体而言，C++提供了多种方式来实现多值返回，每种方式都有其适用的场景。选择合适的方式取决于具体的需求，开发者可以根据代码结构和可读性来灵活使用这些特性。