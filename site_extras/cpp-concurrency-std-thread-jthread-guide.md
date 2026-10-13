> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-concurrency/std-thread-jthread-guide/) 的文章，由站点同步脚本导出。

# C++ std::thread 与 jthread

## 全面解析thread 与 jthread

## 1.准备工作

本文的内容基于gcc-13的源码进行展开，讲详细讲解C++11 std::thread的使用与内核实现、讲解如何使用pthread + thread混用，讲解C++20 jthread的用法与内核实现。

如果说你是一个多线程开发的小白/大神，本篇文章都非常的适合学习，既会包含基础的理论知识，还会包含丰富的实践与内核代码揭秘。

源码剖析版本：gcc-13

> https://github.com/gcc-mirror/gcc/tree/releases/gcc-13

大纲如下：

[TOC]

## 2.C++11的thread

### 2.1 基本使用

创建一个thread，然后传入自己的执行函数，参数等。

这里的result可以使用atomic 也可以使用promise。

基本使用中一般需要注意t.join，让主线程等待子线程，它会阻塞当前线程，直到调用 join() 的线程执行完成。

```cpp
std::atomic<int> result(0);

void threadFunction(int a, int b) {
    result = a + b;
    std::cout << "Child thread finished its work.\n";
}

int main() {
    int a = 5;
    int b = 7;

    std::thread t(threadFunction, a, b);

    t.join();
    std::cout << "Result computed by the child thread: " << result << std::endl;

    return 0;
}
```

如果你没有在线程上指定 join() 或 dettach() ，则会导致运行时错误，因为主/当前线程将完成其执行，而创建的其他线程仍将运行。

示例：去掉上面的t.join()，你的程序将会崩溃。

```
Result computed by the child thread: 0
Child thread finished its work.
terminate called without an active exception
[1]    52814 abort      ./a.out
```

### 2.2 STL实现

大家可以去看std_thread.h文件，来跟我一起阅读内核实现。

代码路径：gcc/libstdc++-v3/include/bits/std_thread.h

#### 2.2.1 与pthread关联

首先它是一个类thread，然后有一个比较重要的类型，像我的Linux系统上面这个宏是打开的，默认一般是`__gthread_t`，而这个通常是一个pthread_t。

```cpp
#ifdef _GLIBCXX_HAS_GTHREADS
    using native_handle_type = __gthread_t;
#else
    using native_handle_type = int;
#endif
```

即：

```
typedef pthread_t __gthread_t;
```

所以我们可以得到第一个结论：底层实现基于linux的pthread实现。

#### 2.2.2 内部id

thread类，有一个私有成员叫做`_M_id`。

```cpp
private:
    id				_M_id;
```

其类型是哪部的一个类，再往里面深入就是native_handle_type。

```cpp
class id
{
  native_handle_type	_M_thread;

public:
  id() noexcept : _M_thread() { }

  explicit
  id(native_handle_type __id) : _M_thread(__id) { }
  
}
```

那再结合上面的内容，可以得到：

```cpp
private:
    pthread_t				_M_id;
```

是不是很熟悉，我们得到了第二个知识点：thread内部维护了一个id，在linux上默认是pthread_t。

#### 2.2.3 是否可连接

如果一个线程处于 "joinable" 状态，那么在它执行完成之前，程序可以调用 `join()` 函数来等待该线程的执行完成。

joinable判断依据是：

```cpp
_M_id != id()
```

基于前面的内容，我们可以推导出：

```cpp
pthread_t _M_id;
pthread_t null_thread = pthread_t();
_M_id != null_thread;
```

即：joinable判断依据是当前线程是否不等于默认的线程id(0)。

join方法非常的重要，我们通常需要使用它，那它的实现原理简单来说就是如下：

- 是否可joinable
  - 是，调用pthread_join(这里的gthread实际上会调用到pthread的接口)
  - 否或者上面join有错误，跑出异常EINVAL或者join错误。
- 设置为不可joinable，即：设置内部的线程id为0

