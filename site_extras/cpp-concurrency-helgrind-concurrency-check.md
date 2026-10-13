> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-concurrency/helgrind-concurrency-check/) 的文章，由站点同步脚本导出。

# Helgrind 并发错误检测

## C++那些事之helgrind并发编程检测

通常我们在写多线程程序的时候很容易遇到两个问题：

- 死锁了，不知道什么原因导致
- 数据不一致，多个线程锁用错了

那么有没有工具来检测这两种场景呢

答案是有的，我们可以使用valgrind的helgrind工具检测这两个问题，为了使本文讲解的更加丝滑，引出了几个例子。

**注：完整示例及修复示例已更新至星球。**

## 死锁

假设有两个线程互相持有对方的锁，此时我们可以模拟出死锁，例如：

```cpp
void thread_func1() {
    std::lock_guard<std::mutex> lock1(mutex1);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::lock_guard<std::mutex> lock2(mutex2);
}

void thread_func2() {
    std::lock_guard<std::mutex> lock2(mutex2);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::lock_guard<std::mutex> lock1(mutex1);
}
```

helgrind使用的方法如下：

```
valgrind --tool=helgrind ./a.out 
```

此时我们可以看到死锁的位置，例如这个示例中指出了两个线程在对应位置发生了死锁

```
thread_func2() (deadlock.cc:18)
thread_func1() (deadlock.cc:12)
```

![](../assets/images/2024/img/deadlock.png)

## 数据竞争

假设有一个共享变量，我们在多线程对它增加10w次，这个结果是多少呢？

很明显这里产生了多线程竞争全局变量的问题，最终的结果不一致，使用helgrind检测方法同上。

```cpp
int shared_var = 0; 

void increment() {
    for (int i = 0; i < 100000; ++i) {
        ++shared_var; 
    }
}
```

此时，我们可以看到data race的行数是哪一行，然后去解决问题。

![](../assets/images/2024/img/datarace.png)

最后，留几个问题吧：

1.上面这两个有问题的代码，如何修复？

2.除了上面工具，还有其他办法？

> 本节完