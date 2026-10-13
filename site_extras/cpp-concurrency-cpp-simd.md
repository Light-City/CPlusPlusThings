> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-concurrency/cpp-simd/) 的文章，由站点同步脚本导出。

# C++ SIMD 向量化

最近在看相关向量化的内容，看起来有点头大，借此机会，学习一下高性能SIMD编程。

SIMD全称single-instruction multiple-data,单指令多数据。

在传统的计算机架构中，CPU一次只能处理一个数据元素。但是，许多任务涉及对大量数据执行相同的操作，例如对数组中的所有元素进行加法、乘法或逻辑操作等。SIMD编程通过向CPU提供专门的指令集，使得CPU能够同时对多个数据元素执行相同的操作。

这种处理方式特别适合涉及向量、矩阵、图像、音频和视频等数据的计算。

目前比较常用的有SSE、SSE2、AVX128、AVX256、AVX512。

本节，将简单学习一下AVX512的一些操作，操作比较多，这里只是引入一些。

## 1.术语

首先第一个问题便是，simd编程的代码跟平时写的代码长相不大一样，各种下划线以及命名，完全看不懂，如何理解呢？

诸如：

- _mm512_set1_ps

Broadcast single-precision (32-bit) floating-point value a to all elements of dst.

- _mm512_set1_epi32

Broadcast 32-bit integer a to all elements of dst.

于是，找到了下面这个表格：

| Abbreviation | Full Name                      | C/C++ Equivalent |
| ------------ | ------------------------------ | ---------------- |
| ps           | packed single-precision        | float            |
| ph           | packed half-precision          | None*            |
| pd           | packed double-precision        | double           |
| pch          | packed half-precision complex  | None*            |
| pi8          | packed 8-bit integer           | int8_t           |
| pi16         | packed 16-bit integer          | int16_t          |
| pi32         | packed 32-bit integer          | int32_t          |
| epi8         | extended packed 8-bit integer  | int8_t           |
| epi16        | extended packed 16-bit integer | int16_t          |
| epi32        | extended packed 32-bit integer | int32_t          |
| epi64        | extended packed 64-bit integer | int64_t          |
| epi64x       | extended packed 64-bit integer | int64_t          |

> https://stackoverflow.com/questions/70911872/what-are-the-names-and-meanings-of-the-intrinsic-vector-element-types-like-epi6

再比如：

_mm512_mask_load_ps

_mm512_mask_loadu_ps

u表示unordered，表示加载无序，当使用 `_mm512_mask_loadu_ps` 函数加载内存中的数据时，不会执行对内存地址的任何对齐要求。而_mm512_mask_load_ps要求满足 64 字节对齐要求。

这样对照着学习，非常快的便可以知道每个接口的含义了。

相关API可以看看Interl Intrinsics Guide。

> https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html

## 2.实际例子

### 2.1 求最小值

对于512位，我们可以存储16个32位float。

```cpp
static inline rf_512 load(float* ptr) { return _mm512_loadu_ps(ptr); }
float a[width] = {1.0, 2.0,  3.0,  22.0, 5.0,  17.0, 9.0,  8.0,
                  9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};
float b[width] = {3.3, 6.2,   5.3,   4.4,   5.5,   6.6,   7.7,  8.8,
                  9.9, 10.10, 21.11, 12.12, 13.13, 14.14, 15.0, 16.16};
rf_512 data = minimum(load(a), load(b));
```

于是我们可以快速得到：

```cpp
1 2 3 4.4 5 6.6 7.7 8 9 10 11 12 13 14 15 16
```

## 2.2 快速打乱数据顺序

对于输入数据是

```
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
```

我们可以快速得到：

```cpp
16.16 2 14.14 13.13 5 6 7 8 9 10 11 12 13 14 15 16
```

对应实现：

```cpp
static inline rf_512 permutexvar(ri_512 idx, rf_512 src) {
  return _mm512_permutexvar_ps(idx, src);
}
/*
raw data: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
shuffle data: 16.16 2 14.14 13.13 5 6 7 8 9 10 11 12 13 14 15 16
*/
void print_permutexvar_mask() {
  float a[width] = {1.0, 2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0,
                    9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};
  float b[width] = {1.1, 2.2,   3.3,   4.4,   5.5,   6.6,   7.7,  8.8,
                    9.9, 10.10, 11.11, 12.12, 13.13, 14.14, 15.0, 16.16};
  mask_type mask = make_bit_mask<1, 0, 1, 1, 0>();
  int idx_array[width] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  ri_512 idx = _mm512_loadu_si512((__m512i*)idx_array);
  rf_512 result = permutexvar_mask(load(a), mask, idx, load(b));
  float result_arr[width];
  store(result, result_arr);
  std::cout << "raw data: ";
  for (int i = 0; i < width; i++) {
    std::cout << a[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "shuffle data: ";
  for (int i = 0; i < width; i++) {
    std::cout << result_arr[i] << " ";
  }
  std::cout << std::endl;
}
```

### 2.3 旋转

对于一个数组：

```
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
```

我们可以旋转得到：

```
4 5 6 7 8 9 10 11 12 13 14 15 16 1 2 3
```

or

```
14 15 16 1 2 3 4 5 6 7 8 9 10 11 12 13
```

也就是rotate down or rotate up。

在这里，我们可以这样实现：

```cpp
rf_512 rotateGeneral(float* arr, int s) {
  int idx_array[width];
  for (int i = 0; i < width; i++) {
    idx_array[i] = (i + s) % width;
  }
  ri_512 idx = _mm512_loadu_si512((__m512i*)idx_array);
  rf_512 result = permutexvar(idx, load(arr));
  return result;
}
```

等等，还有其他的例子，可以发现通过使用simd，我们可以实现一些非常有趣的算法，加速对数组，批量数据的处理。

后面会继续学习simd，一起加油吧~

注：本节的完整示例已在星球更新，谢谢~