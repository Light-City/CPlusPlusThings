> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp20/no-unique-address/) 的文章，由站点同步脚本导出。

# C++20 [[no_unique_address]]

## C++那些事之内存优化

通常程序运行时内存是一个比较大的问题，如何减少内存占用和提升访问速度是至关重要。为了解决这些问题，C++20 引入了 `no_unique_address` 特性，并结合空基类优化（EBO, Empty Base Optimization），为开发者提供了一种有效的方式来优化内存布局。

#### 1. 空基类优化（EBO）

EBO 是指对于空基类（即没有数据成员的类）在派生类中的存在不会占用额外的内存。C++ 编译器会将这些空基类的实例合并，避免不必要的内存开销。这在设计复杂的类层次结构时尤其有用，可以有效减少对象的大小。

```cpp
#include <iostream>
class Empty {};

class Derived : public Empty {
  int value;

 public:
  Derived(int v) : value(v) {}
};

class Test {
  int value;

 public:
  Test(int v) : value(v) {}
  Empty e;
};

int main() {
  std::cout << sizeof(Derived) << std::endl;
  std::cout << sizeof(Test) << std::endl;
  return 0;
}
```

在上面的代码中，`Derived` 类在内存中只占用 `int` 的大小，而不是 `int` 加上 `Empty` 的大小。这是因为编译器利用了 EBO 的特性。

**那么你猜猜上面的输出是多少呢？**

#### 2. `no_unique_address` 特性

C++20 引入了 `no_unique_address` 特性，它允许我们显式标记某些类成员，使其在内存中不会占用额外空间。特别适合<0字节对象>，例如：

unique_ptr中当Deleter是空结构体/类，此时得到的sizeof是16，C++20之后便可以不占子节，整个sizeof为8。

```cpp
template<class T, class Deleter>
class unique_ptr {
    T* pointer = nullptr;
    [[no_unique_address]] Deleter deleter;

   public:
    // 其他成员函数...
};
```

> 本节完