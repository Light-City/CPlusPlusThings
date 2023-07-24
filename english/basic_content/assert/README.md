# Things about assert

## About Author：


![](../img/wechat.jpg)



## 1.First assertion case

Assert, **is macro，rather than function**.

Assert the prototype of a macro is defined in <assert.h>(C) or <cassert>(C++). If its condition is false or returns an error, program execution will terminate.

You can disable assert by defining macro `NDEBUG`, **But it needs to be at the beginning of the source code，before include <assert.h>.**

```c
void assert(int expression);
```

> Code Example：[assert.c](./assert.c)
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
Output：
```c
assert: assert.c:13: main: Assertion 'x==7' failed.
```

## 2.Assertion and normal error handling

+ Assertions are mainly used to check for logically impossible situations.

> For example, they can be used to check the state that code expects before it starts to run, or after the run is complete. Unlike normal error handling, assertions are usually disabled at run time.

+ Ignore the assertion and add at the beginning of the code：
```c++
#define NDEBUG          // Adding this line，then assert will be disable
```

> Code Example：[ignore_assert.c](./ignore_assert.c)
