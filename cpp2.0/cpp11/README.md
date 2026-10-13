> 本文档由 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp11/cpp11/) 同步生成，网站版本含多语言与更新排版。

# C++11 新特性概览

## 1.C++11新特性

- [Variadic Templates](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/variadic)

- Spaces in Template Expressions

  ```cpp
  vector<list<int> > //ok in each C++ version
  vector<list<int>> // before c++ 11 error error: ‘>>’ should be ‘> >’ within a nested template argument list,c++11后可以正常通过
  ```

- [nullptr and nullptr_t](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/nullptr.cpp)

- [Automatic Type Deduction with auto](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/auto.cpp)

- [Uniform Initialization ](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/uniform_initialization.cpp)

- [initializer_list](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/initializer.cpp)

- [explicit for ctors taking more than one argument](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/explicit.cpp)

- [range-based for statement](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/auto.cpp)

  ```cpp
  for(decl:col) {
      statement
  }
  ```

- [=default,=delete](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/default_delete.cpp)

  如果你自行定义了一个ctor,那么编译器就不会给你一个default ctor
  如果强制加上=default,就可以重新获得并使用default ctor.

- Alias(化名)Template(template typedef)

  - [alias.cpp](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/alias.cpp) 
  - [template_template.cpp](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/template_template.cpp)

- [template template parameter](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/template_template.cpp)

- [type alias](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/type_alias.cpp)

- [noexcept](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/noexcept.cpp)

- [override](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/override.cpp)

- [final](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/final.cpp)

- [decltype](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/decltype.cpp)

- [lambda](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/lambda.cpp)

- [Rvalue reference](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/rvalue.cpp)

- [move aware class](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/move.cpp)

- 容器-结构与分类

  - (1) 序列式容器包括：array(C++2.0新引入),vector,deque,list,forward_list(C++2.0新引入)
  - (2) 关联式容器包括：set/multiset,map/multimap
  - (3) 无序容器(C++2.0新引入,更换原先hash_xxx为unordered_xxx)包括：unordered_map/unordered_multimap,unordered_set/unordered_multiset

- [Hash Function](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/hash.cpp)

- [tuple](https://github.com/Light-City/CPlusPlusThings/tree/master/cpp2.0/cpp11/tuple.cpp)

  学习资料：https://www.bilibili.com/video/av51863195?from=search&seid=3610634846288253061