# C++那些事

## 项目概要

学习C++内容，包括理论、实践、项目等

### 1.基础部分

- [const那些事](./basic_content/const)
- [static那些事](./basic_content/static)
- [this那些事](./basic_content/this)
- [inline那些事](./basic_content/inline)
- [sizeof那些事](./basic_content/sizeof)
- [函数指针那些事](./basic_content/func_pointer)
- [纯虚函数和抽象类那些事](./basic_content/abstract)
- [vptr_vtable那些事](./basic_content/vptr_vtable)
- [virtual那些事](./basic_content/virtual)
- [volatile那些事](./basic_content/volatile)
- [assert那些事](./basic_content/assert)
- [位域那些事](./basic_content/bit)
- [extern那些事](./basic_content/extern)
- [struct那些事](./basic_content/struct)
- [struct与class那些事](./basic_content/struct_class)
- [union那些事](./basic_content/union)
- [c实现c++多态那些事](./basic_content/c_poly)
- [explicit那些事](./basic_content/explicit)
- [friend那些事](./basic_content/friend)
- [using那些事](./basic_content/using)
- [::那些事](./basic_content/::)
- [enum那些事](./basic_content/enum)
- [decltype那些事](./basic_content/decltype)
- [引用与指针那些事](./basic_content/pointer_refer)
- [宏那些事](./basic_content/macro)

### 2.进阶部分 

#### 2.1 [effective_c++](./effective_c++)

正在更新...

#### 2.2 [C++2.0新特性](./c++2.0)

- [Variadic Templates](variadic)
- Spaces in Template Expressions

```cpp
vector<list<int> > //ok in each C++ version
vector<list<int>> // before c++ 11 error error: ‘>>’ should be ‘> >’ within a nested template argument list,c++11后可以正常通过
```

- [nullptr and nullptr_t](nullptr.cpp)
- [Automatic Type Deduction with auto](auto.cpp)
- [Uniform Initialization ](uniform_initialization.cpp)
- [initializer_list](initializer.cpp)
- [explicit for ctors taking more than one argument]( explicit.cpp)
- [range-based for statement](auto.cpp)

```cpp
for(decl:col) {
    statement
}
```

- [=default,=delete](default_delete.cpp)

  如果你自行定义了一个ctor,那么编译器就不会给你一个default ctor
  如果强制加上=default,就可以重新获得并使用default ctor.

- Alias(化名)Template(template typedef)

[alias.cpp](alias.cpp) 

[template_template.cpp](template_template.cpp)

- [template template parameter](template_template.cpp)
- [type alias](type_alias.cpp)
- [noexcept](noexcept.cpp)
- [override](override.cpp)
- [final](final.cpp)
- [decltype](decltype.cpp)
- [lambda](lambda.cpp)
- [Rvalue reference](rvalue.cpp)
- [move aware class](move.cpp)
- 容器-结构与分类

(1) 序列式容器包括：array(C++2.0新引入),vector,deque,list,forward_list(C++2.0新引入)

(2) 关联式容器包括：set/multiset,map/multimap

(3) 无序容器(C++2.0新引入,更换原先hash_xxx为unordered_xxx)包括：unordered_map/unordered_multimap,unordered_set/unordered_multiset

- [Hash Function](hash.cpp)
- [tuple](tuple.cpp)

学习资料：https://www.bilibili.com/video/av51863195?from=search&seid=3610634846288253061

#### 2.3 [C++并发编程v1](./concurrency_v1)

- [第一章](./concurrency_v1/chapter1)
- [第二章](./concurrency_v1/chapter2)

学习资料：https://chenxiaowei.gitbook.io/cpp_concurrency_in_action/

#### 2.4 [STL源码剖析](./stl_src)

**stl源码剖析：gcc4.9.1**

- [array](./stl_src/array.md)
- [deque](./stl_src/deque.md)
- [queue and stack](./stl_src/queue_stack.md)
- [list](./stl_src/list.md)
- [vector](./stl_src/vector.md)
- [typename](./stl_src/typename.md)
- [traits](./stl_src/traits.md)
- [iterator](./stl_src/iterator.md)
- [谈谈STL设计之EBO优化](./stl_src/谈谈STL设计之EBO优化.md)
- [rb_tree](./stl_src/rb_tree.md)
- [set and multiset](set_multiset.md)
- [map and multimap](./stl_src/map_multimap.md)
- [hashtable](./stl_src/hashtable.md)
- [myhashtable](./stl_src/myhashtable.md)
- [unordered_map](./stl_src/unordered_map.md)


### 3.代码运行

全部在Ubuntu18.04下用vim编写，使用gcc/g++调试！全部可正常运行！

## 关于作者：

个人公众号：

![](./img/wechat.jpg)

