> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp20/cpp20-modules/) 的文章，由站点同步脚本导出。

# C++20 模块 Modules

[TOC]

当我们使用自己编写的头文件或者第三方库时，通常会用到`#include `指令来引入**库**，这是大家经常使用的一种方式。这种方法，实际上是将一个源文件(头文件)的所有代码拷到另一个文件中。

那么，这会面临如下问题：

- 源文件可能在同一目标中被包含多次，因此我们通常会使用`#pragma once`或者`#ifndef`，从而防止源文件在同一翻译单元中被包含多次。
- 代码的拷贝会导致编译时间更长，一旦修改一个头文件，便会导致间接包含这个头文件的一些文件被重新编译。
- `#include `顺序问题，有时候会遇到莫名其妙的编译问题。

C++20引入了一种替代 `#include 指令`的新方式，称为模块。

下面来深入学习一下模块。

## 1.模块单元

C++模块由一个或多个翻译单元(tu)组成，其中包含用于模块声明的特定关键字。这样的翻译单元称为模块单元。

非模块单元的翻译单元被认为是全局模块的一部分，全局模块是匿名的，没有接口，并且包含常规的非模块代码。

### 1.1 Global Module Fragment

模块单元可以以全局模块片段作为**前缀**，当无法导入头文件时（特别是当头文件使用预处理宏作为配置时），该全局模块片段可以直接使用原来的代码。

例如：下面代码中`module;`到`export module Foo;`中间为global module fragment。

```cpp
module;
#include <iostream>
#ifdef Say
void hello();
#endif
export module Foo;
// purivew
void world();
```

必须要注意的一点是：**如果该模块有全局模块片段，那么第一个声明必须是`module;`**，也就是说当把这个声明放在其他位置会出错。

### 1.2 purview

purview可以理解为模块的整个范围。从模块声明开始，一直延伸到翻译单元的末尾。

例如：hello不在，world、GetData都在purview。

```cpp
module;
void hello();
// <- 这里不在Foo模块的purview内
export module Foo;
// <- 在Foo模块的purview内
void world();
export void GetData();

```

### 1.3 Private module fragment

主模块接口单元可以用私有模块片段作为**后缀**，该部分只能出现在主模块接口单元中，如果存在，则它出现的模块单元必须是该模块的唯一单元。 其目的是将模块的接口和实现封装在单个翻译单元中，而不暴露实现细节。

例如：我想要创建一个Shape，计算其面积。

对外只需要暴露一个创建具体Shape的接口，调用共同的计算面积接口，于是我们可以写出如下模块。

```cpp
export module Shape;

export class Shape {
  public:
   virtual double CalculateArea() = 0;
};

export { std::shared_ptr<Shape> CreateRectangle(double length, double width); }


module :private; // here

class Rectangle : public Shape {
 private:
  double length;
  double width;

 public:
  Rectangle(double l, double w) : length(l), width(w) {}

  double CalculateArea() override { return length * width; }
};

std::shared_ptr<Shape> CreateRectangle(double length, double width) {
  return std::make_shared<Rectangle>(length, width);
}
```

将内部的细节全部放在private里面吗，我自己的g++版本是13，目前还不支持，会报如下错误：

gcc目前的支持情况，可以戳这里

> https://gcc.gnu.org/projects/cxx-status.html

```cpp
shape.cppm:14:1: sorry, unimplemented: private module fragment
   14 | module :private;
```

本地的clang是16版本，测试了一下是可以正常运行！

```cpp
➜  clang++ -std=c++20 shape.cppm --precompile -o shape.pcm  
➜  clang++ -std=c++20 shape.cc -fprebuilt-module-path=. shape.pcm -o shape
➜  ./shape 
area is 2
```

上面三个部分，全局和私有模块片段对于模块的存在来说不是必需的，purview是模块必需的部分。

## 2.模块使用

### 2.1 创建模块

创建模块类似于我们定义一个头文件，它也有一个文件，一般命名后缀是.cppm。我们只需要在这个文件中使用`export`与`module`关键字，后面跟上模块名，这样便创建一个可导出模块。例如：

