> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-memory-debug/address-sanitizer-asan/) 的文章，由站点同步脚本导出。

# AddressSanitizer 内存检测

C和C++是非常不安全且容易出错的编程语言，Address Sanitizer是由Google开发的一种工具，用于检测内存访问错误，如使用后释放（use-after-free）和内存泄漏。它已集成到GCC版本>= 4.8中，可用于C和C++代码。Address Sanitizer使用运行时插桩来跟踪内存分配，这意味着您必须使用AddressSanitizer构建代码，以充分利用其功能。

- [Use after free](https://github.com/google/sanitizers/wiki/AddressSanitizerExampleUseAfterFree) (dangling pointer dereference)
- [Heap buffer overflow](https://github.com/google/sanitizers/wiki/AddressSanitizerExampleHeapOutOfBounds)
- [Stack buffer overflow](https://github.com/google/sanitizers/wiki/AddressSanitizerExampleStackOutOfBounds)
- [Global buffer overflow](https://github.com/google/sanitizers/wiki/AddressSanitizerExampleGlobalOutOfBounds)
- [Use after return](https://github.com/google/sanitizers/wiki/AddressSanitizerExampleUseAfterReturn)
- [Use after scope](https://github.com/google/sanitizers/wiki/AddressSanitizerExampleUseAfterScope)
- [Initialization order bugs](https://github.com/google/sanitizers/wiki/AddressSanitizerInitializationOrderFiasco)
- [Memory leaks](https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizer)

本文将会以上面8种场景来介绍内存检测工具**AddressSanitizer**。

## 1.Use after free

以下面代码为例。首先，通过`new`关键字分配了一个整数的内存空间，并将其地址赋给指针 `ptr`。然后，使用 `delete` 关键字释放了这块内存。但是，在释放后，程序仍然尝试通过指针 `ptr` 访问已经释放的内存，并将值 10 赋给该内存位置，这就是使用后释放错误。

```cpp
#include <iostream>

int main() {
  int* ptr = new int;
  delete ptr;
  *ptr = 10;  // Use after free
  return 0;
}
```

在编译时添加下面选项。

```
-fsanitize=address
```

运行生成的bin文件，得到如下结果。

![](../assets/images/2024/mem/address/heap_use_after_free.png)

## 2.heap buffer overflow

首先，通过 `new` 关键字在堆上分配了一个包含两个整数的数组，并将数组的起始地址赋给指针 `arr`。然后，程序试图将值 10 存储在数组的第三个位置（索引为 2），这超出了数组的有效范围，导致堆缓冲区溢出错误。

```
#include <iostream>

int main() {
  int* arr = new int[2];
  arr[2] = 10;  // Heap buffer overflow
  delete[] arr;
  return 0;
}

```

同上面用法：

```
g++-13 heap_buffer_overflow.cc  -Wall -g -fsanitize=address 
```

然后：

```
./a.out
```

得到下面检测结果。

![heap_buffer_overflow](../assets/images/2024/mem/address/heap_buffer_overflow.png)

## 3.stack buffer overflow

在 `vulnerableFunction` 函数中，定义了一个包含 8 个字符的字符数组 `buffer`。然后，使用 `std::cin` 从标准输入读取数据存储到 `buffer` 中，但没有限制输入的大小。如果输入的字符数超过了 `buffer` 的大小，就会发生栈缓冲区溢出错误。

```cpp
void vulnerableFunction() {
  char buffer[8];
  std::cin >> buffer;  // Stack buffer overflow
  std::cout << "You entered: " << buffer << std::endl;
}
```

用法同上。

![stack_buffer_overflow](../assets/images/2024/mem/address/stack_buffer_overflow.png)

## 4.global buffer overflow

全局缓冲区（`buffer` 数组）在程序的全局范围内定义，而没有受到足够的输入大小限制。如果输入的字符数超过了缓冲区的大小，就会发生全局缓冲区溢出错误。与栈缓冲区溢出类似，全局缓冲区溢出可能导致程序的不稳定性和安全性问题。

```cpp
#include <iostream>

char buffer[8];

int main() {
    std::cin >> buffer; // 全局缓冲区溢出错误
    std::cout << "You entered: " << buffer << std::endl;
    return 0;
}
```

用法同上，得到：

![global_buffer_overflow](../assets/images/2024/mem/address/global_buffer_overflow.png)

## 5.use after return 

在函数 `f` 中，局部变量 `i` 的地址被返回，但在 `main` 函数中，尝试通过返回的指针访问已经被销毁的局部变量。当出现这种场景是，可以对bin文件进行检测，设置`ASAN_OPTIONS=detect_stack_use_after_return=1`。

```
g++-13 stack_use_after_return.cc -Wall -g -fsanitize=address
ASAN_OPTIONS=detect_stack_use_after_return=1 ./a.out
```

代码：

```cpp
#include <iostream>

int *f() {
  int i = 42;
  int *p = &i;
  return p;
}

int main() { return *f(); }

```

我们便会得到：

![stack_use_after_return](../assets/images/2024/mem/address/stack_use_after_return.png)

## 6.use after scope

将局部变量 `x` 的地址赋给指针 `p`，然而，在超出该内部作用域后，尝试通过指针 `p` 访问已经超出范围的局部变量 `x`，导致范围外使用错误。这也是未定义行为。

```cpp
#include <iostream>
int main() {
  int* p = 0;
  {
    int x = 0;
    p = &x;
  }
  *p = 5;
  return 0;
}
```

编译与运行：

```
g++-13 stack_use_after_scope.cc -Wall -g -fsanitize=address
ASAN_OPTIONS=detect_stack_use_after_scope=0  ./a.out 
```

实测clang++可以，g++无法检测。

![stack_use_after_scope](../assets/images/2024/mem/address/stack_use_after_scope.png)

## 7.Initialization order bugs

在两个不同的源文件中，存在全局变量 `a` 的初始化，但初始化的顺序不确定。在 `init_order_b.cc` 文件中，尝试使用变量 `a` 的值，但由于初始化顺序不一致，可能导致未定义的行为。

代码如下：

```cpp
// init_order_a.cc
int duplicate(int n) { return n * 2; }
auto a = duplicate(2);

// init_order_b.cc
#include <iostream>

extern int a;
auto b = a;  

int main() {
  std::cout << b << std::endl;
  return 0;
}
```

当我们编写这样的代码时，其结果是不确定的，可能是4也可能是0，取决于初始化的顺序。

例如：

```
clang++ init_order_a.cc init_order_b.cc  -Wall -g -fsanitize=address
```

这样输出是4，而先编译b，后编译a，得到0。

```
clang++ init_order_b.cc init_order_a.cc  -Wall -g -fsanitize=address
```

当出现这种问题时，我们可以使用AddressSanitizer检测初始化顺序。

```cpp
ASAN_OPTIONS=check_initialization_order=true ./a.out 
```

![init_order](../assets/images/2024/mem/address/init_order.png)

## 8.memory leak

通过 `new` 关键字动态分配了一个整数的内存，但在程序结束前未使用 `delete` 关键字释放这块内存，导致内存泄漏。

```cpp
#include <iostream>

int main() {
  int* ptr = new int;
  // Missing delete ptr; // Memory leak
  return 0;
}

```

设置`ASAN_OPTIONS=detect_leaks=1`。

```
ASAN_OPTIONS=detect_leaks=1 ./a.out
```

![memory_leak](../assets/images/2024/mem/address/memory_leak.png)