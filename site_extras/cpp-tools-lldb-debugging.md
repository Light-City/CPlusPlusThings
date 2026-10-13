> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-tools/lldb-debugging/) 的文章，由站点同步脚本导出。

# LLDB 调试 C++ 程序

[TOC]

本文将介绍如何使用 LLDB 调试器调试一个简单的 C++ 程序。我们的示例代码命名为 `test.cpp`。

```cpp
#include <iostream>
#include <string>

struct MyStruct {
    int myInt;
    std::string myString;
};

int main() {
    MyStruct myStruct;
    myStruct.myInt = 42;
    myStruct.myString = "Hello, world!";

    int sum = 0;
    for (int i = 1; i <= 5; ++i) {
        sum += i;
    }

    return 0;
}
```

## 编译

首先，我们需要使用 `-g` 标志编译代码以启用额外的调试信息。

```sh
clang++ -g test.cpp -o test
```

## 开始调试

接着，我们附加调试器。

```sh
lldb test
```

## 断点

我们将断点设置在 `main()` 函数。

```sh
(lldb) b main
```

输出：

Breakpoint 1: where = test`main + 19 at test.cpp:10:14, address = 0x0000000100003be3

## 运行

然后，我们启动进程。

```sh
(lldb) run
```

输出：

```cpp
Process 11333 launched: '/Users/light/test' (x86_64)
Process 11333 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 1.1
    frame #0: 0x0000000100003be3 test`main at test.cpp:10:14
   7   	};
   8   	
   9   	int main() {
-> 10  	    MyStruct myStruct;
   11  	    myStruct.myInt = 42;
   12  	    myStruct.myString = "Hello, world!";
   13  
```

## 查看断点

现在，我们已经停在了 `main()` 函数的开头。我们可以设置在特定行上的断点。

```sh
(lldb) b 16
```

输出：Breakpoint 2: where = test`main + 76 at test.cpp:16:16, address = 0x0000000100003c1c

要列出当前设置的断点，请使用以下命令。

```sh
(lldb) breakpoint list
```

输出：

```
(lldb) breakpoint list
Current breakpoints:
1: name = 'main', locations = 1, resolved = 1, hit count = 1
  1.1: where = test`main + 19 at test.cpp:10:14, address = 0x0000000100003be3, resolved, hit count = 1 

2: file = '/Users/zhangxing/test.cpp', line = 16, exact_match = 0, locations = 1, resolved = 1, hit count = 0
  2.1: where = test`main + 76 at test.cpp:16:16, address = 0x0000000100003c1c, resolved, hit count = 0 
```

## 程序继续运行

我们可以继续执行程序直到下一个断点。

```sh
(lldb) c
```

输出：

```
Process 11823 resuming
Process 11823 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 2.1
    frame #0: 0x0000000100003c1c test`main at test.cpp:16:16
   13  	
   14  	    int sum = 0;
   15  	    for (int i = 1; i <= 5; ++i) {
-> 16  	        sum += i;
   17  	    }
   18  	
   19  	    return 0;
```

## 打印变量

要获取当前局部变量的值，可以使用以下命令：

```sh
(lldb) p myStruct
```

输出：

```
(MyStruct) $0 = (myInt = 42, myString = "Hello, world!")
```

## 观察点

要监视特定变量的值更改，我们可以使用监视点。

```sh
(lldb) watch set variable sum
```

输出：

```cpp
Watchpoint created: Watchpoint 1: addr = 0x7ffeefbff1e8 size = 4 state = enabled type = w
    declare @ '/Users/zhangxing/test.cpp:14'
    watchpoint spec = 'sum'
    new value: 0
```

当next后，会看到Watchpoint的变化。

```cpp
(lldb) n

Watchpoint 1 hit:
old value: 3
new value: 6
Process 11823 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = watchpoint 1
    frame #0: 0x0000000100003c25 test`main at test.cpp:15:29
   12  	    myStruct.myString = "Hello, world!";
   13  	
   14  	    int sum = 0;
-> 15  	    for (int i = 1; i <= 5; ++i) {
   16  	        sum += i;
   17  	    }
   18  	

```

如果想要根据条件停止执行，可以设置停止条件。

```sh
(lldb) watch modify -c '(sum > 5)'
```

输出：

```cpp
Watchpoint 1 hit:
new value: 6
Process 12848 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = watchpoint 1
    frame #0: 0x0000000100003c25 test`main at test.cpp:15:29
   12  	    myStruct.myString = "Hello, world!";
   13  	
   14  	    int sum = 0;
-> 15  	    for (int i = 1; i <= 5; ++i) {
   16  	        sum += i;
   17  	    }
   18  	
```

完成调试后，可以继续执行程序。

```sh
(lldb) c
```

如需了解更多信息，请参阅 LLDB 文档：

- [LLDB 和 GDB 对照指南](https://lldb.llvm.org/lldb-gdb.html)
- [LLDB 教程](https://lldb.llvm.org/tutorial.html)

---

通过这些步骤，您可以轻松地使用 LLDB 调试您的 C++ 程序，查找错误并解决问题。