```cpp
thread::join()
{
  int __e = EINVAL;

  if (_M_id != id()) // 是否可joinable
    __e = __gthread_join(_M_id._M_thread, 0);

  if (__e)
    __throw_system_error(__e);

  _M_id = id(); // _M_id = pthread_t()，设置为0
}
```

在join之后，如果我们再join，此时便会抛出上面EINVAL异常，即：无效参数。

```
terminate called after throwing an instance of 'std::system_error'
  what():  Invalid argument
```

#### 2.2.4 分离

跟join类似，不过相反，告诉程序你不再关心该线程的执行了，实现逻辑除了调用接口不一样，其他地方都一致，不赘述了。

detach重使用的是pthread_detach。

```cpp
void
thread::detach()
{
  int __e = EINVAL;

  if (_M_id != id())
    __e = __gthread_detach(_M_id._M_thread);

  if (__e)
    __throw_system_error(__e);

  _M_id = id();
}
```

#### 2.2.5 构造与析构

##### 2.2.5.1 构造

使用：

```
thread t1;
```

默认构造内核代码：

```cpp
thread() noexcept = default;
```

带参数的构造使用：

```cpp
std::thread t2(threadFunction, a, b);
```

带参数构造内核代码：

```cpp
template <typename _Callable, typename... _Args,
          typename = _Require<__not_same<_Callable>>>
explicit thread(_Callable &&__f, _Args &&...__args) {}
```

内部实现是：

```cpp
_M_start_thread(_State_ptr(new _State_impl<_Wrapper>(
                std::forward<_Callable>(__f), std::forward<_Args>(__args)...)),
            _M_thread_deps_never_run);
```

可以看到核心函数是`_M_start_thread`，这里面的代码简化为：

```cpp
const int err = __gthread_create(&_M_id._M_thread,
         &execute_native_thread_routine,
         state.get());
if (err)
  __throw_system_error(err);
```

而`__gthread_create`可以理解为是pthread_create，那么是不是非常的清楚了？

注意到这里有一个细节就是传递给pthread_create的参数重func是execute_native_thread_routine，上面转化为pthread_create为：

```cpp
pthread_create (&_M_id._M_thread, NULL, &execute_native_thread_routine, state.get());
```

而execute_native_thread_routine是一个extern "C" 包裹的函数，使 C++ 中的函数名称具有 C 链接。

所以上面构造函数逻辑非常清晰了，就是调用pthread_create初始化内部线程id，那么这里有一个疑问，**用户传递的函数与参数怎么处理呢？**

这就引入了State，细心的朋友注意到上面pthread_create的最后一个参数是通常的函数参数：`state.get()`，函数是execute_native_thread_routine。

```cpp
static void*
execute_native_thread_routine(void* __p)
{
  thread::_State_ptr __t{ static_cast<thread::_State*>(__p) };
  __t->_M_run();
  return nullptr;
}
```

`_State`是一个接口，其实现是`_State_impl`，可以看到参数与函数是转发到了这里面，然后通过`_M_run`去执行用户传递的函数，执行的时候是调用`_Callable`的operator()，在前面我们知道创建的时候是带了`_Wrapper`，所以`_Callable`等于`_Wrapper`，而`__Wrapper`又是一个`_Call_wrapper`别名，会将用户的函数与参数传递进去。

```cpp
using _Wrapper = _Call_wrapper<_Callable, _Args...>;

struct _State
{
  virtual ~_State();
  virtual void _M_run() = 0;
};
using _State_ptr = unique_ptr<_State>;


template <typename _Callable>
struct _State_impl : public _State
{
  _Callable _M_func;

  template <typename... _Args>
  _State_impl(_Args &&...__args)
      : _M_func(std::forward<_Args>(__args)...)
  {
  }

  void
  _M_run() { _M_func(); }
};
```

所以到这里，我们可以看一下

```cpp
template <typename... _Tp>
using _Call_wrapper = _Invoker<tuple<typename decay<_Tp>::type...>>;
```

