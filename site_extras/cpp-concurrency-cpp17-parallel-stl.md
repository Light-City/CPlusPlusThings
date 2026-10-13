> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-concurrency/cpp17-parallel-stl/) 的文章，由站点同步脚本导出。

# C++17 并行 STL

## 1.各个编译器支持进度

不同编译器对并行算法库的支持，如下：

> https://en.cppreference.com/w/cpp/compiler_support/17#C.2B.2B17_library_features

可以看到gcc9之后就支持了，clang目前支持还不完全，自己也本地测了一下确实不支持。

![](../assets/images/2024/parallel/img/fea.png)

其支持的算法包括：

![](../assets/images/2024/parallel/img/all_std.png)

> https://en.cppreference.com/w/cpp/experimental/parallelism

## 2.并行算法库要求

gcc9以后便可以使用并行算法库了，我们可以看一下下面的gcc-9文档。

> https://gcc.gnu.org/gcc-9/changes.html

其中，提到一个非常重要的信息：**需要tbb支持**。

![](../assets/images/2024/parallel/img/tbb_req.png)

TBB是由英特尔（Intel）公司开发的一个用于并行编程的 C++ 库。TBB 旨在简化多核处理器上的并行软件开发，提供了一套抽象和模型，使程序员能够更容易地实现并行算法。

1.C++17当中使用并行算法库的前提必须得安装tbb，那如果不安装tbb会怎样呢？

我自己实测就是可以编译与运行，只是没有任何加速，跟原来非并行跑出来的结果是一样的。

2.TBB如何安装呢？

一个是官方的tbb，叫做oneTBB。

> https://github.com/oneapi-src/oneTBB

另一个是其他人写的tbb，叫做tbb。

> https://github.com/wjakob/tbb

区别在于添加了基于 CMake 的构建系统。

现在官方也是支持cmake ，所以建议去使用官方的吧。

编译也是非常简单：

```
mkdir build && cmake ..
make -j8
make install
```

## 3.如何使用

相比于传统的STL使用又有哪些改变呢？

**1.接口层面**

其实比较简单，例如 sort原来是：

```cpp
std::sort(begin, end, comp);
```

现在变为：

```
std::sort(exe_policy, begin, end, comp);
```

只需要添加执行策略(execution policy)，有三个选项：

1. **`std::execution::seq`（顺序执行）:**
   - 这是默认的执行策略，也就是说，如果你没有明确指定执行策略，算法将使用顺序执行。
   - 算法将按照顺序依次处理元素，不涉及并行处理。
2. **`std::execution::par`（并行执行）:**
   - 使用这个执行策略将启用并行执行。算法会尝试以并行的方式处理元素，充分利用多核处理器。
3. **`std::execution::par_unseq`（并行和向量化执行）:**
   - 这个执行策略允许算法在并行的同时进行向量化（vectorization），这意味着可以利用 SIMD（Single Instruction, Multiple Data）指令集进行并行向量化操作。
   - 与 `std::execution::par` 不同，`par_unseq` 在能够并行处理的同时还允许矢量化处理，从而更充分地利用硬件的并行和向量化能力。

**2.头文件层面**

必须要引入：

```cpp
#include <algorithm>
#include <execution>
```

如果有tbb的操作，可以引入tbb的头文件，例如：设置线程数。

**3.编译层面**

编译时需要指定链接信息，例如：

```cpp
g++ -O3 -std=c++17 -Wall -Wextra -pedantic -I/usr/local/include/tbb -L /usr/local/lib/ -o main.out main.cc -ltbb
```

## 4.性能测试

最近在做sort相关的工作，想看看merge与sort的性能，所以写了个benchmark，逻辑如下：

- 串行sort vs 并行sort(par策略)

- 串行merge vs 并行merge(par策略)

数据范围：

```cpp
BENCHMARK(BM_ParallelSort)->Arg(16384)->Arg(16384000)->Arg(163840000);  
```

下面的测试结果将采用tbb默认线程调度策略。

**对比组1：并行Sort/串行Sort详细数据与加速比**

![](../assets/images/2024/parallel/img/sort_parallel.png)

![](../assets/images/2024/parallel/img/sort_parallel_sp.png)

**对比组2：并行Merge/串行Merge详细数据与加速比**

![](../assets/images/2024/parallel/img/merge_parallel.png)

![](../assets/images/2024/parallel/img/merge_parallel_sp.png)

tbb文档：

> https://oneapi-src.github.io/oneTBB/index.html

如果要设置线程数，可以这样操作：

```cpp
#include <tbb/tbb.h>

tbb::task_scheduler_init init(16);
```