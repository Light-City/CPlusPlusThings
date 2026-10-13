> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp20/cpp-coroutines/) 的文章，由站点同步脚本导出。

# C++20 协程入门

## 1.什么是Coroutines

自C++20之后，C++终于支持协程了，C++ 23应该带来额外的支持，至少应该涵盖最常见的用例。

**协程**（英语：coroutine）是计算机程序的一类组件，推广了[协作式多任务](https://zh.wikipedia.org/wiki/协作式多任务)的[子例程](https://zh.wikipedia.org/wiki/子例程)，允许执行被挂起与被恢复。相对子例程而言，协程更为一般和灵活，但在实践中使用没有子例程那样广泛。协程更适合于用来实现彼此熟悉的程序组件，如[协作式多任务](https://zh.wikipedia.org/wiki/协作式多任务)、[异常处理](https://zh.wikipedia.org/wiki/异常处理)、[事件循环](https://zh.wikipedia.org/wiki/事件循环)、[迭代器](https://zh.wikipedia.org/wiki/迭代器)、[无限列表](https://zh.wikipedia.org/wiki/惰性求值)和[管道](https://zh.wikipedia.org/wiki/管道_(软件))。

> 来自维基百科的一段话描述了协程的基本思想。

从C++角度，可以简单理解协程是**任何包含co_return, co_yield或co_await的函数**。

目前C++20协程是函数对象上的语法糖，编译器将围绕协程生成一个代码框架。此代码依赖于用户定义的**promise types**。可能会在C++23之后对此有所变动，不过在此之前，我们先按照C++20的规则来，需要自己编写这些类型。

前面提到co_return、co_yield、co_await，这些将在未来章节进行阐述。

## 2.Step By Step Coroutine

接下来，我们从一个简单的coroutine例子入手，下面这个协程不会做任何事情，我们称它为HelloTask courtine。

```cpp
#include <coroutine>
struct HelloTask {
    struct promise_type {
        HelloTask get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};
HelloTask myCoroutine() {
    co_return; // make it a coroutine
}
int main() {
    HelloTask x = myCoroutine();
}
```

编译运行这段代码：

```cpp
g++ hello.cpp -fcoroutines -std=c++20
```

需要你的编译器支持C++20特性。

从上到下来浏览一遍这段HelloWorld程序跟平时的开发有何不同。

### 2.1 头文件

首先是头文件引入了`coroutine`，这里我直接拿gcc-10的源码，看看到底做了什么事情。

> https://github.com/gcc-mirror/gcc/blob/releases/gcc-10/libstdc++-v3/include/std/coroutine

内容包括如下：

```cpp
// 用于发现协程promise_type的特征类型
// 核心代码:using promise_type = typename _Result::promise_type;
1.coroutine_traits

// 协程句柄，用于管理暂停或执行的协程以及Promise
2.coroutine_handle
  // coroutine_handle全特化版本	是其他特化的公开基类。它保有handle的底层		地址。
  2.1 coroutine_handle<void>

  // coroutine_handle泛化版本
  2.2 template <typename _Promise>
      struct coroutine_handle : coroutine_handle<>


  // coroutine_handle全特化版本 处理无操作协程
  2.3 template <>
      struct coroutine_handle<noop_coroutine_promise> : public 			coroutine_handle<>
3.平凡多可等待对象 Trivial awaitables
  // await表达式应该暂停
  3.1 suspend_always
  // await表达式不应该暂停
  3.2 suspend_never
```

可以看到里面包含了很多专有的术语：

- coroutine_handle
- Promise
- awaitable

等等。

后面所讲的协程内容都是围绕这些来展开的。

## 2.2 Promise

我们继续上面的HelloTask，会发现里面有这么一段:

```cpp
struct HelloTask {
    struct promise_type {
        HelloTask get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};
```

那么问题来了，为何这里要写promise_type？

之所以写promise_type是因为我们在上述2.1节提到了handle，当编译器创建协程的handle时，需要指定该协程对象的promise_type，所以所有的协程对象基本都有一个HelloTask::promise_type结构。仔细的朋友可能就非常懂了，这里传递promise_type过去，在Handle里面会解析出来，从而去判断当前用户自定义的协程对象的行为。

进一步的看看promise_type里面有什么，这里面的内容都是固定的，随着后续的内容越来越多，所涉及的函数也便是越来越多，例如这里使用的是`return_void`，那么当后面使用co_return返回对象时，便会使用另外一个函数了。

**get_return_object**是可以返回当前用户自定义对象的，这里是不做任何操作，承接上文2.1，我们可以使用coroutine_handle来做。

```cpp
struct promise_type {
    using Handle = std::coroutine_handle<promise_type>;
    HelloTask get_return_object() {
        return HelloTask{Handle::from_promise(*this)};
    }
  // other member function
}
```

可以看到这里使用了coroutine_handle的from_promise成员去构造了一个对象出来。

**initial_suspend**：告诉编译器协程是否应该在第一次执行时保持挂起，或者表示协程实例化时是否保持挂起。

**final_suspend**：与initial_suspend相反，在协程函数体完成后，在协程的末尾有一个final_suspend。这使你有机会进行额外的清理。

这两个都返回awaitable，这里采用了头文件中的suspend_always，当然也可以用户自定义awaitable，这个会在后面文章进行阐述。

suspend_always与suspend_never的源码如下:

```cpp
struct suspend_always
{
  constexpr bool await_ready() const noexcept { return false; }

  constexpr void await_suspend(coroutine_handle<>) const noexcept {}

  constexpr void await_resume() const noexcept {}
};

struct suspend_never
{
  constexpr bool await_ready() const noexcept { return true; }

  constexpr void await_suspend(coroutine_handle<>) const noexcept {}

  constexpr void await_resume() const noexcept {}
};
```

这里面包含了几个成员，

- await_ready
  - 如果知道结果已经准备就绪是否同步完成？
- await_suspend
  - **返回值可以是bool 、void、coroutine handle。**
  - 这里返回的是void，则立即将控制返回到当前Coroutine的 caller/resumer （此Coroutine仍然被暂停），否则如果等待_suspend返回bool，true表示控制权交给caller/resumer，表示false恢复当前的coroutine。关于返回coroutine handle在后面文章进行阐述。
- await_resume
  - 它的结果是整个co_await表达式的结果。所以返回值不一定是void，还可以是自定义对象之类的。

**return_void**: 当使用`co_return;`时，将调用return_void。

**unhandled_exception**：用于捕获异常。

最后，还差一个创建coroutine一步，根据一开始的定义协程是**任何包含co_return, co_yield或co_await的函数**。因此便有了:

```cpp
HelloTask myCoroutine() {
    co_return; // make it a coroutine
}
```

### 2.3 整体流程

整个调用流程是什么呢？本节来详细阐述一下上述代码的执行逻辑，开始之前我们改一下代码，加上一些print操作:

```cpp
#include <iostream>
#include <coroutine>
struct HelloTask {
    struct promise_type {
        HelloTask get_return_object() { std::cout << "get_return_object" << std::endl; return { }; }
        std::suspend_never initial_suspend() { std::cout << "initial_suspend" << std::endl; return {}; }
        std::suspend_never final_suspend() noexcept { std::cout << "final_suspend" << std::endl;return {}; }
        void return_void() { std::cout << "return_void" << std::endl; }
        void unhandled_exception() { std::cout << "unhandled_exception" << std::endl; }
    };
};
HelloTask myCoroutine() {
    std::cout << "create coroutine" << std::endl;
    co_return; // make it a coroutine
}
int main() {
    std::cout << "start main" << std::endl;
    HelloTask x = myCoroutine();
    std::cout << "end main" << std::endl;
}
```

此时输出:

```cpp
start main
get_return_object
initial_suspend
create coroutine
return_void
final_suspend
end main
```

流程如下：

- start main
- 在堆上分配coutine对象，构造promise，调用promise.get_return_object返回当前对象(此时输出了get_return_object)，这里这么做的目的是把结果保存到了局部变量里面。
- promise.initial_suspend，此时输出initial_suspend，这里会触发co_await，输出create coroutine，执行协程体后，调用co_return。
- co_return调用promise.return_void 输出return_void。
- 最终，promise.final_suspend输出final_suspend。
- end main

关于协程的相关内容还有特别多的东西，本节为开篇，后续将持续更新~