> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-concurrency/std-thread-internals/) 的文章，由站点同步脚本导出。

# C++ std::thread 内核实现

## 深度揭秘std::thread

当我们在写一个lambda时，传给thread，线程创建与task调用是在何时触发？

```cpp
auto t = std::thread([this] {
  // do something
});
```

这这个线程中传递了一个lambda，那么本节所要解决的问题是：这个lambda何时触发？是在构造的时候？还是在某处调用时？

来，让我们一起进入源码时代。

## 1._M_make_routine

本节所分析的代码分别如下：

1) 头文件

> https://github.com/gcc-mirror/gcc/blob/releases/gcc-4.9/libstdc%2B%2B-v3/include/std/thread

2. 实现文件

> https://github.com/gcc-mirror/gcc/blob/releases/gcc-4.9/libstdc%2B%2B-v3/src/c%2B%2B11/thread.cc

3. posix头文件

> https://github.com/gcc-mirror/gcc/blob/releases/gcc-4.9/libgcc/gthr-posix.h

上面的代码会调用下面这个构造函数：

```cpp
template<typename _Callable, typename... _Args>
explicit 
thread(_Callable&& __f, _Args&&... __args)
{
  _M_start_thread(_M_make_routine(std::__bind_simple(
          std::forward<_Callable>(__f),
          std::forward<_Args>(__args)...)));
}
```

`__f`为我们传入的lambda，这里通过bind将lambda与参数绑定在一起，随后传给`_M_make_routine`，便构造出`_Impl`。

```cpp
template<typename _Callable>
      shared_ptr<_Impl<_Callable>>
      _M_make_routine(_Callable&& __f)
      {
	// Create and allocate full data structure, not base.
	return std::make_shared<_Impl<_Callable>>(std::forward<_Callable>(__f));
      }
```

而`_Impl`里面存的是刚才bind的返回结果`_M_func`。

```cpp
template<typename _Callable>
      struct _Impl : public _Impl_base
      {
	_Callable		_M_func;

	_Impl(_Callable&& __f) : _M_func(std::forward<_Callable>(__f))
	{ }

	void
	_M_run() { _M_func(); }
      };
```

随后`_M_make_routine`创建好的`shared_ptr<_Impl<_Callable>>`传递给`_M_start_thread`。

## 2._M_start_thread

`_M_start_thread`会调用`__gthread_create`，在这里会传递`execute_native_thread_routine`。

```cpp
void
  thread::_M_start_thread(__shared_base_type __b)
  {
    if (!__gthread_active_p())
#if __EXCEPTIONS
      throw system_error(make_error_code(errc::operation_not_permitted),
			 "Enable multithreading to use std::thread");
#else
      __throw_system_error(int(errc::operation_not_permitted));
#endif

    __b->_M_this_ptr = __b;
    int __e = __gthread_create(&_M_id._M_thread,
			       &execute_native_thread_routine, __b.get());
    if (__e)
    {
      __b->_M_this_ptr.reset();
      __throw_system_error(__e);
    }
  }
```

关键点1:`execute_native_thread_routine`

在下面代码中给出了几个信息：

第一：为何要extern "C"

第二：调用了`_M_run()`

```cpp
  extern "C"
  {
    static void*
    execute_native_thread_routine(void* __p)
    {
      thread::_Impl_base* __t = static_cast<thread::_Impl_base*>(__p);
      thread::__shared_base_type __local;
      __local.swap(__t->_M_this_ptr);

      __try
	{
	  __t->_M_run();
	}
      __catch(const __cxxabiv1::__forced_unwind&)
	{
	  __throw_exception_again;
	}
      __catch(...)
	{
	  std::terminate();
	}

      return 0;
    }
  } // extern "C"
```

关键点2：`__gthread_create`

调用了pthread_create

```cpp
static inline int
__gthread_create (__gthread_t *__threadid, void *(*__func) (void*),
		  void *__args)
{
  return __gthrw_(pthread_create) (__threadid, NULL, __func, __args);
}
```

好了，我们来梳理一下上面的流程，当我们构造std::thread时，传入了自定义的callback(lambda)执行我们的业务逻辑，std::thread构造函数会发起pthread_create的调用，同时把execute_native_thread_routine以C语言风格传递给pthread_create，在执行pthread_create会执行execute_native_thread_routine，那么里面的`_M_run()`便会执行，这便是对应我们的lambda函数调用。

本节对线程的调用有了更近一步的认识，你学到了吗，欢迎分享。