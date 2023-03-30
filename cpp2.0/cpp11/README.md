# C++新特性

## 1.C++11新特性

- [Variadic Templates](./variadic)

- Spaces in Template Expressions

  ```cpp
  vector<list<int> > //ok in each C++ version
  vector<list<int>> // before c++ 11 error error: ‘>>’ should be ‘> >’ within a nested template argument list,c++11后可以正常通过
  ```

- [nullptr and nullptr_t](./nullptr.cpp)

- [Automatic Type Deduction with auto](./auto.cpp)

- [Uniform Initialization ](./uniform_initialization.cpp)

- [initializer_list](./initializer.cpp)

- [explicit for ctors taking more than one argument](./explicit.cpp)

- [range-based for statement](./auto.cpp)

  ```cpp
  for(decl:col) {
      statement
  }
  ```

- [=default,=delete](./default_delete.cpp)

  如果你自行定义了一个ctor,那么编译器就不会给你一个default ctor
  如果强制加上=default,就可以重新获得并使用default ctor.

- Alias(化名)Template(template typedef)

  - [alias.cpp](./alias.cpp) 
  - [template_template.cpp](./template_template.cpp)

- [template template parameter](./template_template.cpp)

- [type alias](./type_alias.cpp)

- [noexcept](./noexcept.cpp)

- [override](./override.cpp)

- [final](./final.cpp)

- [decltype](./decltype.cpp)

- [lambda](./lambda.cpp)

- [Rvalue reference](./rvalue.cpp)

- [move aware class](./move.cpp)

- 容器-结构与分类

  - (1) 序列式容器包括：array(C++2.0新引入),vector,deque,list,forward_list(C++2.0新引入)
  - (2) 关联式容器包括：set/multiset,map/multimap
  - (3) 无序容器(C++2.0新引入,更换原先hash_xxx为unordered_xxx)包括：unordered_map/unordered_multimap,unordered_set/unordered_multiset

- [Hash Function](./hash.cpp)

- [tuple](./tuple.cpp)

  学习资料：https://www.bilibili.com/video/av51863195?from=search&seid=3610634846288253061