相当于将上面_Call_wrapper的所有参数去掉引用和修饰之后作为tuple传递进去，即：

```cpp
_Invoker<tuple<_Callable, Arg1, Arg2, ...>
```

然后最终传递给`std::__invoke`。

所以，假设我写了这么一行代码：

```cpp
_Call_wrapper<decltype(&add), int, int> invoker(&add, 3, 4);
```

它会通过invoke帮你调用add函数。

```cpp
invoke(_Fn&& __f, _Args&&... __args)
```

当然上面调用的是其内部的`__invoke`，两者没有什么区别，一个是对外的接口，一个是对内的。

经过上面的讲解，我们可以小结一下：

- 带参数的构造函数会创建tid，通过pthread_create
- 传递给pthread_create的函数是一个包裹的函数，内部使用State去驱动
- 用户通过将函数、参数传递给thread，最终调用这个函数是通过`std::__invoke`来驱动起来。

##### 2.2.5.2 拷贝与移动

thead禁止了拷贝构造、拷贝赋值，只支持移动构造与移动赋值。

```cpp
thread(const thread &) = delete;
thread &operator=(const thread &) = delete;
```

移动构造

```cpp
thread(thread &&__t) noexcept
{
  swap(__t);
}

void
swap(thread &__t) noexcept
{
	std::swap(_M_id, __t._M_id);
}
```

移动赋值

```cpp
thread &operator=(thread &&__t) noexcept
{
  if (joinable())
    std::__terminate();
  swap(__t);
  return *this;
}
```

##### 2.2.5.3 析构

析构的原则是你创建的线程资源得消费/释放掉，而一个线程如果是可joinable，那么它被pthread_create了，但是还没有被使用，此时会直接terminate。

```cpp
~thread()
{
  if (joinable())
    std::__terminate();
}
```

#### 2.2.6 其他成员

例如：获取线程内部的线程id、线程的id内部的`_M_thread`。

```cpp
id
get_id() const noexcept
{ return _M_id; }

native_handle_type
native_handle()
{ return _M_id._M_thread; }
```

#### 2.2.7 面试考点

**1.构造与析构**

所以这里有一个提问：下面代码有什么问题吗？

```cpp
// test1.cc
int main() {
  thread t1;
}

// test2.cc
int main() {
	std::thread t2(threadFunction, a, b);
}
```

其答案是：test1正常运行，test2 crash！

原因是：thread的默认构造函数不做任何事情，而带参数的构造函数里面会调用`__gthread_create`(即：pthread_create)创建线程，即初始化了内部的`_M_id._M_thread`，在析构的时候会判断joinable，此时的`_M_id`与id()是不相等的，即：joinable = true，直接std::__terminate()此时crash了。

**2.移动赋值**

假设现在有下面两个函数f1、f2，这两个函数可以正常运行吗？为什么？

```cpp
void f1() {
  std::thread t1(tfunc);
  std::thread t2;
  t2 = std::move(t1);
  t2.join();
}
void f2() {
  std::thread t1(tfunc);
  std::thread t2(tfunc);
  t2 = std::move(t1);
  t2.join();
}
```

其答案是：f1正常运行，f2 crash!

原因是：`f1`中t2使用了默认构造函数(不做任何事情)，所以`t2`本身是非joinable的，可以安全使用移动赋值。但是，`f2`的t2是可joinable的(因此构造函数中初始化了tid)，而移动赋值时会判断是否joinable，如果joinable，那么直接terminate，这其实比较好理解，因为t2本身的线程被创建了，还没有消费使用，就给swap出去了，那如何修复这个问题呢？

其实很简单：在前面加个join即可：

```cpp
t2.join();  // 新加的
t2 = std::move(t1);
t2.join();
```

可能有些人觉得t2搞了两次join难道没问题吗，因此此时第二次的t2实际上已经是t1了，所以可以看作是：

```cpp
t2.join();
t1.join();
```



## 3.C++20的jthread

自C++20之后引入了`std::jthread`，它可以自动join启动的线程，并发出中断信号。

