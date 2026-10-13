> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-things/vector-resize-reverse/) 的文章，由站点同步脚本导出。

# C++ vector resize 与 reverse

在C++中，`resize`和`reserve`是用于容器（例如`std::vector`）的两个成员函数，用于管理vector的大小和内存分配。

例如：

```
std::vector<int> v1;
v1.resize(1000); //allocation + instance creation
cout <<(v1.size() == 1000)<< endl;   //prints 1
cout <<(v1.capacity()==1000)<< endl; //prints 1

std::vector<int> v2;
v2.reserve(1000); //only allocation
cout <<(v2.size() == 1000)<< endl;   //prints 0
cout <<(v2.capacity()==1000)<< endl; //prints 1
```

`resize`函数用于更改容器的大小，可以增加或减少容器中元素的数量。当增加容器的大小时，新添加的元素将会进行初始化。当减少容器的大小时，超出新大小的元素将会被移除。

`reserve`函数用于预分配容器的内存空间，但不改变容器的大小。它可以用于提前分配足够的内存，以避免频繁的重新分配内存操作，从而提高性能。但是，调用`reserve`并不会改变容器的大小，只是为容器分配足够的内存空间。

需要注意的是，`resize`和`reserve`函数在容器类型上的可用性有所不同。`resize`函数对于大多数容器类型是可用的，而`reserve`函数只对具有动态大小的容器类型有效，例如`std::vector`和`std::deque`。对于固定大小的容器类型（例如`std::array`），调用`reserve`没有任何效果。

从源码层面上来看：

```
void resize(size_type new_size, value_type value = value_type()) {
    if (new_size < size()) {
        erase(begin() + new_size, end()); // 移除超出新大小的元素
    } else {
        insert(end(), new_size - size(), value); // 添加新元素
    }
}
void reserve(size_type _n) {
    if (__n > max_size())
     __throw_length_error(__N("vector::reserve"));
   if (capacity() < __n)
     _M_reallocate(__n);
}
```

> 本节完