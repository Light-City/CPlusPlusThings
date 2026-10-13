> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp17/cpp17-pmr/) 的文章，由站点同步脚本导出。

# C++17 PMR 内存资源

[TOC]

## 1.引入

在C++17之前，标准库提供了`std::allocator`，而在C++17中，这一功能得到了加强，引入了`polymorphic_allocator`。

在C++17之前，如果我们想要使用`std::allocator`来自定义内存池，我们不能使用传统的虚拟多态方式，因为`std::allocator`并没有提供虚拟函数。因此，通过继承显然不是一个好的选择，但是我们可以将其作为类成员使用，如下所示：

```cpp
template <typename Allocator = std::allocator<uint8_t>>
class MemoryPool {
 public:
  explicit STLMemoryPool(const Allocator& alloc) : alloc_(alloc) {}

  Status Allocate(int64_t size, uint8_t** out) {
    try {
      *out = alloc_.allocate(size);
    } catch (std::bad_alloc& e) {
      return Status::OutOfMemory(e.what());
    }
    return Status::OK();
  }

  void Free(uint8_t* buffer, int64_t size) {
    alloc_.deallocate(buffer, size);
  }

 private:
  Allocator alloc_;
};
```

在C++17之前如果我们想在内存分配/释放时做一些print操作，或者一些自定义操作，可以使用两种办法：

- 自定义全局的new/delete

```cpp
void* operator new(std::size_t size) {
  void* ptr = malloc(size);
  std::cout << "Allocated: " << size << " bytes at address " << ptr << std::endl;
  return ptr;
}

void operator delete(void* ptr, std::size_t n) noexcept {
  std::cout << "Deallocated: " << n << " bytes at address " << ptr << std::endl;
  free(ptr);
}
```

- 自定义allocator

```cpp
class allocator {
 public:
  using value_type = T;
  value_type* allocate(std::size_t n) {
    value_type* p = static_cast<value_type*>(::operator new(n * sizeof(value_type)));
    std::cout << "Allocated: " << n * sizeof(value_type) << " bytes at address "
              << static_cast<void*>(p) << std::endl;
    return p;
  }

  void deallocate(value_type* p, std::size_t n) noexcept {
    std::cout << "Deallocated: " << n * sizeof(value_type) << " bytes at address "
              << static_cast<void*>(p) << std::endl;
    ::operator delete(p);
  }
};

std::vector<int, allocator<int>> vec;
```

那么，除此之外，还有哪些办法呢？

C++17之后，我们可以通过使用多态内存管理工具，polymorphic memory resources(pmr)，使用方式：

```cpp
#include <memory_resources>

pmr::monotonic_buffer_resource res(10);
pmr::vector<vector<int>> vec1(&res);
```

对于上面这个问题，我们可以通过继承memory_resource，有关下面代码的解释见下方memory_resource。

```cpp
class TrackAllocator : public std::pmr::memory_resource {
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        void* p = std::pmr::new_delete_resource()->allocate(bytes, alignment);
        std::cout << "do_allocate: " << bytes << " bytes at " << p
                  << " alignment: " << alignment << std::endl;
        return p;
    }

    void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override {
      std::cout << "do_deallocate: " << bytes << " bytes at "
                << " alignment: " << alignment << std::endl;
      return std::pmr::new_delete_resource()->deallocate(p, bytes, alignment);
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return std::pmr::new_delete_resource()->is_equal(other);
    }
};
```

至此，我们便做到了上面的效果。

## 2.memory_resource

STL中std::par::memory_resource类的实现比较简单，以下为一个伪代码实现：

```cpp
class memory_resource {
  static const size_t __max_align = alignof(max_align_t);

public:
  virtual ~memory_resource();

  void* allocate(size_t __bytes, size_t __align = __max_align) {
    return do_allocate(__bytes, __align);
  }

  void deallocate(void* __p, size_t __bytes, size_t __align = __max_align) {
    do_deallocate(__p, __bytes, __align);
  }

  bool is_equal(const memory_resource& __other) const noexcept { 
    return do_is_equal(__other); 
  }

private:
  virtual void* do_allocate(size_t, size_t)                       = 0;
  virtual void do_deallocate(void*, size_t, size_t)               = 0;
  virtual bool do_is_equal(memory_resource const&) const noexcept = 0;
};
```

与默认的std::allocator不同，memory_resource提供了多态的能力，允许用户通过继承的方式，重写以下三个接口：

- do_allocate
  - 用于分配指定大小和对齐要求的内存。
- do_deallocate
  - 释放之前通过 `do_allocate` 分配的内存。
- do_is_equal
  - 检查当前内存资源对象是否与另一个内存资源对象相等。

于是，我们上方的TrackAllocator通过重写上面三个函数接口即可。

细心的朋友发现，除了这几个接口，memory_resource也提供了对外的三个接口。除此之外，提供了全局的接口：

```cpp
memory_resource* new_delete_resource() noexcept;
memory_resource* null_memory_resource() noexcept;
memory_resource* get_default_resource() noexcept;
memory_resource* set_default_resource(memory_resource* __r) noexcept;
```

- new_delete_resource

