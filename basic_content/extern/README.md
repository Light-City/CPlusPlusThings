# extern "C"

## 关于作者：

个人公众号：

![](../img/wechat.jpg)

## 1.C++与C编译区别

在C++中常在头文件见到extern "C"修饰函数，那有什么作用呢？ 是用于C++链接在C语言模块中定义的函数。

C++虽然兼容C，但C++文件中函数编译后生成的符号与C语言生成的不同。因为C++支持函数重载，C++函数编译后生成的符号带有函数参数类型的信息，而C则没有。

例如`int add(int a, int b)`函数经过C++编译器生成.o文件后，`add`会变成形如`add_int_int`之类的, 而C的话则会是形如`_add`, 就是说：相同的函数，在C和C++中，编译后生成的符号不同。

这就导致一个问题：如果C++中使用C语言实现的函数，在编译链接的时候，会出错，提示找不到对应的符号。此时`extern "C"`就起作用了：告诉链接器去寻找`_add`这类的C语言符号，而不是经过C++修饰的符号。

## 2.C++调用C函数

C++调用C函数的例子: 引用C的头文件时，需要加`extern "C"`

```c++
//add.h
#ifndef ADD_H
#define ADD_H
int add(int x,int y);
#endif

//add.c
#include "add.h"

int add(int x,int y) {
    return x+y;
}

//add.cpp
#include <iostream>
#include "add.h"
using namespace std;
int main() {
    add(2,3);
    return 0;
}
```

编译：

```
//Generate add.o file
gcc -c add.c
```

链接：

```
g++ add.cpp add.o -o main
```

没有添加extern "C" 报错：

```c++
> g++ add.cpp add.o -o main                                   
add.o：在函数‘main’中：
add.cpp:(.text+0x0): `main'被多次定义
/tmp/ccH65yQF.o:add.cpp:(.text+0x0)：第一次在此定义
/tmp/ccH65yQF.o：在函数‘main’中：
add.cpp:(.text+0xf)：对‘add(int, int)’未定义的引用
add.o：在函数‘main’中：
add.cpp:(.text+0xf)：对‘add(int, int)’未定义的引用
collect2: error: ld returned 1 exit status
```

添加extern "C"后：

`add.cpp`

```c++
#include <iostream>
using namespace std;
extern "C" {
    #include "add.h"
}
int main() {
    add(2,3);
    return 0;
}
```

编译的时候一定要注意，先通过gcc生成中间文件add.o。

```
gcc -c add.c 
```

然后编译：

```
g++ add.cpp add.o -o main
```

而通常为了C代码能够通用，即既能被C调用，又能被C++调用，头文件通常会有如下写法：

```c
#ifdef __cplusplus
extern "C"{
#endif
int add(int x,int y);
#ifdef __cplusplus
}
#endif
```

即在C++调用该接口时，会以C接口的方式调用。这种方式使得C++者不需要额外的extern C，而标准库头文件通常也是类似的做法，否则你为何不需要extern C就可以直接使用stdio.h中的C函数呢？


上述案例源代码见：

- [add.h](extern_c++/add.h)

- [add.c](extern_c++/add.c)

- [add.cpp](extern_c++/add.cpp)

## 2.C中调用C++函数

`extern "C"`在C中是语法错误，需要放在C++头文件中。

```c
// add.h
#ifndef ADD_H
#define ADD_H
extern "C" {
    int add(int x,int y);
}
#endif

// add.cpp
#include "add.h"

int add(int x,int y) {
    return x+y;
}

// add.c
extern int add(int x,int y);
int main() {
    add(2,3);
    return 0;
}
```

编译：

```c
g++ -c add.cpp
```

链接：

```
gcc add.c add.o -o main
```

上述案例源代码见：

- [add.h](extern_c/add.h)

- [add.c](extern_c/add.c)

- [add.cpp](extern_c/add.cpp)

综上，总结出使用方法，在C语言的头文件中，对其外部函数只能指定为extern类型，C语言中不支持extern "C"声明，在.c文件中包含了extern "C"时会出现编译语法错误。所以使用extern "C"全部都放在于cpp程序相关文件或其头文件中。

总结出如下形式：

（1）C++调用C函数：

```c++
//xx.h
extern int add(...)

//xx.c
int add(){
    
}

//xx.cpp
extern "C" {
    #include "xx.h"
}
```

（2）C调用C++函数

```c
//xx.h
extern "C"{
    int add();
}
//xx.cpp
int add(){
    
}
//xx.c
extern int add();
```


不过与C++调用C接口不同，C++确实是能够调用编译好的C函数，而这里C调用C++，不过是把C++代码当成C代码编译后调用而已。也就是说，C并不能直接调用C++库函数。
