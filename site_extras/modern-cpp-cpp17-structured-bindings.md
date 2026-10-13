> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp17/structured-bindings/) 的文章，由站点同步脚本导出。

# C++17 结构化绑定

在聊结构化绑定之前，有几个面试问题，看看你会不会？

- 如何使用结构化绑定访问自定义类的私有成员？

- 如何使用结构化绑定修改自定义类的成员呢？

这几个题目估计没几个人能答上来，题目与答案网上也都没有，基于这些问题，我们引出本文的内容：如何为自定义类型支持结构化绑定，里面有哪些坑？

**注：懒人版，答案与代码已放星球。**

为了让本文的内容更加丝滑，从示例出发。

## 1.自定义类型

如果给定一个类：

```cpp
struct Foo {
  Foo(int64_t id, std::string data) : id_(id), data_(std::move(data)) {}
  void Print() { std::cout << "id = " << id_ << ", data = " << data_ << std::endl; }

 private:
  int64_t id_;
  std::string data_;
};
```

要让下面这个代码运行起来：

```cpp
int main() {
  Foo f{12, "hello world"};
  const auto& [id, data] = f;
  std::cout << "id = " << id << ", data = " << data << std::endl;
  f.Print();
}
```

如果直接运行会编译：

```cpp
 cannot decompose inaccessible member 'Foo::id_' of 'const Foo'
   42 |   const auto& [id, data] = f;
```

那么怎么做呢？

## 2.支持结构化绑定

这个问题我们使用两个步骤：

1.特化 `std::tuple_size` 和 `std::tuple_element`

```cpp
namespace std {
  template <>
  struct tuple_size<Foo> : std::integral_constant<std::size_t, 2> {};

  template <>
  struct tuple_element<0, Foo> {
    using type = int64_t;
  };

  template <>
  struct tuple_element<1, Foo> {
    using type = std::string;
  };
}
```

2.为上面的结构体添加友元get函数，然后实现即可：

```cpp
template <std::size_t Index>
decltype(auto) get(const Foo& f);

template <>
decltype(auto) get<0>(const Foo& f) {
  return f.id_;
}

template <>
decltype(auto) get<1>(const Foo& f) {
  return f.data_;
}
```

此时便可以正常的工作了，得到输出：

```cpp
id = 12, data = hello world
id = 12, data = hello world
```

最后，抛出几个问题：

- 上面提供的是只读访问，如果在访问的时候修改const auto &会发生什么？

例如：

```cpp
auto& [id, data] = f;
```

- 如何支持对类成员的修改访问？

即，让这个语句能够正常运行

```cpp
auto& [id, data] = f;
```

好了，本节完，欢迎留言区交流。