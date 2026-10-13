> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-projects/google-benchmark-cpp/) 的文章，由站点同步脚本导出。

# Google Benchmark 性能测试

## 《C++那些事》之开启你的BenchMark测试

## 0.导语

本节目标：完成一个BenchMark小项目!

在平时开发中，如何测试自己的接口性能呢？C++里面如何快速搭建一个BenchMark测试框架呢？

本节将Step By Step开启BenchMark入门的第一课。

## 1.项目结构

我们以vector与list插入为例，来讲解BenchMark项目。

这次项目是基于Bazel来管理的，如果还不知道如何配置相关环境的，请参考之前的视频课程/文档资料。

创建一个项目结构为：

```
```

BenchMark采用google的开源项目：

```cpp
https://github.com/google/benchmark.git
```

像以前大家使用第三方库，需要自己源码编译一波，生成so库，然后链接，太麻烦了！这里直接使用bazel管理，只需要在WORKSPACE里面，添加依赖的git项目，例如这里采用google benchmark的v1.7.1版本。

```cpp
git_repository(
    name = "google_benchmark",
    remote = "https://github.com/google/benchmark.git",
    tag = "v1.7.1",
)
```

随后，我们需要实现主逻辑，创建一个src目录，里面添加一个BUILD文件：目的在于编译过会cpp文件，注意：这里依赖了benchmark库，使用方法为@**前面配置的name**//:benchmark

```cpp
cc_binary(
    name = "vector_list",
    srcs = ["vector_list.cc"],
    deps = [
        "@google_benchmark//:benchmark",
    ]
)
```

这样的话，在cc文件中便可以使用了:

```cpp
#include <benchmark/benchmark.h>
```

### 2.Demo环节

编写两个插入函数，非常简单，框架如下所示，里面有一些需要说明的，如下：

- state为benchmark的运行时状态信息
  - 例如：休眠控制、入参数控制等
- state.range(0)便是获取第0个参数
- DoNotOptimize表示不做编译器优化

```cpp
// 测试 std::vector 的插入速度
static void BM_vectorInsert(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v;
        for (size_t i = 0; i < state.range(0); ++i) {
            v.insert(v.begin() + i, i);
        }
        benchmark::DoNotOptimize(v);
    }
}

// 测试 std::list 的插入速度
static void BM_listInsert(benchmark::State& state) {
    for (auto _ : state) {
        std::list<int> l;
        for (size_t i = 0; i < state.range(0); ++i) {
            l.insert(l.begin(), i);
        }
        benchmark::DoNotOptimize(l);
    }
}
```

主入口调用：

- Range(8, 8 << 10) 表示从8开始，每次乘以8，直到8192，这个对应上面的state.range(0)，会把这个数作为参数传递进去
- BENCHMARK_MAIN表示使用google benchmark的main函数，我们知道每个可执行文件都有一个main，那这个文件可以执行的原因就在于，这个宏展开后就是main函数，你学会了吗？

```cpp
// 定义基准测试
BENCHMARK(BM_vectorInsert)->Range(8, 8<<10);
BENCHMARK(BM_listInsert)->Range(8, 8<<10);

// 运行基准测试
BENCHMARK_MAIN();
```

## 3.总结

本节Step by Step带大家学习google benchmark，并将其应用到我们自己的项目中，你可以在里面做很多有趣的事情，本节源码放入知识星球当中供大家学习，欢迎加入一起交流~