返回一个`memory_resource`指针，该指针使用`new`和`delete`运算符来分配和释放内存。这个资源是使用全局的`new`和`delete`运算符实现的，因此它是默认的内存资源管理器。这个memory_resource子类是`__resource_adaptor_imp`，它会负责重写上面三个接口。

- null_memory_resource

返回一个`memory_resource`指针，该指针表示一个不执行任何操作的空内存资源。当你想要在不进行实际内存分配的情况下测试或占位时，可以使用这个资源。STL源码当中的实现中定义了一个

```cpp
inline memory_resource*
null_memory_resource() noexcept {
	class type final : public memory_resource {};
}
```

type作为子类重写了上面三个接口，do_allocate接口会跑出分配错误的异常，因为语义就是不允许分配了。

```
do_allocate(size_t, size_t) override
    { std::__throw_bad_alloc(); }
```

所以当我们使用null_memory_resource，需要try catch，例如：string字符小于16是在stack上分配，此时不会用到我们这里的memory_resource，所以正常运行，当长度大于等于16，那么就会跑出std::bad_alloc异常。

```cpp
std::pmr::memory_resource* default_resource = std::pmr::null_memory_resource();

std::pmr::set_default_resource(default_resource);

try {
  std::pmr::vector<int> data{1};
} catch (std::bad_alloc e) {
  std::cerr << "bad_alloc is thrown vector" << std::endl;
}

std::pmr::string str1{"hello world hel"};
std::cerr << str1 << " length:" << str1.size() << std::endl;

try {
  std::pmr::string str1{"hello world hel0"};
} catch (std::bad_alloc e) {
  std::cerr << "bad_alloc is thrown string" << std::endl;
}
```

- get_default_resource

返回当前默认的全局内存资源管理器。默认情况下，它是一个与`new_delete_resource()`返回的相同资源，但通过`set_default_resource`函数可以更改默认资源。

- set_default_resource

设置内存资源管理器，参数可以为空(返回默认资源)，否则设置为传递的资源，并返回之前的默认资源。

## 3.内存复用

std::pmr::monotonic_buffer_resource是一个缓存区，在内存复用方面具有重要的优势。monotonic_buffer_resource继承memory_resource，重写上面三个接口，内部会持有上游的memory_resource，如果当前内存不足，便会从上游去取。这有助于降低内存碎片，提高内存利用率。

下面列出一些有关std::pmr::monotonic的使用方法。

**用法1 - 使用固定缓冲区创建 `monotonic_buffer_resource`：**

```cpp
char buf1[30] = {};
std::fill_n(std::begin(buf1), std::size(buf1) - 1, '_');
std::pmr::monotonic_buffer_resource pool1{std::data(buf1), std::size(buf1)};
std::pmr::vector<int> myVec1{&pool1};
```

**用法2 - 利用上游资源创建 `monotonic_buffer_resource`：**

```cpp
std::pmr::memory_resource* upstreamResource2 = std::pmr::get_default_resource();
std::pmr::monotonic_buffer_resource pool2(upstreamResource2);
{
    std::pmr::vector<double> numbers(&pool2);
    for (double d = 0.1; d < 1.0; d += 0.1) {
        numbers.push_back(d);
    }
}
```

**用法3 - 继承std::pmr::monotonic_buffer_resource：**

```cpp
class MyMonotonicBufferResource : public std::pmr::monotonic_buffer_resource {
public:
    using std::pmr::monotonic_buffer_resource::monotonic_buffer_resource; // Inherit constructors

    // Public function to access do_allocate
    void* allocate(std::size_t bytes, std::size_t alignment) {
        return do_allocate(bytes, alignment);
    }

    // Public function to access do_deallocate
    void deallocate(void* p, std::size_t bytes, std::size_t alignment) {
        do_deallocate(p, bytes, alignment);
    }
};

MyMonotonicBufferResource pool3;
// Allocate memory
void* mem = pool3.allocate(100, alignof(int));

// Use the allocated memory (this is just an example)
std::pmr::vector<int> numbers({1, 2, 3}, &pool3);

// Deallocate memory
pool3.deallocate(mem, 100, alignof(int));

std::cout << "Usage 3 - Inherit: \n\n";
```

## 4.pool resource

synchronized_pool_resource这是一个线程安全的内存池资源类。它的设计目的是在多线程环境中安全地进行内存分配和释放。当多个线程并发地尝试进行内存分配或释放时，`synchronized_pool_resource` 使用同步机制确保线程安全性。这通常会带来一些性能开销，因为需要加锁来保护共享的内存池。

与之对应的是unsynchronized_pool_resource，需要用户自己保证线程安全。

synchronized_pool_resource源码实现其实就是mutex + unsynchronized_pool_resource。

```cpp
class synchronized_pool_resource : public memory_resource {

public:
	// some interface
private:
  mutex __mut_;
  unsynchronized_pool_resource __unsync_;
};
```

pool_options可以用来配置这些内存池。

```cpp
struct pool_options {
  size_t max_blocks_per_chunk        = 0;
  size_t largest_required_pool_block = 0;
};
```

- max_blocks_per_chunk

表示每个块中最多可以包含的块数量。用于限制内存池从上游内存资源一次性获取的内存量。如果设置为 0，内核会通过munge_options函数去修改pool_options。

- largest_required_pool_block

表示内存池能够处理的最大块大小。