### 3.1 基本使用

如果我们使用前面的thread，下出下面这个代码会core掉，因为在析构函数中会去判断是否joinable，此时会terminate。

```cpp
std::thread t{ [] {std::cout << "hello std::thread" << std::endl; } };

std::cout << "t.joinable(): " << t.joinable() << std::endl;
```

如果使用了C++20的jthread，便不会有这个问题，例如：

```cpp
std::jthread tj{ [] {std::cout << "hello std::jthread" << std::endl; } };

std::cout << "tj.joinable(): " << tj.joinable() << std::endl;
```

此时你会发现可以正常运行！

### 3.2 STL实现

jthread是在thread里面，其实现基于前面的thread，源码位置：gcc/libstdc++-v3/include/std/thread

到底有什么秘密呢？我们一起来揭秘。

#### 3.2.1 与thread关联

jthread与thread息息相关，可以说基于thread实现，其中最核心的成员就是thread类型，而内部的id与native_handle_type类型均使用了thread内部的。

```cpp
using id = thread::id;
using native_handle_type = thread::native_handle_type;

thread _M_thread;
```

#### 3.2.2 stop_token与stop_source

jthread内部使用了 std::stop_token 增强控制。std::stop_token 是一种向线程执行发送停止请求的机制。 它作为请求运行，如果在线程执行中正确处理，则允许其被终止。

内部使用了std::stop_source成员来维护。

```cpp
stop_source _M_stop_source;
```

下面我们一起来看看STL的实现。

**1.stop_source**

首先stop_source内部会维护一个state，叫做：

```cpp
stop_token::_Stop_state_ref _M_state;
```

这个里面便是：

```
_Stop_state_t* _M_ptr
```

因为一个stop_source可以被不同的线程所持有，所以它可以是共享的，因此内部是通过一个引用来做的，所以你可能会在源码里面看到`_Stop_state_ref`，它来负责管理整个state的生命周期，所有权问题。其共享的实现是通过底层的`_Stop_state_t`实现，内部有两个成员非常重要：

```cpp
using value_type = uint32_t;
std::atomic<value_type> _M_owners{1};
std::atomic<value_type> _M_value{_S_ssrc_counter_inc};
```

一个是`_M_owners`，管理所有权的；`_M_value`负责管理什么时候停止。

我们平时用的三个接口分别是：

- stop_possible
- stop_requested
- request_stop

底层便会给`_M_value`设置上对应的bit

```cpp
static constexpr value_type _S_stop_requested_bit = 1;
static constexpr value_type _S_locked_bit = 2;
static constexpr value_type _S_ssrc_counter_inc = 4;
```

当用户请求停止：request_stop，此时会设置上`_S_stop_requested_bit`，如果用户调用了stop_requested，那么也是去查看内部的`_M_value`是否设置上`_S_stop_requested_bit`。

`_S_locked_bit`与stop_possible接口有关，如果已经发出了停止请求或者仍然存在可以发出停止请求，那么stop_possible会返回true。

另外一个是`_S_ssrc_counter_inc`，这个是默认值，在stop_source的构造/拷贝构造时会给value加上`_S_ssrc_counter_inc`。

**2.stop_token**

像上面讲到的`_Stop_state_ref`所有内容基本都在stop_token里面，它的内部也是维护了一个state，叫做：

```
_Stop_state_ref _M_state;
```

实现细节与上面一致，就不赘述了。那么它与stop_source的区别是什么呢？

这里引用cppreference的解释：stop_token 类提供了检查是否已为其关联的 std::stop_source 对象发出或可以发出停止请求的方法。 它本质上是相关停止状态的线程安全“视图”。

> https://en.cppreference.com/w/cpp/thread/stop_token

**3.stop_callback**

stop_callback与stop_token的关系是stop_callback会注册到stop_token里面，stop_token内部会维持一个双向链表结构来对callback进行删除与新增，插入callback采用的是头插法。

callback触发时机有两块：

- 在request_stop时触发

