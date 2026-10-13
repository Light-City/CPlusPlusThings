> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp20/cpp20-concepts/) 的文章，由站点同步脚本导出。

# C++20 Concepts 概念

[TOC]

C++20引入了concept(概念)，是对模板参数(编译时评估)的一组约束。你可以将它们用于类模板和函数模板来控制函数重载和特化。一些优点包括：

- 对模版参数强制类型约束
- 提高代码可读性(替换了较长的SFINAE代码)
- 提供更友好的报错信息
- 通过限制可以使用的类型来防止意外的模板实例化

以一个比较简单加法为例。

```cpp
#include <iostream>

struct Foo {};

template <typename T>
T Add(T a, T b) {
  return a + b;
}

int main() {
  std::cout << Add(1, 2) << std::endl;
  Foo f1, f2;
  std::cout << Add(f1, f2) << std::endl;
  return 0;
}
```

对于Foo来说，是不支持加法的，于此同时也是不可以直接`std::cout << `，因此在编译时报一大堆错误，包含`operator<<`与`operator+`，但这并不是我们期望的错误信息，我们比较期望的是编译器给我们最直观的错误信息，即：这个结构体能不能相加。

```
add.cc: In function 'int main()':
add.cc:13:13: error: no match for 'operator<<' (operand types are 'std::ostream' {aka 'std::basic_ostream<char>'} and 'Foo')
   13 |   std::cout << Add(f1, f2) << std::endl;
      |   ~~~~~~~~~ ^~ ~~~~~~~~~~~
      |        |          |
      |        |          Foo
      |        std::ostream {aka std::basic_ostream<char>}
In file included from /usr/local/Cellar/gcc/13.2.0/include/c++/13/iostream:41,
                 from add.cc:1:
/usr/local/Cellar/gcc/13.2.0/include/c++/13/ostream:110:7: note: candidate: 'std::basic_ostream<_CharT, _Traits>::__ostream_type& std::basic_ostream<_CharT, _Traits>::operator<<(__ostream_type& (*)(__ostream_type&)) [with _CharT = char; _Traits = std::char_traits<char>; __ostream_type = std::basic_ostream<char>]'
  110 |       operator<<(__ostream_type& (*__pf)(__ostream_type&))
  
/usr/local/Cellar/gcc/13.2.0/include/c++/13/ostream:801:5: note:   template argument deduction/substitution failed:
/usr/local/Cellar/gcc/13.2.0/include/c++/13/ostream: In substitution of 'template<class _Ostream, class _Tp> _Ostream&& std::operator<<(_Ostream&&, const _Tp&) [with _Ostream = std::basic_ostream<char>&; _Tp = Foo]':
add.cc:13:26:   required from here
/usr/local/Cellar/gcc/13.2.0/include/c++/13/ostream:801:5: error: no type named 'type' in 'struct std::enable_if<false, void>'
add.cc: In instantiation of 'T Add(T, T) [with T = Foo]':
add.cc:13:19:   required from here
add.cc:7:12: error: no match for 'operator+' (operand types are 'Foo' and 'Foo')
    7 |   return a + b;
      |          ~~^~~
```

当我们使用concept实现之后：

```cpp
template <typename T>
concept Addable = requires(T a, T b) { a + b; };

template <typename T>
  requires Addable<T>
T Add(T a, T b) {
  return a + b;
}
```

便可以得到我们关心的编译错误：

```cpp
add_concept.cc: In function 'int main()':
add_concept.cc:17:19: error: no matching function for call to 'Add(Foo&, Foo&)'
   17 |   std::cout << Add(f1, f2) << std::endl;
      |                ~~~^~~~~~~~
add_concept.cc:10:3: note: candidate: 'template<class T>  requires  Addable<T> T Add(T, T)'
   10 | T Add(T a, T b) {
      |   ^~~
add_concept.cc:10:3: note:   template argument deduction/substitution failed:
add_concept.cc:10:3: note: constraints not satisfied
add_concept.cc: In substitution of 'template<class T>  requires  Addable<T> T Add(T, T) [with T = Foo]':
add_concept.cc:17:19:   required from here
add_concept.cc:6:9:   required for the satisfaction of 'Addable<T>' [with T = Foo]
add_concept.cc:6:19:   in requirements with 'T a', 'T b' [with T = Foo]
add_concept.cc:6:42: note: the required expression '(a + b)' is invalid
    6 | concept Addable = requires(T a, T b) { a + b; };
      |                                        ~~^~~
```

下面，我们来针对上面这个例子深入学习concept语法。

## 1.concept语法

语法：

```
template <template-parameter-list>
concept concept-name = constraint-expression;
```

我们来对比一下实际的例子，Addable是concept-name，constraint-expression是`requires(T a, T b) { a + b; }`。

```
template <typename T>
concept Addable = requires(T a, T b) { a + b; };
```

使用方式为：

```
#include <concepts>
```

这个concept可以放在多个地方，如下：

- typename的位置
- requires后面
- auto前面

### 1.1 替换typename

约束模版参数，替换typename。

```cpp
// template <typename T>  typename->Addable
template <Addable T>
T Add(T a, T b) {
  return a + b;
}
```

### 1.2 requires关键字

我们在函数模板中使用 requires 关键字。 它可以访问我们的模板T是否是可以相加的，如果模板可以处理相加，它将返回 true。

