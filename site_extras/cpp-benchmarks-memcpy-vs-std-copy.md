> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-benchmarks/memcpy-vs-std-copy/) 的文章，由站点同步脚本导出。

# memcpy 与 std::copy 性能对比

最近在做性能优化时，发现有一些代码用了std::copy，还有一些用了memcpy，两者在功能上其实类似，那么它们对性能影响是怎样的呢？

于是本篇文章将会探讨这两者的一些使用场景。

## memcpy：理论上有一些性能优势

从理论上讲，`memcpy`可能具有轻微的性能优势，原因在于其不同于`std::copy`的要求。`memcpy`在处理数据拷贝时可以忽略数据重叠的可能性，而`std::copy`需要处理一些不同的情况(底层memmove)。然而，这种性能差异在实际应用中往往难以察觉，特别是对于小规模的数据拷贝操作。

值得注意的是，`memcpy`只能用于处理C风格的POD结构，因此在处理非POD类型时并不是一个可行的选择。

这里使用quick-bench做了一个测评，可以看到性能并无太大差别。

> https://quick-bench.com/q/QER6YyI5f33kmqNqV7_Zdw8P5AE

![](../assets/images/2024/bench/img/copy_memcpy.png)

## 如何选择？

在实际项目中，如何选择`memcpy`还是`std::copy`取决于具体的需求。如果处理的是POD类型，而且希望保持灵活性，那么选择`std::copy`是更为明智的选择。对于大多数应用场景，`std::copy`提供了更通用、更安全的数据拷贝方式。

然而，在追求极致性能的情况下，可以考虑使用`memcpy`，尤其是在处理大规模数据时。但在做出选择时，务必结合实际应用场景，并借助性能分析工具进行详细测试和优化。

总的来说，`memcpy`和`std::copy`都有各自的优势和劣势，我们需要根据项目的具体要求，在性能和灵活性之间做出权衡。在实践中，通常选择更安全和通用的`std::copy`是一个更可靠的选择。