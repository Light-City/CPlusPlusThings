# C++新特性

## 1.[C++11新特性](./c++2.0/c++11)

- [Variadic Templates](./c++2.0/variadic)

- Spaces in Template Expressions

  ```cpp
  vector<list<int> > //ok in each C++ version
  vector<list<int>> // before c++ 11 error error: ‘>>’ should be ‘> >’ within a nested template argument list,c++11后可以正常通过
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

  如果你自行定义了一个ctor,那么编译器就不会给你一个default ctor
  如果强制加上=default,就可以重新获得并使用default ctor.

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

- 容器-结构与分类

  - (1) 序列式容器包括：array(C++2.0新引入),vector,deque,list,forward_list(C++2.0新引入)
  - (2) 关联式容器包括：set/multiset,map/multimap
  - (3) 无序容器(C++2.0新引入,更换原先hash_xxx为unordered_xxx)包括：unordered_map/unordered_multimap,unordered_set/unordered_multiset

- [Hash Function](./c++2.0/hash.cpp)

- [tuple](./c++2.0/tuple.cpp)

  学习资料：https://www.bilibili.com/video/av51863195?from=search&seid=3610634846288253061