requires可以放在模版中，也可以放在函数之后，但是不可以放在类之后。于此同时它有两个写法：

- requires 条件

例如：

```cpp
template <typename T>
  requires Addable<T>
T Add(T a, T b) { ... }
```

- requires 表达式

例如：

```cpp
template <typename T>
concept Addable = requires(T a, T b) { a + b; };
```

#### 1.2.1 放在模版中

函数：

```cpp
template <typename T>
  requires Addable<T>
T Add(T a, T b) {
  return a + b;
}
```

类：

```cpp
template <typename T>
	requires Addable<T> 
class Bar {
 public:
  T Add(T a, T b) { return a + b; }
};
```

#### 1.2.2 函数尾随 Requires 子句

函数：

```cpp
template <typename T>
T Add(T a, T b)
  requires Addable<T>
{
  return a + b;
}
```

对于类则不支持这种写法，会报错：error: expected unqualified-id before 'requires'  28 |   requires Addable<T>

```cpp
template <typename T>
class Bar requires Addable<T> 
{
 public:
  T Add(T a, T b) { return a + b; }
};
```

#### 1.2.3 requires基本示例

以数据库当中的类型为例，数据库中有不同类型，我们将其划分为：null、binary、number等，我们想要对传递的类型执行打印操作，于是有了下面的示例：

```cpp
#include <concepts>

class NumberType {};
class BaseBinaryType {};
class NullType {};
class FloatingPointType : public NumberType {};
class IntegerType : public NumberType {};
class BinaryType: public BaseBinaryType {};

template <typename T>
  requires std::is_base_of_v<NumberType, T> || std::is_base_of_v<BaseBinaryType, T>
void PrintValue(T v) {}

int main() {
  PrintValue(FloatingPointType{});
  PrintValue(NullType{});
  return 0;
}
```

对于requires我们可以使用`||`，上面示例中出现了NullType，它不满足`requires`，因此会编译出现：

```cpp
concept_requires.cc:16:13: error: no matching function for call to 'PrintValue(NullType)'
   16 |   PrintValue(NullType{});
      |   ~~~~~~~~~~^~~~~~~~~~~~
concept_requires.cc:12:6: note: candidate: 'template<class T>  requires (is_base_of_v<NumberType, T>) || (is_base_of_v<BaseBinaryType, T>) void PrintValue(T)'
   12 | void PrintValue(T v) {}
      |      ^~~~~~~~~~
concept_requires.cc:12:6: note:   template argument deduction/substitution failed:
concept_requires.cc:12:6: note: constraints not satisfied
concept_requires.cc: In substitution of 'template<class T>  requires (is_base_of_v<NumberType, T>) || (is_base_of_v<BaseBinaryType, T>) void PrintValue(T) [with T = NullType]':
concept_requires.cc:16:13:   required from here
concept_requires.cc:12:6:   required by the constraints of 'template<class T>  requires (is_base_of_v<NumberType, T>) || (is_base_of_v<BaseBinaryType, T>) void PrintValue(T)'
concept_requires.cc:11:45: note: no operand of the disjunction is satisfied
   11 |   requires std::is_base_of_v<NumberType, T> || std::is_base_of_v<BaseBinaryType, T>
      |            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
```

### 1.3 concept与auto

当auto出现时，我们可以将其与concept一起使用，例如：

```cpp
auto add(auto x, auto y) {
  return x + y;
}
```

我们可以变为：

```cpp
template <typename T>
concept Addable = requires(T a, T b) { a + b; };

auto add2(Addable auto x, Addable auto y) {
  return x + y;
}
```

编译时会出现：

```cpp
concept_auto.cc:17:20: error: no matching function for call to 'add2(Foo&, Foo&)'
   17 |   std::cout << add2(f1, f2) << std::endl;
      |                ~~~~^~~~~~~~
concept_auto.cc:10:6: note: candidate: 'template<class auto:18, class auto:19>  requires (Addable<auto:18>) && (Addable<auto:19>) auto add2(auto:18, auto:19)'
   10 | auto add2(Addable auto x, Addable auto y) {
      |      ^~~~
concept_auto.cc:10:6: note:   template argument deduction/substitution failed:
concept_auto.cc:10:6: note: constraints not satisfied
concept_auto.cc: In substitution of 'template<class auto:18, class auto:19>  requires (Addable<auto:18>) && (Addable<auto:19>) auto add2(auto:18, auto:19) [with auto:18 = Foo; auto:19 = Foo]':
concept_auto.cc:17:20:   required from here
concept_auto.cc:8:9:   required for the satisfaction of 'Addable<auto:18>' [with auto:18 = Foo]
concept_auto.cc:8:19:   in requirements with 'T a', 'T b' [with T = Foo]
concept_auto.cc:8:42: note: the required expression '(a + b)' is invalid
    8 | concept Addable = requires(T a, T b) { a + b; };
```

## 2.编译器支持

需要GCC(10.0+)，Clang(10.0+)，编译选项：`-std=c++20`/`-std=c++2a`

> https://en.cppreference.com/w/cpp/compiler_support

## 3.总结

自C++20提供的concept之后，我们不再需要enable_if/SFINAE的机制、函数重载来做一些模版约束检查了，使用concept可以帮你搞定这个操作，它提供了一种更清晰和强大的模板参数约束机制，使得模板代码更易于编写、理解和维护。通过在编译时进行类型检查，它有助于提高代码的稳健性和可读性。