callback提前注册到stop_token里面，那么在执行一段时间，用户发起请求，此时会触发callback。

```cpp
auto onStop = []() {
    std::cout << "Timer has stopped." << std::endl;
};

// Register the onStop callback with stop_callback
std::stop_callback callback(source.get_token(), onStop);
// do something for example: sleep

// stop
source.request_stop(); // invoke callback
```

- 注册时立即触发

stop_token已经要求停止，此时callback在注册时会立即触发，因为线程已经执行完了，此时应该执行回调。

```cpp
thread.request_stop();
std::stop_callback callback(stopToken, []() {
  std::cout << "Callback executed" << std::endl;
}); // invoke callback
```

具体STL中的实现是`_M_register_callback`会调用`__cb->_M_run()`，此时会调用callback函数。

#### 3.2.3 构造与析构

##### 3.2.3.1 构造

- 默认构造

默认构造会初始化一个空的stopstate，内部tid不做维护，行为跟thread一致。

```cpp
jthread() noexcept
  : _M_stop_source{nostopstate}
  { }
```

- 带参数构造

初始化`_M_thread`，它是通过`_S_create`然后调用`_S_create_pmf`创建一个thread，然后给`_M_thread`，这样内部的thread变被构造好了。

```cpp
template<typename _Callable, typename... _Args,
   typename = enable_if_t<!is_same_v<remove_cvref_t<_Callable>,
             jthread>>>
  explicit
  jthread(_Callable&& __f, _Args&&... __args)
  : _M_thread{_S_create(_M_stop_source, std::forward<_Callable>(__f),
      std::forward<_Args>(__args)...)}
  { }
```

##### 3.2.3.2 拷贝与移动

拷贝构造与拷贝赋值被禁用掉了，行为跟thread一致！

```
jthread(const jthread&) = delete;
jthread& operator=(const jthread&) = delete;
```

移动构造默认，移动赋值使用swap。

```cpp
jthread(jthread&&) noexcept = default;

jthread&
operator=(jthread&& __other) noexcept
{
  std::jthread(std::move(__other)).swap(*this);
  return *this;
}

void
swap(jthread& __other) noexcept
{
  std::swap(_M_stop_source, __other._M_stop_source);
  std::swap(_M_thread, __other._M_thread);
}
```

##### 3.2.3.3 析构

析构决定了jthread在一开始的行为，为何我们不用手动join，是因为在析构的时候，它会去判断是否可join，如果joinable，那么用户没有join过，析构函数帮你调用一下join，同时会在之前检查request_stop。

```cpp
~jthread()
{
  if (joinable())
    {
      request_stop();
      join();
    }
}
```

##### 3.2.3.4 其他成员

其他成员基本上是在thread上进行操作，然后对外暴露了一个接口，例如：

```cpp
[[nodiscard]] bool
joinable() const noexcept
{
  return _M_thread.joinable();
}

join()
{
  _M_thread.join();
}

void
detach()
{
  _M_thread.detach();
}

[[nodiscard]] id
get_id() const noexcept
{
  return _M_thread.get_id();
}

[[nodiscard]] native_handle_type
native_handle()
{
  return _M_thread.native_handle();
}

[[nodiscard]] static unsigned
hardware_concurrency() noexcept
{
  return thread::hardware_concurrency();
}
```

剩下一些是对stop_source的操作：

```cpp
[[nodiscard]] stop_source
get_stop_source() noexcept
{
  return _M_stop_source;
}

[[nodiscard]] stop_token
get_stop_token() const noexcept
{
  return _M_stop_source.get_token();
}

bool request_stop() noexcept
{
  return _M_stop_source.request_stop();
}
```

### 3.3 示例

cppreference中这个例子非常不错，在示例中用户工作函数传递了stop_source作为参数，在内部可以通过判断stop_requested()，如果停止了，我们可以做一些释放资源的操作，还是非常有用的。

触发的条件就是主线程执行了request_stop。

> https://en.cppreference.com/w/cpp/thread/stop_source