```cpp
// foo.cppm
export module foo;

// main.cc
import foo;
```

`export`关键字可以用在类、变量等地方，通常有下面两种写法：

```cpp
export void func();

export {
  void func();
  constexpr double PI{3.14};
};
```

一种写法是export关键字放在常见声明前面，另外一种写法是导出块，类似于namespace写法，可以导出多个内容。

### 2.2 导出

这里就会涉及到一个重要问题，可以导出什么？

- variables, classes, structs, functions, namespaces, template functions/classes, concepts可以被导出
- 内部链接不可导出，如static变量与函数，匿名namespace。

```cpp
export static constexpr double PI = 3.14; // 不可导出
```

- 导出声明必须发生在命名空间级别

```cpp
namespace {
  export void print_no_export() { // 匿名命名空间，不可导出
    std::cout << "print no export" << std::endl;
  }
};
namespace light {
  export void print_export() { // ok
    std::cout << "print export" << std::endl;
  }
};

struct Foo {
  export int a; // 不能导出成员变量
};
```

- 导出命名空间会隐式导出其中的所有内容

```cpp
export class Rectangle {
 private:
  double length;
  double width;
 public:
  Rectangle(double l, double w) : length(l), width(w) {} 

  double CalculateArea() { return length * width; } // 隐式导出
};

export namespace {
  void print_export() { // 隐式导出
    std::cout << "print export" << std::endl;
  }
};

export {
  void func();  // 隐式导出
  constexpr double PI{3.14}; // 隐式导出
};
```

- 导出实体的第一个声明必须是导出声明。 后续声明和定义不需要有 export 关键字。

```cpp
export class Foo; // ok
export class Foo; // ok，只是会冗余

class Foo { // ok，隐式export
 public:
  void print() { std::cout << "this is foo" << std::endl; }
};

class Bar;  // not export
export class Bar; // 无效，第一个声明已经是不可导出，后续的不可导出


Foo f; // ok
f.print(); // ok
Bar b; // not ok
```

- 非导出模块不可导出

```cpp
module foo;

export void print { }  // error: 'export' may only occur after a module interface declaration
```

### 2.3 导入

与之对应的便是导入，导入也有一些规则，例如：

- 不可导入自身

- 在模块单元中，所有导入必须出现在该模块单元中的任何声明之前。 不能在模块单元中的任意点导入。

```cpp
void func() {}
import shape; // error: post-module-declaration imports must be contiguous

---------------
import shape; // ok
void func() {}
```

- 仅允许全局范围导入

```cpp
namespace light {
  import shape; // not ok
};
```

- 不允许循环导入

```cpp
// shape.cppm
export module shape;
import circle; // 循环导入

// circle.cppm
import shape;
```

### 2.4 模块中的include

`#include <iostream>`在模块中如何使用呢？

#### 2.4.1 purview区`#include`

使用import替换`#include`

```cpp
export module foo;
import <iostream>;
```

例如：

g++-13编译如下，可以通过`c++-system-header`后面跟iostream来编译出`gcm`。

```cpp
g++-13 -std=c++20 -fmodules-ts -x c++-system-header iostream 
g++-13 -fmodules-ts -std=c++20 -x c++ shape.cppm circle.cppm shape.cc
```

如果是自己的头文件，例如：`consts.h`，发现可以直接放在模块里面去`#include`，例如：

```cpp
export module foo;
#include "consts.h"
```

### 2.4.2 Global Module Fragment区`#inlcude`

对于`#include`以及宏都可以直接放在这个区使用，例如：

```cpp
module;
#ifdef
#include <iostream>
#endif
export module foo;
```

## 3.模块分解

当我们想将一个大模块分解成更小的模块时，我们可以使用以下两种方法：

- 模块分区。
  - 即允许我们将模块分解为多个文件。 但是，这对使用者来说实际上是不可见的，使用时正常导入模块即可。
- 子模块。
  - 即允许我们将较大的模块分解为任意数量的子模块的层次结构。使用者可以选择导入整个模块，或者只导入特定的子模块。

### 3.1 模块分区

语法：

