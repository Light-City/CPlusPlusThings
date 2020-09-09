# Stories About C Plus Plus

Thanks for all the support about << Stories About C ++ >>, right now you could open this link(https://light-city.club/sc/) to read this article.
Any star, retweet or pr will be weclomed.


-----------------------


### 0. Directory

- Chinese version：**C++ 那些事**
- English version：**Stories About C Plus Plus**

This repository meets the need of people who wants to really know about C++, and may help you in the interview. Besides, it also provides
other details,such as in-depth source code analysis and multi-threaded concurrency. It is a comprehensive C ++ repository from entry to advanced improvement

### 1.Foundation

- [Stories About const](./basic_content/const)
- [Stories About static](./basic_content/static)
- [Stories about this](./basic_content/this)
- [Stories About inline](./basic_content/inline)
- [Stories About sizeof](./basic_content/sizeof)
- [Stories About pointer of function](./basic_content/func_pointer)
- [Stories About pure virtual function and abstract](./basic_content/abstract)
- [Stories About vptr_vtable](./basic_content/vptr_vtable)
- [Stories About virtual](./basic_content/virtual)
- [Stories About volatile](./basic_content/volatile)
- [Stories About assert](./basic_content/assert)
- [Stories About bit](./basic_content/bit)
- [Stories About extern](./basic_content/extern)
- [Stories About struct](./basic_content/struct)
- [Stories About struct and class](./basic_content/struct_class)
- [Stories About union](./basic_content/union)
- [Stories About polymorphism](./basic_content/c_poly)
- [Stories About explicit](./basic_content/explicit)
- [Stories About friend](./basic_content/friend)
- [Stories About using](./basic_content/using)
- [Stories About :: ](./basic_content/maohao)
- [Stories About enum](./basic_content/enum)
- [Stories About decltype](./basic_content/decltype)
- [Stories About pointer_refer](./basic_content/pointer_refer)
- [Stories About macro](./basic_content/macro)

### 2.Code Samples

#### 2.1 10 Days Practice

- [x] day1

Basic Grammar About C ++
- [x] day2

Recursive、Structor、Enumerate、Static Variable

- [x] day3

Function

- [x] day4

Thorough About Function 

- [x] day5

Inheritance and Polymorphism 

- [x] day6

Vitrual Function、Abstruct 

- [x] day7

Operator overloading

- [x] day8

Template And STL

- [x] day9

Exception

- [x] day10

File And Stream

#### 2.2 Practical Exercises

├── [Square brackets overload .cpp](./practical_exercises/key_exercises/中括号重载.cpp)
├── [Clock++operator overloading.cpp](./practical_exercises/key_exercises/时钟++运算符重载.cpp)
├── [Mandatory conversion of operator overloading.cpp](./practical_exercises/key_exercises/运算符重载之强制转换.cpp)
└── [Clock with overloaded parenthesis.cpp](./practical_exercises/key_exercises/重载圆括号的时钟.cpp)

├── [Template of Function.cpp](./practical_exercises/key_exercises/函数模板.cpp)

├── [Dynamic array.cpp](./practical_exercises/key_exercises/动态数组.cpp)

├── [Dictionary insertion and search.cpp](./practical_exercises/key_exercises/字典插入与查找.cpp)

├── [Catch Exception.cpp](./practical_exercises/key_exercises/异常捕获.cpp)

├── [Template of Stack.cpp](./practical_exercises/key_exercises/类模板之栈.cpp)

├── [Template of Array.cpp](./practical_exercises/key_exercises/类模板特化之数组.cpp)

├── [Inheritance And Package.cpp](./practical_exercises/key_exercises/继承与封装.cpp)

├── [Read And Write files.cpp](./practical_exercises/key_exercises/读写文件综合题.cpp)
├── [Operator Overloading About Input and Output.cpp](./practical_exercises/key_exercises/输入输出运算符重载.cpp)
├── [Input And Output Overloading.cpp](./practical_exercises/key_exercises/输入输出重载.cpp)
├── [Input Format.cpp](./practical_exercises/key_exercises/输出格式.cpp)

### 2.[C++2.0 New Features](./c++2.0)

#### 2.0 Overview

C++2.0 is an Abbreviation, meaning「modern C++」，including C++11/14/17/20.

#### 2.1 [C++11 New Features](./c++2.0/c++11)

- [Variadic Templates](./c++2.0/variadic)

- Spaces in Template Expressions

  ```cpp
  vector<list<int> > //ok in each C++ version
  vector<list<int>> // before c++ 11 error error: ‘>>’ should be ‘> >’ within a nested template argument list,version beyond c++ 11 could   compile successfully
  ```

- [nullptr and nullptr_t](./c++2.0/nullptr.cpp)

- [Automatic Type Deduction with auto](./c++2.0/auto.cpp)

- [Uniform Initialization ](./c++2.0/uniform_initialization.cpp)

- [initializer_list](./c++2.0/initializer.cpp)

- [explicit for ctors taking more than one argument](./c++2.0/explicit.cpp)

- [range-based for statement](./c++2.0/auto.cpp)

  ```cpp
  for(decl:col) {
      statement
  }
  ```

- [=default,=delete](./c++2.0/default_delete.cpp)

  If you define a ctor by yourself, compiler won't compile the default ctor.
  If you add =default, you could recatch and use default ctor.

- Alias(化名)Template(template typedef)

  - [alias.cpp](./c++2.0/alias.cpp) 
  - [template_template.cpp](./c++2.0/template_template.cpp)

- [template template parameter](./c++2.0/template_template.cpp)

- [type alias](./c++2.0/type_alias.cpp)

- [noexcept](./c++2.0/noexcept.cpp)

- [override](./c++2.0/override.cpp)

- [final](./c++2.0/final.cpp)

- [decltype](./c++2.0/decltype.cpp)

- [lambda](./c++2.0/lambda.cpp)

- [Rvalue reference](./c++2.0/rvalue.cpp)

- [move aware class](./c++2.0/move.cpp)

- Container-Struct And Classify

  - (1) Serial containers include：`array`(C++2.0 Newly Introduced),`vector`,`deque`,`list`,`forward_list`(C++2.0 Newly Introduced)
  - (2) Associated containers include：`set/multiset`,`map/multimap`
  - (3) Unordered container(C++2.0 Newly Introduced,replace `hash_xxx` to `unordered_xxx`) include：`unordered_map/unordered_multimap,unordered_set/unordered_multiset`

- [Hash Function](./c++2.0/hash.cpp)

- [tuple](./c++2.0/tuple.cpp)

  Learning Material：https://www.bilibili.com/video/av51863195?from=search&seid=3610634846288253061

### 2.2 C++14/17/20

To Be Continued ...

### 3.Design Pattern

- [Singleton pattern](./design_pattern/singleton)

### 4. [STL Source Code Analysis](./src_analysis/stl)

**STL Source Code Analysis：gcc 4.9.1**

- [array](./src_analysis/stl/array.md)
- [deque](./src_analysis/stl/deque.md)
- [queue and stack](./src_analysis/stl/queue_stack.md)
- [list](./src_analysis/stl/list.md)
- [vector](./src_analysis/stl/vector.md)
- [typename](./src_analysis/stl/typename.md)
- [traits](./src_analysis/stl/traits.md)
- [iterator](./src_analysis/stl/iterator.md)
- [Talking about STL design and EBO optimization](./src_analysis/stl/谈谈STL设计之EBO优化.md)
- [rb_tree](./src_analysis/stl/rb_tree.md)
- [set and multiset](set_multiset.md)
- [map and multimap](./src_analysis/stl/map_multimap.md)
- [hashtable](./src_analysis/stl/hashtable.md)
- [myhashtable](./src_analysis/stl/myPhashtable.md)
- [unordered_map](./src_analysis/stl/unordered_map.md)

### 4. [Concurrent Programming](./concurrency)

#### 4.1 C++ Concurrency in Action

- [Chapter One](./concurrency/concurrency_v1/chapter1)
- [Chapter Two](./concurrency/concurrency_v1/chapter2)

Learning materials: https://chenxiaowei.gitbook.io/cpp_concurrency_in_action/

#### 4.2 Multithreading And Multiprocess

##### 4.2.1 Threading In C++

- [Introduction](./concurrency/Threading_In_CPlusPlus/1.thread)
- [Five Types of Thread Creation](./concurrency/Threading_In_CPlusPlus/2.create_type)
- [Join And Detachs](./concurrency/Threading_In_CPlusPlus/3.join_detach)
- [mutex in C++ Threading](./concurrency/Threading_In_CPlusPlus/4.mutex)

> From：
>
> https://www.youtube.com/watch?v=eZ8yKZo-PGw&list=PLk6CEY9XxSIAeK-EAh3hB4fgNvYkYmghp&index=4

### 5. [C++ Conventional method](./codingStyleIdioms)

##### What is your favorite custom for c ++ programming style?

- [1.ClassInitializers](./codingStyleIdioms/1_classInitializers)
- [2.Replace Enumclass with Namespace](./codingStyleIdioms/2_enumclass_namespace)
- [3.RAII(Resource Acquisition Initialization)](./codingStyleIdioms/3_RAII)
- [4.Copy and Swap](./codingStyleIdioms/4_copy-swap)
- [5.pImpl(Pointer Implement)](./codingStyleIdioms/5_pImpl)

### 6.Learning Courses

#### 6.1 [Chinese Name:极客时间《现代 C++ 实战 30 讲》](https://time.geekbang.org/channel/home)

- [heap、stack、RAII：How to manage resources for C ++ ?](./learn_class/modern_C++_30/RAII)
  - [heap](./modern_++_30/RAII/heap.cpp)
  - [stack](./learn_class/modern_C++_30/RAII/stack.cpp)
  - [RAII](./learn_class/modern_C++_30/RAII/RAII.cpp)
- [Implementing smart pointers for C ++](./learn_class/modern_C++_30/smart_ptr)
  - [auto_ptr、scope_ptr](./learn_class/modern_C++_30/smart_ptr/auto_scope.cpp)
  - [unique_ptr](./learn_class/modern_C++_30/smart_ptr/unique_ptr.cpp)
  - [shared_ptr](./learn_class/modern_C++_30/smart_ptr/shared_ptr.cpp)
- [What exactly does r value and move solve？](./learn_class/modern_C++_30/reference)
  - [L value and R value](./learn_class/modern_C++_30/reference/reference.cpp)
  - [Extend the declaration cycle](./learn_class/modern_C++_30/reference/lifetime.cpp)
  - [Reference folding](./learn_class/modern_C++_30/reference/collapses.cpp)
  - [Perfect forward](./learn_class/modern_C++_30/reference/forward.cpp)
  - [Do not return Reference](./learn_class/modern_C++_30/reference/don'treturnReference.cpp)
- [Container 1](./learn_class/modern_C++_30/container1)
- [Container 2](./learn_class/modern_C++_30/container2)
- [Exception](./learn_class/modern_C++_30/exception)
- [Literal、Static Assertion And Member Function Specifier](./learn_class/modern_C++_30/literalAssert)
- [Return Object？](./learn_class/modern_C++_30/returnObj)c
- [Getting started with generic programming and templates](./learn_class/modern_C++_30/compilerpoly)
- [A whole Compiler Compute World](./learn_class/modern_C++_30/compilercompute)
- [SFINAE：What is it if it is not replace error?](./learn_class/modern_C++_30/SFINAE)
- [constexpr：A Normal World](./learn_class/modern_C++_30/constexpr)
- [Function object and Lambda：functionLambda](./learn_class/modern_C++_30/functionLambda)
- [Memory Model and Atomic：Understangding the complexity of concurrency](./learn_class/modern_C++_30/memorymodel_atomic)

### 7.Tools

#### 7.1 [Container shortcut output tool](./tool/output)

Modified the code, [Click here for the code](./tool/output/output_container.h)

Input：

```cpp
map<int, int> mp{
            {1, 1},
            {2, 4},
            {3, 9}};
    cout << mp << endl;
```

Output：

```cpp
{ 1 => 1, 2 => 4, 3 => 9 }
```

#### 7.2 Output Like Python（Jupyter Notebook）

- [How to output like python in C/C++](./tool/像Python一样玩CC++.md)

#### 7.3 Observe the changes in the compilation process

-  [https://cppinsights.io](https://cppinsights.io/)

#### 7.4 Debug Tools For C ++：dbg-macro

- [Debug Tool: dbg-macro](./tool/C++的Debug工具dbg-macro.md)

### 8.Expansion

#### 8.1 Other Problems
- [How to convert string to in C ++？](./extension/some_problem/string_int.md)


### 9.Circumstance


- **Running Circumstance**

  Ubuntu 18.04

- **IDE**

  CLion  gcc/g++

### 10.Contributor



| 贡献人  | 地址                          |
| ------- | ----------------------------- |
| 光城    | https://github.com/Light-City |
| ChungZH | https://github.com/ChungZH    |
| xliu79  | https://github.com/xliu79     |



### 11.About The Writer

个人公众号：

![](./img/wechat.jpg)
