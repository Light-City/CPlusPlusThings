> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-idioms/class-template-over-function-template/) 的文章，由站点同步脚本导出。

# C++ 类模板优于函数模板

https://stackoverflow.com/questions/5688355/partial-specialisation-of-member-function-with-non-type-parameter

最近看到一个很有意思的模版问题，看看大家对函数模版与类模版的基础掌握的如何，对于下面这个例子会出现什么问题？

你一般使用什么方法进行修复？

```cpp
template <typename T, int R>
struct foo {
  foo(const T& v) : value_(v) {}

  void bar() {
    std::cout << "Generic" << std::endl;
    for (int i = 0; i < R; ++i) std::cout << value_ << std::endl;
  }

  T value_;
};

template <>
void foo<float, 3>::bar() {
  std::cout << "Float" << std::endl;
  for (int i = 0; i < 3; ++i) std::cout << value_ << std::endl;
}

template <int R>
void foo<double, R>::bar() {
  std::cout << "Double" << std::endl;
  for (int i = 0; i < R; ++i) std::cout << value_ << std::endl;
}
```