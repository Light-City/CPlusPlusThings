> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-concurrency/cpp-deadlock-debugging/) 的文章，由站点同步脚本导出。

# C++ 死锁排查

## C++如何排查并发编程死锁问题？

最近在Apache arrow里面写一个支持并行的算子：nested loop join，然后既然涉及到并行，这里就会遇到大家常说的死锁问题，假设你碰到了死锁问题，如何调试与定位呢？

那这便是本篇目标，帮助你快速掌握并发编程：死锁检测与死锁调试问题，非常的干，好了进行正文。

## 1.引入

为了更好的讲解死锁，我们用一个程序来引入。

```cpp
std::mutex gMutex;

int t2() {
  std::lock_guard<std::mutex> m(gMutex);
  return 0;
}

int t1() {
  std::lock_guard<std::mutex> l(gMutex);
  return t2();
}
```

相信看这个程序，大家都会觉得有问题，死锁了！问题出在`t1()`函数和`t2()`函数中都对全局的互斥锁`gMutex`进行了加锁操作，但是`t1()`函数在加锁后调用了`t2()`函数，而`t2()`函数内部又试图再次对`gMutex`进行加锁。

t1锁已经加上了，但还没释放，t2又去加锁，两个人都在等待谁先释放，进入了死循环，实际在项目中代码并不会如这里这么简单，非常的复杂，例如：我在Apache arrow中写的代码是这样：

```cpp
Status OnBuildSideFinished(size_t thread_index) {
  std::lock_guard<std::mutex> guard(probe_side_mutex_);
  // do something
  accumulate_build_ready_ = true;
  return scheduler_->StartTaskGroup(thread_index,task_group_probe_,queued_batches_to_probe_.batch_count()); 
}
```

你看这个代码比上面的场景就复杂多了，嵌套了至少5层堆栈，剩余代码还没贴出来，但是这两个本质都是一个死锁模型。

## 2.调试

讲解了死锁模型之后，碰到这种问题，如何定位呢？

这里可以采用两种办法，第一种直接运行程序，然后gdb上去。

例如：

```
./a.out
```

然后找到进程号后：

```
gdb -p xxx
```

此时我们可以得到及格正在等待的线程。

```cpp
(gdb) info threads 
  Id   Target Id                                 Frame 
* 1    Thread 0x7ffff7fe2740 (LWP 32301) "a.out" 0x00007ffff7bc8017 in pthread_join () from /lib64/libpthread.so.0
  2    Thread 0x7ffff6fd0700 (LWP 32305) "a.out" 0x00007ffff7bcd54d in __lll_lock_wait () from /lib64/libpthread.so.0
```

然后去看`__lll_lock_wait`的堆栈，例如这里我看了2号线程，然后查看堆栈得到t1与t2的行号，直接可以定位到哪里出了问题，非常的直观！



除了这种方式之外，还可以直接gdb上去运行程序，此时会卡死，然后ctrl + c杀掉之后也是可以跟上面一样的内容。

例如：

```
(gdb) r
Starting program: /home/light/a.out 
Missing separate debuginfos, use: debuginfo-install glibc-2.17-326.el7_9.x86_64
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib64/libthread_db.so.1".
Hello World!
[New Thread 0x7ffff6fd0700 (LWP 32305)]
^C
Thread 1 "a.out" received signal SIGINT, Interrupt.
0x00007ffff7bc8017 in pthread_join () from /lib64/libpthread.so.0
Missing separate debuginfos, use: debuginfo-install libgcc-4.8.5-44.el7.x86_64 libstdc++-4.8.5-44.el7.x86_64
(gdb) info threads 
  Id   Target Id                                 Frame 
* 1    Thread 0x7ffff7fe2740 (LWP 32301) "a.out" 0x00007ffff7bc8017 in pthread_join () from /lib64/libpthread.so.0
  2    Thread 0x7ffff6fd0700 (LWP 32305) "a.out" 0x00007ffff7bcd54d in __lll_lock_wait () from /lib64/libpthread.so.0
```

好了，本节就讲这么多，感兴趣的欢迎转发这篇硬核文章！于此同时，推荐一下自己的**线程池项目**，个人用视频讲解了这个项目，同时配上docker一键部署(这一期的视频，后面打算给大家录制一下)，感兴趣的联系我即可。