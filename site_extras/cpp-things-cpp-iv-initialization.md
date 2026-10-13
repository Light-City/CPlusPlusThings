> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-things/cpp-iv-initialization/) 的文章，由站点同步脚本导出。

# C++ 初始化与赋值

## C++ iv

```cpp
int* p = nullptr;
std::shared_ptr<int> s(p); // 计数器1
std::shared_ptr<int> s2(s); // 计数器2
assert(s.use_count() == 2); 
```

这段代码会引发几个疑问：

- 为何nullptr原始指针构造出来的智能指针计数器不是0？
- delete nullptr是否是安全的？

### 1.空的shared_ptr语义

我们直接看shared_ptr源码，所谓**空**指的是：

- 从默认构造一个shared_ptr
- 从nullptr_t构造一个shared_ptr

这两种情况才是空，而对于从原始指针构造的shared_ptr，它占有那个指针的所有权，不管它是否为空。

分别对应下面三个构造。

```cpp
private:
    element_type*      __ptr_;
    __shared_weak_count* __cntrl_;

public:
    _LIBCPP_HIDE_FROM_ABI
    _LIBCPP_CONSTEXPR shared_ptr() _NOEXCEPT
        : __ptr_(nullptr),
          __cntrl_(nullptr)
    { }

    _LIBCPP_HIDE_FROM_ABI
    _LIBCPP_CONSTEXPR shared_ptr(nullptr_t) _NOEXCEPT
        : __ptr_(nullptr),
          __cntrl_(nullptr)
    { }
    shared_ptr(_Yp* __p, _Dp __d)
        : __ptr_(__p)
    {}
```

在上面原始指针式nullptr情况下，删除delete nullptr会发生什么？删除两次又回发生什么？

### 2.delete nullptr

假设有下面的代码，运行之后会crush？会正常运行？

```cpp
#include <iostream>

int main() {
    int *p = nullptr;
    delete p;
    std::cout << "delete once" << std::endl;
    delete p;
    std::cout << "delete twice" << std::endl;

}
```

答案是：正常输出。

具体原因参考cppreference:

If *expression* evaluates to a null pointer value, no destructors are called, and the deallocation function may or may not be called (it's unspecified), but the default deallocation functions are guaranteed to do nothing when passed a null pointer.

> https://en.cppreference.com/w/cpp/language/delete

如果表达式的计算结果为空指针值，则不会调用析构函数，也可能调用或不调用释放函数(未指定)，但默认的释放函数保证在传递空指针时不做任何事情。


!!! note "阅读提示"
    本文讨论 C++ 中**初始化**（initialization）与**赋值**（assignment）的区别，可参见 [类初始化列表](../cpp-idioms/class-initializer-list.md)。