> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-things/member-initializer-list-notes/) 的文章，由站点同步脚本导出。

# C++ 成员初始化列表注意点

在C++中，类的正确初始化是确保程序正常运行的关键步骤之一。尤其是在使用初始化列表时，我们需要注意一些潜在的陷阱，以避免在代码执行过程中产生意外的结果。

初始化列表通常被认为是设置新类实例状态的首选方法，因为它提供了一种简洁、高效的方式来初始化类成员。然而，有时候在初始化列表中引用其他成员变量可能导致一些令人困惑的问题。

## 问题示例

考虑以下的`Foo`类：

```cpp
#include <iostream>

class Foo
{
public:
  Foo(int size) : size_(size),
                 capacity_(size_ + 2),
                 length_(capacity_)
  {}

  void print() {
    std::cout << "Size: " << size_ << " Capacity:  " << capacity_ << " Length: " << length_  << std::endl;
  }

private:
  int length_;
  int capacity_;
  int size_;
};

int main() {
  Foo foo(50);
  foo.print();
}
```

在这个例子中，我们试图通过初始化列表中引用其他成员变量的方式来初始化`length_`、`capacity_`和`size_`。然而，由于类成员的初始化顺序是按照它们在类中声明的顺序进行的，可能导致`length_`与`capacity_`并没有按照我们期望的方式初始化。

## 如何解决?

为了避免这种问题，应该遵循以下几点：

**1.避免在初始化列表中引用其他成员变量**

**2.顺序一致性：成员变量的初始化顺序是按照它们在类中声明的顺序**

修正示例：

```cpp
#include <iostream>

class Foo
{
public:
  Foo(int size) : size_(size),
                 capacity_(size_ + 2),
                 length_(calculateLength(size_))
  {}

  void print() {
    std::cout << "Size: " << size_ << " Capacity:  " << capacity_ << " Length: " << length_  << std::endl;
  }

private:
  int calculateLength(int size) {
    return size + 2;
  }

private:
  int size_;
  int capacity_;
  int length_;
};

int main() {
  Foo foo(50);
  foo.print();
}
```