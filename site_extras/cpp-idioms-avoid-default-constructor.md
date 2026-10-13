> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-idioms/avoid-default-constructor/) 的文章，由站点同步脚本导出。

# C++ 避免默认构造函数

在 C++ 中，有一些容器通过 `resize()` 方法调整大小时，默认构造函数会被调用，这可能并不是我们想要的行为。本文将介绍如何使用自定义分配器来控制这一过程，避免默认构造的调用。

示例1：

```cpp
// [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
vec.resize(5);
vec.resize(10);
```

你知道此时输出什么吗？

答案是：

```
[0, 1, 2, 3, 4]
[0, 1, 2, 3, 4, 0, 0, 0, 0, 0]
```

这里需要明白几点内容：

**第一：为何resize回去之后的数据变为了0，发生了什么？**

**第二：如果我想要原来的数据，怎么做？即我想要输出结果是：**

```cpp
[0, 1, 2, 3, 4]
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```

**第三：如果vector的成员没有默认构造函数会发生什么？**

**第四：resize有什么限制？**

那么本节就来研究一下这个强大的功能，如何让resize避免调用默认构造。

注：篇幅有限，懒人版本节所有代码更新于星球。

## 1.resize剖析

为了让本文更加丝滑，我使用了下面示例，禁止Foo的构造，然后vector上面resize它。

```cpp
class Foo {
 public:
  Foo() = delete;

 private:
  int a;
};

std::vector<Foo> f;
f.resize(10);
```

此时会编译错误，我们可以看到一堆报错信息。

第一个问题：可以看到调用栈到了`__uninitialized_default_n`，此时会去检查当前类型是否是平凡类型，这便是resize的第一个限制！

```cpp
required from '_ForwardIterator std::__uninitialized_default_n(_ForwardIterator, _Size) [with _ForwardIterator = Foo*; _Size = long unsigned int]'
  710 |       return __uninitialized_default_n_1<__is_trivial(_ValueType)
      |              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  711 |                                          && __can_fill>::
```

第二个问题：由于我们禁止了默认构造，那么分配器在原地址placement new 一个默认对象的时候失败了，此时便报错，这便是第二个限制！

```cpp
gcc/14.1.0_1/include/c++/14/bits/stl_construct.h:119:7: error: use of deleted function 'Foo::Foo()'
  119 |       ::new((void*)__p) _Tp(std::forward<_Args>(__args)...);
      |       ^~~~~~~~~~~~~~~~~~~
```

那么说到这里了，便可以进入正文了，可以看到resize的时候会去使用分配器去分配内存，当传递的大小大于size(此时假设在capacity内，即地址有效)，那么此时会使用分配器placement new一个默认构造函数的对象。

gcc的源码实现可以放在这里给大家看一下，很简单吧？就是调用construct。

```cpp
__try
{
  typedef __gnu_cxx::__alloc_traits<_Allocator> __traits;
  for (; __n > 0; --__n, (void)++__cur)
    __traits::construct(__alloc, std::__addressof(*__cur));
  return __cur;
}
__catch(...)
{
  std::_Destroy(__first, __cur, __alloc);
  __throw_exception_again;
}
```

好了，resize原理讲明白了，我们来讲讲一开始的问题。

## 2.避免调用默认构造

如何避免调用默认构造呢？

那其实很简单了，就是屏蔽调gcc默认的construct，怎么屏蔽了？

这里便用到了vector的第二个参数：分配器！

## 3.分配器

为了解决这个问题，我们可以使用自定义分配器。自定义分配器可以控制对象的构造行为，允许我们选择在调用 `resize` 时是否使用默认构造。以下是一个简单的自定义分配器实现，只贴了核心代码。

```cpp
template <class T>
class no_init_alloc {
 public:
  using value_type = T;
  // Override construct method to do nothing
  template <class U>
  void construct(U* p) {
    // Do nothing, memory is not initialized
  }
};
```

我们只需要在construct不做任何事情即可，此时便可以使用它，即：

```cpp
std::vector<Foo, no_init_alloc<Foo>> std_vec;
```

此时便可以得到：

```cpp
[0, 1, 2, 3, 4]
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```

而不是：

```
[0, 1, 2, 3, 4]
[0, 1, 2, 3, 4, 0, 0, 0, 0, 0]
```

好了，你学会了吗？

> 本节完