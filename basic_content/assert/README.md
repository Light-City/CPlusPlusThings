# assert那些事

## 关于作者：

个人公众号：

![](../img/wechat.jpg)



## 1.第一个断言案例

断言，**是宏，而非函数**。

assert 宏的原型定义在 <assert.h>（C）、<cassert>（C++）中。其作用是如果它的条件返回错误，则终止程序执行。

可以通过定义 `NDEBUG` 来关闭 assert，**但是需要在源代码的开头，include <assert.h> 之前。**

```c
void assert(int expression);
```

> 代码样例：[assert.c](./assert.c)
```c
#include <stdio.h> 
#include <assert.h> 

int main() 
{ 
    int x = 7; 

    /*  Some big code in between and let's say x  
    is accidentally changed to 9  */
    x = 9; 

    // Programmer assumes x to be 7 in rest of the code 
    assert(x==7); 

    /* Rest of the code */

    return 0; 
} 
```
输出：
```c
assert: assert.c:13: main: Assertion 'x==7' failed.
```
可以看到输出会把源码文件，行号错误位置，提示出来！

## 2.断言与正常错误处理

+ 断言主要用于检查逻辑上不可能的情况。

>例如，它们可用于检查代码在开始运行之前所期望的状态，或者在运行完成后检查状态。与正常的错误处理不同，断言通常在运行时被禁用。

+ 忽略断言，在代码开头加上：
```c++
#define NDEBUG          // 加上这行，则 assert 不可用
```

> 样例代码：[ignore_assert.c](./ignore_assert.c)
