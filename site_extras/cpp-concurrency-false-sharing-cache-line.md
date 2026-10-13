> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-concurrency/false-sharing-cache-line/) 的文章，由站点同步脚本导出。

# C++ False Sharing 与缓存行

最近看到一段代码，手动做的对齐，于是研究一下不对齐又会带来什么影响？

```cpp
template <typename T>
class AtomicWithPadding {
 private:
  static constexpr int kCacheLineSize = 64;
  uint8_t padding_before[kCacheLineSize];

 public:
  std::atomic<T> value;

 private:
  uint8_t padding_after[kCacheLineSize];
};
```

这里便引出了大家面试的必考点：

- Cache line

于是有下面几个面试问题，你能答上来？

- 什么是cache line?
- 什么是MESI协议？
- 什么是False sharing?
- C++里面如何保证避免False sharing?
- 避免False sharing可以带来什么好处？
- 你知道C++17之后提供了哪两个变量解决这个问题？

于是，找了个性能测试，大概实现是：

- 定义4个变量，a、b、c、d，多线程进行共享
- 另外是对这四个变量对齐，然后开n个线程去调度。

下面是benchmark：

```cpp
------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations
------------------------------------------------------------------
singleThread                  4.05 ms         4.04 ms          133
directSharing/real_time       8.78 ms        0.069 ms           77
falseSharing/real_time        8.81 ms        0.083 ms           74
noSharing/real_time           1.16 ms        0.071 ms          578
```

可以看到noSharing得到了非常好的效果！缓存命中率大大提高，当然这里用perf分析一下是更好的，可以看看L1 cache miss。

以上答案在知识星球已更新，欢迎一起探讨，留言区交流，谢谢~