> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-things/cpp-vs-rust-destructor/) 的文章，由站点同步脚本导出。

# C++ 与 Rust 析构对比

在Rust中mem::forget用于防止对象的析构，跳过清理工作，从而让资源的释放交给其他机制管理。

例如：

```cpp
let file = File::open("foo.txt").unwrap();
mem::forget(file);
```

那么在C++中如何防止析构？即跳过析构函数呢？

**本节将给出三种办法，完整代码示例也放于星球。**

## 1. `placement new`

`placement new`可以让我们手动管理对象的构造和析构。如果不手动调用对象的析构函数，我们就可以跳过对象的析构，从而实现类似`mem::forget`的效果。

例如：

```cpp
alignas(Foo) std::byte storage[sizeof(Foo)];  // 分配对齐的原始内存块
Foo* pfoo = new (storage) Foo();  // 在这块内存上构造Foo对象
```

在上面的代码中，`placement new`创建了对象`pfoo`，但我们不调用析构函数，这样可以跳过对象的清理逻辑。

## 2. 使用智能指针的自定义删除器

另一种实现类似功能的方法是通过自定义智能指针的删除器，防止对象被销毁。我们可以定义一个不执行任何操作的删除器，从而避免析构函数的调用。

```cpp
std::unique_ptr<Foo, NoOpDeleter> pfoo(new Foo(), NoOpDeleter{});
```

使用自定义的删除器`NoOpDeleter`，我们可以防止`std::unique_ptr`在离开作用域时调用析构函数，从而实现类似`mem::forget`的效果。

## 3.union

union的方案很神奇，大家知道原理吗？欢迎留言区一起讨论～

```cpp
template <class T>
union Wrapper {
  T value;
  ~Wrapper() {}
};

struct Foo {
  ~Foo() { std::cout << "dtor foo\n"; }
};

Wrapper<Foo> f; 
// f.value.~Foo);  必须手动调用
```