```cpp
export module <module-name>:<part-name>;
import :<part-name>; // 可以在import前面添加export导出该分区接口
```

例如：我有一个shape，对外使用的时候只需要`import shape`，然后调用对应的接口即可，这里分别使用了circle分区与rectangle分区的接口。

```cpp
import shape;

int main() {
    DrawCircle();
    DrawRectangle();
    return 0;
}
```

shape是有两个分区，一个是circle，一个是rectangle，于是模块shape.cppm内容为：

```cpp
export module shape;

export import :circle;
export import :rectangle;
```

两个子分区内容为：

```cpp
// circle.cppm
module;
#include <iostream>
export module shape:circle;
export void DrawCircle() { std::cout << "draw circle" << std::endl; }

--------------
// rectangle.cppm

module;
#include <iostream>
export module shape:rectangle;
export void DrawRectangle() { std::cout << "draw rectangle" << std::endl; }
```

当我使用clang与g++编译后发现，clang-16编译报错，不支持。

```cpp
 error: sorry, module partitions are not yet supported
```

g++-13支持，需要注意编译的时候按照子分区->主分区的顺序进行编译，不然就会出错。

```cpp
➜ g++-13 -fmodules-ts -std=c++20 -x c++ shape.cppm circle.cppm rectangle.cppm shape.cc
shape:circle: error: failed to read compiled module: No such file or directory
shape:circle: note: compiled module file is 'gcm.cache/shape-circle.gcm'
shape:circle: note: imports must be built before being imported
shape:circle: fatal error: returning to the gate for a mechanical issue
```

应该改为：

```cpp
g++ -fmodules-ts -std=c++20 -x c++ circle.cppm rectangle.cppm shape.cppm shape.cc
```

### 3.2 子模块

语法：

```
export module <module-name>.<sub_module-name>;
import <module-name>.<sub_module-name>; // 可以在import前面添加export导出该分区接口
```

上面的例子也可以用子模块来实现，我们将shape依旧作为主模块，两个子模块分别是circle与rectangle。

```cpp
// shape.cppm
export module shape;

export import shape.circle;
export import shape.rectangle;

---------------------
// rectangle.cppm
module;
#include <iostream>
export module shape.rectangle;
export void DrawRectangle() { std::cout << "draw rectangle" << std::endl; }

---------------------
  
// circle.cppm
module;
#include <iostream>
export module shape.circle;
export void DrawCircle() { std::cout << "draw circle" << std::endl; }
```

可以看到在使用上模块分区用的是`:`，而子模块用的是`.`。

调用侧代码同模块分区。

不过它们在使用的时候有一些区别，例如：当子分区被引入时，使用其接口引发错误：internal compiler error: Segmentation fault: 11，而子模块是可以正常被引入使用。

```cpp
// 引入子分区
import shape:circle;

int main() {
    DrawCircle(); // error
    return 0;
}


// 引入子模块
import shape.circle;

int main() {
    DrawCircle(); // ok
    return 0;
}
```

对于子模块来说，在import时，不可以省略主模块名，上面在主分区中引入分区模块，我们可以使用`:circle`，这里不可以使用`.circle`。报错如下：

```cpp
shape.cppm:3:8: error: 'import' does not name a type
    3 | export import .circle;
```

## 4.接口与实现

通常在写代码时，我们会将代码拆分为"头文件"与"实现文件"，对于模块来说，这一操作不再需要。但是仍旧可以遵循以前的编写风格。

例如：绘制一个shape。

- 定义模块shape.cppm

```cpp
export module shape;

export class Shape {
 public:
  Shape();
  void Draw();
};
```

- 实现模块shape.cc

```cpp
import shape;
import <iostream>;

Shape::Shape() {}
void Shape::Draw() { std::cout << "draw a shape" << std::endl; }
```

跟平时使用`.h`与`.cpp`的分离模式基本类似。

> https://clang.llvm.org/docs/StandardCPlusPlusModules.html
>
> https://timsong-cpp.github.io/cppwp/n4861/module.import#7.sentence-2
>
> https://gcc.gnu.org/projects/cxx-status.html