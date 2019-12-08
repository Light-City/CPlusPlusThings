# C++ STL源码剖析之双向环形链表list

## 0. 导语

源码对应的版本为**gcc-4.9.1**

### 1.list

list为双向环形链表，其结构为：

![](https://raw.githubusercontent.com/Light-City/cloudimg/master/list_1.png)

自己绘制的图如下：

![list_all](https://raw.githubusercontent.com/Light-City/cloudimg/master/list_a.png)

双向环状链表从节点值为3开始插入，红色框表示最后一个节点(end()指向的节点)。黄色线条表示指向前驱节点，黑色线条表示指向后继节点。

### 1.1 list源码

#### 1.1.1 类结构

```c++
 template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
 class list : protected _List_base<_Tp, _Alloc> 
 {
        
 }
```

`list`继承`_List_base`。

#### 1.1.2 双向环形链表实现

【**构造函数**】

**（1）不带任何元素的list**

```c++
explicit
list(const allocator_type &__a) _GLIBCXX_NOEXCEPT: _Base(_Node_alloc_type(__a)) {}
```

**（2）带n个元素且赋予初值的list**

```c++
explicit list(size_type __n, const value_type &__value = value_type(),const allocator_type &__a = allocator_type()) : _Base(_Node_alloc_type(__a)) 
{ _M_fill_initialize(__n, __value); }
```

**（3）从一个范围中进行初始化list**

```c++
template<typename _InputIterator>
list(_InputIterator __first, _InputIterator __last,
     const allocator_type &__a = allocator_type())
        : _Base(_Node_alloc_type(__a)) {
    // Check whether it's an integral type.  If so, it's not an iterator.
    typedef typename std::__is_integer<_InputIterator>::__type _Integral;
    _M_initialize_dispatch(__first, __last, _Integral());
}
```

【**创建节点**】

做的事情：创建一个新的节点并动态分配内存，返回节点。

```c++
_Node *_M_create_node(const value_type &__x) {
    _Node *__p = this->_M_get_node();
    __try
    {
        _M_get_Tp_allocator().construct
                (std::__addressof(__p->_M_data), __x);
    }
    __catch(...)
    {
        _M_put_node(__p);
        __throw_exception_again;
    }
    return __p;
}
```

注意到里面有两个重要的函数`_M_get_node`与`_M_put_node`，我们来查看后发现这些方法来自基类，源码为：

```c++
_List_node<_Tp> * _M_get_node() { return _M_impl._Node_alloc_type::allocate(1); }

void _M_put_node(_List_node<_Tp> *__p)   _GLIBCXX_NOEXCEPT
{ _M_impl._Node_alloc_type::deallocate(__p, 1); }
```

对应的就是创建节点动态分配内存，若创建过程中抛出异常，则释放内存。

【**插入节点**】

插入节点包括：

- 尾部插入n个指定节点值的节点，对应的函数`_M_fill_initialize`

在list的构造函数中使用：

```c++
explicit list(size_type __n, const value_type &__value = value_type(),const allocator_type &__a = allocator_type()) : _Base(_Node_alloc_type(__a)) 
{ _M_fill_initialize(__n, __value); }
```

- 指定位置插入指定节点值的节点，对应的函数`_M_insert`

其中大家经常使用的`push_back`与`push_front`底层就是调用`_M_insert`函数。

两者函数区别是：

```c++
this->_M_insert(end(), __x);  // push_back   尾部插入  
this->_M_insert(begin(), __x); // push_front 头部插入
```

- **双向环形链表插入函数**`_M_hook`   **(最重要！！！)**

像前面提到的`push_back`、`push_front`、`_M_insert`，还有`insert`都是使用最基础的双向链表插入函数`_M_hook`实现的。

**下面来深入研究一下：**

其中`_M_fill_initialize`源码如下：

```c++
void _M_fill_initialize(size_type __n, const value_type &__x) {
    for (; __n; --__n)
        push_back(__x);
}
```

其中`push_back`源码如下：

```c++
void push_back(const value_type &__x) { this->_M_insert(end(), __x); }
```

其中`_M_insert`，在指定的位置插入初始值为x的节点。

```c++
void _M_insert(iterator __position, const value_type &__x) {
    _Node *__tmp = _M_create_node(__x);
    __tmp->_M_hook(__position._M_node);
}
```

其中`_M_hook`实现在`gcc-4.9.1/libstdc++-v3/src/c++98/list.cc`中,当然`_List_node_base`的其他函数，例如：`_M_unhook`也在这个文件中。

```c++
// 在指定的位置前插入this指向的节点
void_List_node_base::_M_hook(_List_node_base* const __position) _GLIBCXX_USE_NOEXCEPT
{
  this->_M_next = __position;		
  this->_M_prev = __position->_M_prev;
  __position->_M_prev->_M_next = this;
  __position->_M_prev = this;
}
```

所以上述细分为两个函数：我们把上述代码进行总结：

（1）在指定的位置插入初始值为x的节点

```c++
void _M_insert(iterator __position, const value_type &__x) {
    _Node *__tmp = _M_create_node(__x);
    __tmp->_M_next = __position;			// 第一步
    __tmp->_M_prev = __position->_M_prev;	// 第二步
    __position->_M_prev->_M_next = __tmp;	// 第三步
    __position->_M_prev = __tmp;			// 第四步
}
```

这种插入是在指定位置前插入，(对应到代码就是)例如实现在指定`__position`节点为7前插入节点值9的节点(对应到代码就是__tmp)，下面阐述了具体的插入流程。

![list_insert](https://raw.githubusercontent.com/Light-City/cloudimg/master/list_insert.png)

（2）在末尾依次插入n个节点值为x的节点

```c++
void _M_fill_initialize(size_type __n, const value_type &__x) {
	for (; __n; --__n)
		_M_insert(end(), __x);
}
```

对于上述的代码大家或许会联想到`insert`,它有三个。`insert`实现文件在`libstdc++-v3/include/bits/list.tcc`。

- **第一：在指定迭代器之前插入指定元素值节点。**

实现是调用前面的`_M_hook`函数。

```c++
template<typename _Tp, typename _Alloc>
typename list<_Tp, _Alloc>::iterator
list<_Tp, _Alloc>::
#if __cplusplus >= 201103L
insert(const_iterator __position, const value_type& __x)
#else
insert(iterator __position, const value_type& __x)
#endif
{
  _Node* __tmp = _M_create_node(__x);
  __tmp->_M_hook(__position._M_const_cast()._M_node);
  return iterator(__tmp);
}
```

- **第二：在指定迭代器之前插入n个指定节点值的节点。**

```c++
void insert(iterator __position, size_type __n, const value_type &__x) {
    list __tmp(__n, __x, get_allocator());
    splice(__position, __tmp);
}
```

实现是先调用list构造函数，完成创建一个拥有n个指定节点值的list。

```c++
explicit list(size_type __n, const value_type &__value = value_type(),const allocator_type &__a = allocator_type()) : _Base(_Node_alloc_type(__a)) 
{ _M_fill_initialize(__n, __value); }
```

然后使用`splice`函数完成从另一个list中来插入当前list。

```c++
void splice(iterator __position, list &__x)
{
    if (!__x.empty()) {
        _M_check_equal_allocators(__x);

        this->_M_transfer(__position._M_const_cast(),
                          __x.begin(), __x.end());
    }
}
```

其中`_M_transfer`追踪代码，可以知道：

```c++
// Moves the elements from [first,last) before position.
void
_M_transfer(iterator __position, iterator __first, iterator __last) {
    __position._M_node->_M_transfer(__first._M_node, __last._M_node);
}
```

再次分析得到其来自list的基类`_List_node_base`，而`_M_transfer`实现文件在`gcc-4.9.1/libstdc++-v3/src/c++98/list.cc`中：

```c++
void _List_node_base::
_M_transfer(_List_node_base * const __first,
    _List_node_base * const __last) _GLIBCXX_USE_NOEXCEPT
{
  if (this != __last)
  {
      // Remove [first, last) from its old position.
      __last->_M_prev->_M_next  = this;
      __first->_M_prev->_M_next = __last;
      this->_M_prev->_M_next    = __first;

      // Splice [first, last) into its new position.
      _List_node_base* const __tmp = this->_M_prev;
      this->_M_prev                = __last->_M_prev;
      __last->_M_prev              = __first->_M_prev;
      __first->_M_prev             = __tmp;
  }
}
```

仍然是上述的图：

![list_all](https://raw.githubusercontent.com/Light-City/cloudimg/master/list_a.png)

经过前面分析，我们知道`splice`是将上述图的所代表的整个list插入指定迭代器前面，例如，我们想要在下面两个节点前面插入，具体图形步骤如下：

`this`代表的节点为值为8的节点，下图描述的就是在节点10与节点8中间插入整个list。

`__last`代表的是红色框节点，该节点为`end()`指向的节点，我们是不需要该节点的，所以在后面处理中，会把该节点从整个list中去除掉。

`__first`代表的是图中节点值为3的节点。

**第一步：先将最后一个有效节点，也就是红色框的前一个节点的next指针指向指定的节点8。**

![](https://raw.githubusercontent.com/Light-City/cloudimg/master/step1.png)

对应代码为：

```
__last->_M_prev->_M_next  = this;	
```

**第二步：`_last`的next指针指向自己。**

![](https://raw.githubusercontent.com/Light-City/cloudimg/master/step2.png)

对应的代码为：

```
__first->_M_prev->_M_next = __last;
```

**第三步：让指定迭代器之前的节点的nex指向原先list的第一个节点(`__first`)。**

![](https://raw.githubusercontent.com/Light-City/cloudimg/master/step3.png)

对应的代码为：

```
this->_M_prev->_M_next    = __first;
```

**第四步：保存指定迭代器的前驱节点(对应到哪图中的节点值为10的节点)。**

```
_List_node_base* const __tmp = this->_M_prev;
```

**第五步：指定迭代器的前驱节点指向原list中实际最后一个节点(end()前一节点)。**

![](https://raw.githubusercontent.com/Light-City/cloudimg/master/step5.png)

对应的代码为：

```
this->_M_prev                = __last->_M_prev;
```

**第六步：让原list的最后一个节点(end()指向的节点)的prev指向自己。**

![](https://raw.githubusercontent.com/Light-City/cloudimg/master/step6.png)

对应的代码为：

```
__last->_M_prev              = __first->_M_prev;
```

**第七步：让原list第一个节点的prev指向第四步保存的节点。**

![](https://raw.githubusercontent.com/Light-City/cloudimg/master/step7.png)

对应的代码为：

```
__first->_M_prev             = __tmp;
```

这样经过以上七步操作，完成了在节点8与节点10之前插入一个list。

- **第三：从一个list范围把数据插入到指定迭代器前面。**

```c++
template<typename _InputIterator>
void
insert(iterator __position, _InputIterator __first,
       _InputIterator __last) {
    list __tmp(__first, __last, get_allocator());
    splice(__position, __tmp);
}
```

原理同上，只不过这个`__tmp`是调用另外一个构造函数。

【**删除节点**】

- 删除指定节点

删除指定节点分为两个，第一个通过迭代器删除，第二个是通过元素值删除。

**（1）通过迭代器删除，对应函数为erase**

其中`pop_front`与`pop_back`、`erase`、`remove`底层实现基于`_M_erase`函数。

```c++
this->_M_erase(begin()); 	// pop_front 不断删除起点的元素
this->_M_erase(iterator(this->_M_impl._M_node._M_prev)); // pop_back移除最后一个元素
```

在`libstdc++-v3/include/bits/list.tcc`中：

```c++
erase(iterator __position)
#endif
{
  iterator __ret = iterator(__position._M_node->_M_next);
  _M_erase(__position._M_const_cast());
  return __ret;
}
```

**（2）通过元素值删除，对应函数为remove**

特殊情况处理，当删除元素的地址与迭代器的地址一样的时候，先保存起来，最后判断保存的迭代器是不是end()，如果不是，就删除掉。底层仍旧是通过`_M_erase`删除。

```c++
template<typename _Tp, typename _Alloc>
void list<_Tp, _Alloc>::remove(const value_type& __value)
{
  iterator __first = begin();
  iterator __last = end();
  iterator __extra = __last;
  while (__first != __last)
  {
      iterator __next = __first;
      ++__next;
      if (*__first == __value)
        {
          // _GLIBCXX_RESOLVE_LIB_DEFECTS
          // 526. Is it undefined if a function in the standard changes
          // in parameters?
          if (std::__addressof(*__first) != std::__addressof(__value))
        _M_erase(__first);
          else
        __extra = __first;
        }
      __first = __next;
  }
  if (__extra != __last)
	_M_erase(__extra);
}
```

除了这个remove外，还有一个是`remove_if`，根据条件来删除。

```c++
template<typename _Tp, typename _Alloc>
template <typename _Predicate>
void list<_Tp, _Alloc>::
remove_if(_Predicate __pred)
{
    iterator __first = begin();
    iterator __last = end();
	while (__first != __last)
    {
        iterator __next = __first;
        ++__next;
        if (__pred(*__first))
          _M_erase(__first);
        __first = __next;
    }
}

```

对上述的`remove`的if去掉，在里面添加一个判断即可。

使用如下：

```c++
bool isone(int one) {
    return one==2;
}
int main() {
    list<int> t;
    t={3,4,0,2,0,10,10};
    for(auto i:t) cout<<i<<" ";	 // 3 4 0 2 0 10 10 
    cout<<endl;
    t.remove_if(isone);
    for(auto i:t) cout<<i<<" ";  // 3 4 0 0 10 10 
    cout<<endl;
}
```

- 删除一系列节点
- 删除所有节点，对应的函数`clear`

**（1）删除指定节点详细分析**

```c++
_M_erase(iterator __position)
_GLIBCXX_NOEXCEPT
{
    __position._M_node->_M_unhook();
    _Node *__n = static_cast<_Node *>(__position._M_node);
#if __cplusplus >= 201103L
    _M_get_Node_allocator().destroy(__n);
#else
    _M_get_Tp_allocator().destroy(std::__addressof(__n->_M_data));
#endif
    _M_put_node(__n);		// 释放内存
}

```

其中`_M_unhook`实现在`gcc-4.9.1/libstdc++-v3/src/c++98/list.cc`，实现如下：

```c++
void _List_node_base::_M_unhook() _GLIBCXX_USE_NOEXCEPT
{
  _List_node_base* const __next_node = this->_M_next;	// 第一步：保存后继节点
  _List_node_base* const __prev_node = this->_M_prev;	// 第二步：保存前驱节点
  __prev_node->_M_next = __next_node;					// 第三步：前驱节点的next指向后继节点
  __next_node->_M_prev = __prev_node;					// 第四步：后继节点的prev指向前驱节点
}
```

例如：删除节点值为9的节点，第三与第四步图解：

![list_erase](https://raw.githubusercontent.com/Light-City/cloudimg/master/list_erase.png)

**（2）删除一系列元素详细分析**

```c++
iterator
#if __cplusplus >= 201103L
erase(const_iterator __first, const_iterator __last) noexcept
#else
erase(iterator __first, iterator __last)
#endif
{
    while (__first != __last)
        __first = erase(__first);
    return __last._M_const_cast();
}
```

使用erase，删除给定迭代器范围内的数据。

**（3）删除所有元素详细分析**

清空元素，并初始化，回到list默认状态。

```c++
void clear()
_GLIBCXX_NOEXCEPT
{
    _Base::_M_clear();
    _Base::_M_init();
}
```

其中`_M_clear`实现在：`libstdc++-v3/include/bits/list.tcc`中：

```c++
_List_base<_Tp, _Alloc>::
_M_clear() _GLIBCXX_NOEXCEPT
{
  typedef _List_node<_Tp>  _Node;
  _Node* __cur = static_cast<_Node*>(_M_impl._M_node._M_next);
  while (__cur != &_M_impl._M_node)
    {
      _Node* __tmp = __cur;			// 保存节点
      __cur = static_cast<_Node*>(__cur->_M_next);	// 往后遍历
    #if __cplusplus >= 201103L
      _M_get_Node_allocator().destroy(__tmp);
    #else
      _M_get_Tp_allocator().destroy(std::__addressof(__tmp->_M_data));
    #endif
      _M_put_node(__tmp);	// 释放内存
    }
}
```

`_M_init`实现，全部指向自己即可。

```c++
void _M_init()
_GLIBCXX_NOEXCEPT
{
    this->_M_impl._M_node._M_next = &this->_M_impl._M_node;
    this->_M_impl._M_node._M_prev = &this->_M_impl._M_node;
}
```

【**元素访问**】

每个都实现了两个版本：引用与常引用。

- front 返回第一个元素

```c++
reference front()
_GLIBCXX_NOEXCEPT
{ return *begin(); }
const_reference
front() const
_GLIBCXX_NOEXCEPT
{ return *begin(); }
```

- 返回最后一个元素

```c++
reference
back()
_GLIBCXX_NOEXCEPT
{
    iterator __tmp = end();
    --__tmp;
    return *__tmp;
}
const_reference
back() const
_GLIBCXX_NOEXCEPT
{
    const_iterator __tmp = end();
    --__tmp;
    return *__tmp;
}
```

【**算法**】

- unique

从容器中的每个连续的相等元素组中除去除第一个元素外的所有元素。

请注意，只有与[列表](http://www.cplusplus.com/list)容器紧邻的元素相比，该元素才从[列表](http://www.cplusplus.com/list)容器中删除。因此，此功能对于排序列表特别有用。

```c++
template<typename _Tp, typename _Alloc>
template <typename _BinaryPredicate>
  void
  list<_Tp, _Alloc>::
  unique(_BinaryPredicate __binary_pred)
  {
    iterator __first = begin();
    iterator __last = end();
    if (__first == __last)
  return;
    iterator __next = __first;
    while (++__next != __last)
  {
    // 满足条件就删除
    if (__binary_pred(*__first, *__next))
       // 删除
      _M_erase(__next);
    else
      __first = __next;
    __next = __first;
  }
  }
```

举例如下：

```c++
// list::unique
#include <iostream>
#include <cmath>
#include <list>

// a binary predicate implemented as a function:
bool same_integral_part (double first, double second)
{ return ( int(first)==int(second) ); }

// a binary predicate implemented as a class:
struct is_near {
  bool operator() (double first, double second)
  { return (fabs(first-second)<5.0); }
};

int main ()
{
  double mydoubles[]={ 12.15,  2.72, 73.0,  12.77,  3.14,
                       12.77, 73.35, 72.25, 15.3,  72.25 };
  std::list<double> mylist (mydoubles,mydoubles+10);
  
  mylist.sort();             //  2.72,  3.14, 12.15, 12.77, 12.77,
                             // 15.3,  72.25, 72.25, 73.0,  73.35

  mylist.unique();           //  2.72,  3.14, 12.15, 12.77
                             // 15.3,  72.25, 73.0,  73.35

  mylist.unique (same_integral_part);  //  2.72,  3.14, 12.15
                                       // 15.3,  72.25, 73.0

  mylist.unique (is_near());           //  2.72, 12.15, 72.25

  std::cout << "mylist contains:";
  for (std::list<double>::iterator it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
```

上述排序后会删除所有的重复元素，只剩下一个，而不排序只会删除重复且连续的元素。

- merge

merge源码实现采用前面`_M_transfer`函数，假设现在有两个list，分别是list1与list2。list1中的元素与list2中的元素进行比较，如果list1中元素值小于list2中元素值，则对list1迭代器++，不进行任何操作，而如果list1中的元素值大于list2中的元素值，那么每次将list2这个小的元素对应的迭代器塞入`_M_transfer`函数中，通过这个函数完成向list1中刚才比较的迭代器前面插入list2较小的元素，那么最后所有元素都会被插入到list1中。

当list1已经遍历完毕，而list2还没有遍历完毕，那么只需要执行一次`_M_transfer`，将list2链表从当前迭代器开始到最后的end插入到list1的末尾即可。

```c++
template<typename _Tp, typename _Alloc>
void
list<_Tp, _Alloc>::
#if __cplusplus >= 201103L
merge(list&& __x)
#else
merge(list& __x)
#endif
{
  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 300. list::merge() specification incomplete
  if (this != &__x)
  {
      _M_check_equal_allocators(__x);

      iterator __first1 = begin();
      iterator __last1 = end();
      iterator __first2 = __x.begin();
      iterator __last2 = __x.end();
      while (__first1 != __last1 && __first2 != __last2)
        if (*__first2 < *__first1)
         {
            iterator __next = __first2;
            _M_transfer(__first1, __first2, ++__next);
            __first2 = __next;
         }
        else
          ++__first1;
      if (__first2 != __last2)
        _M_transfer(__last1, __first2, __last2);
   }
}
```

使用：

```c++
int main() {
 	list<int> l1 = {2,3,5,7};
    list<int> l2 = {1,10,9,5};
    l1.sort();
    l2.sort();
    l1.merge(l2);
    for(auto i:l1) cout<<i<<" ";	// 1 2 3 5 5 7 9 10
    return 0;
}
```

- sort

由于STL本身的排序算法sort接受的输入迭代器是随机访问迭代器，但是双向list链表容器的访问方式是双向迭代器，因此，不能使用STL本身的排序算法sort，必须自己定义属于自己访问的排序算法。我们从源码的剖析中，可以看到该排序算法思想类似于归并排序。

sort中splice调用的是：

```c++
void splice(const_iterator __position, list& __x, const_iterator __i) noexcept
{ splice(__position, std::move(__x), __i); }
```

进一步分析：

```c++
void
splice(iterator __position, list &__x, iterator __i)
{
    iterator __j = __i._M_const_cast();
    ++__j;
    if (__position == __i || __position == __j)
        return;

    if (this != &__x)
        _M_check_equal_allocators(__x);

    this->(__position._M_const_cast(),
                      __i._M_const_cast(), __j);
}
```

最后调用的是`_M_transfer`。

在sort中还有一个函数`swap`，完成两个链表交换，实现代码在`gcc-4.9.1/libstdc++-v3/src/c++98/list.cc`中：

```c++
void
_List_node_base::swap(_List_node_base& __x,
          _List_node_base& __y) _GLIBCXX_USE_NOEXCEPT
{
      if ( __x._M_next != &__x )
    {
      if ( __y._M_next != &__y )
        {
          // Both __x and __y are not empty.
          std::swap(__x._M_next,__y._M_next);
          std::swap(__x._M_prev,__y._M_prev);
          __x._M_next->_M_prev = __x._M_prev->_M_next = &__x;
          __y._M_next->_M_prev = __y._M_prev->_M_next = &__y;
        }
      else
        {
              // __x is not empty, __y is empty.
              __y._M_next = __x._M_next;
              __y._M_prev = __x._M_prev;
              __y._M_next->_M_prev = __y._M_prev->_M_next = &__y;
              __x._M_next = __x._M_prev = &__x;
            }
        }
      else if ( __y._M_next != &__y )
        {
          // __x is empty, __y is not empty.
          __x._M_next = __y._M_next;
          __x._M_prev = __y._M_prev;
          __x._M_next->_M_prev = __x._M_prev->_M_next = &__x;
          __y._M_next = __y._M_prev = &__y;
        }
}
```

具体的实现思路是，判断两个链表为空还是不为空，然后修改next指针与prev指针。

下面来看看强大的sort：

```c++
template<typename _Tp, typename _Alloc>
void
list<_Tp, _Alloc>::
sort() {
    // Do nothing if the list has length 0 or 1.
    if (this->_M_impl._M_node._M_next != &this->_M_impl._M_node
        && this->_M_impl._M_node._M_next->_M_next != &this->_M_impl._M_node) {
        list __carry; // 辅助链表，用于从a中提取元素以及临时保存两个链表的合并结果
        list __tmp[64]; // 保存着当前每一个归并层次的结果, i号链表保存的元素个数为2的i次方或者0
        list *__fill = &__tmp[0]; // 表示当前最大归并排序的层次，while循环之后__fill变成log2(list.size())
        list *__counter;

        do {
            __carry.splice(__carry.begin(), *this, begin()); //把当前链表的第一个节点放在carry链表头

            for (__counter = &__tmp[0];
                 __counter != __fill && !__counter->empty();
                 ++__counter) {
                __counter->merge(__carry);	// 两个有序链表合并
                __carry.swap(*__counter); // 类似于交换链表carry和counter[i]内容	
            }
            __carry.swap(*__counter); // 类似于交换链表carry和counter[i]内容	
            if (__counter == __fill)
                ++__fill;
        } while (!empty());
		// 每两个进行归并，依次网上，直到最后*(__fill-1)保存最后的排序结果。然后交换到当前list中。
        for (__counter = &__tmp[1]; __counter != __fill; ++__counter)
            __counter->merge(*(__counter - 1));
        swap(*(__fill - 1)); 
    }
}
```

上述代码看起来比较难懂，在网上查找后发现，G2.9中：

```c++
template <class T, class Alloc>
void list<T, Alloc> :: sort(){
    // 判断链表是否为空或者只有一个元素
    if(node->next == node || link_type(node->next)->next == node){
        return;
    }
    
    list<T, Alloc> carry;
    list<T, alloc> counter[64];
    int fill = 0;
    while(!empty()){
        carry.splice(carry.begin(), *this, begin());
        int i = 0;
        while(i < fill && !counter[i].empty()){
            counter[i].merge(carry);
            carry.swap(counter[i++]);
        }
        carry.swap(counter[i]);
        if(i == fill){
            ++fill;
        } 
    }
    
    for(int i = 1; i < fill; ++i){
        counter[i].merge(counter[i-1]);
    }
    swap(counter[fill-1]);
}
```

对应的外部实现是：

```c++
void sortList(list<int> &l) {
    if (l.size() <= 1) {
        return;
    }
    list<int> carry;       // 辅助链表，用于从a中提取元素以及临时保存两个链表的合并结果
    list<int> counter[64]; // 保存着当前每一个归并层次的结果, i号链表保存的元素个数为2的i次方或者0
    int fill = 0;          // 表示当前最大归并排序的层次，while循环之后fill变成log2(a.size())

    while (!l.empty()) {
        carry.splice(carry.begin(), l, l.begin()); // 将链表a中的第一个元素移动至carry开头
        int i = 0;
        // 从小往大不断合并非空归并层次直至遇到空层或者到达当前最大归并层次
        while (i < fill && !counter[i].empty()) {
            counter[i].merge(carry);    // 链表合并，结果链表是有序的，必须保证合并前两个链表是有序的
            carry.swap(counter[i++]);   // 链表元素互换
        }
        carry.swap(counter[i]);
        if (i == fill) {       // i到达当前最大归并层次，说明得增加一层
            ++fill;
        }
    }

    for (int i = 1; i < fill; ++i) {  // 将所有归并层次的结果合并得到最终结果counter[fill - 1]
        counter[i].merge(counter[i - 1]);
    }
    l.swap(counter[fill - 1]);
}
```

这一块可以参考

> https://blog.csdn.net/chenhanzhun/article/details/39337331

上面给出了详细的过程图解。

我们再次将G4.9转换对应的外部实现：

```c++
void sortList1(list<int> &l) {
    typedef list<int> list;
    if (l.size() <= 1) {
        return;
    }
    list __carry;
    list __tmp[64];
    list *__fill = &__tmp[0];
    list *__counter;
    do {
        __carry.splice(__carry.begin(), l, l.begin());
        for (__counter = &__tmp[0];
             __counter != __fill && !__counter->empty();
             ++__counter) {
            __counter->merge(__carry);
            __carry.swap(*__counter);
        }
        __carry.swap(*__counter);
        if (__counter == __fill) ++__fill;
    } while (!l.empty());

    for (__counter = &__tmp[1]; __counter != __fill; ++__counter)
        __counter->merge(*(__counter - 1));

    l.swap(*(__fill - 1));
}
```

使用：

```c++
int main() {
    list<int> l = {7, 5, 8, 1};
    cout << "===============排序前==============" << endl;
    for (auto i:l) cout << i << " ";
    cout << endl;
    sortList1(l);
    cout << "===============排序后==============" << endl;
    for (auto i:l) cout << i << " ";
    cout << endl;

    return 0;
}
```

【**操作符重载**】

```c++
template<typename _Tp, typename _Alloc>
inline bool
operator==(const list<_Tp, _Alloc> &__x, const list<_Tp, _Alloc> &__y) {
    typedef typename list<_Tp, _Alloc>::const_iterator const_iterator;
    const_iterator __end1 = __x.end();
    const_iterator __end2 = __y.end();

    const_iterator __i1 = __x.begin();
    const_iterator __i2 = __y.begin();
    while (__i1 != __end1 && __i2 != __end2 && *__i1 == *__i2) {
        ++__i1;
        ++__i2;
    }
    return __i1 == __end1 && __i2 == __end2;
}
```

实现思路是，迭代判断两个迭代器是否都抵达末尾。

剩下就是其他的操作符重载，比较简单，就不阐述了。其中`lexicographical_compare`实现在`c++-v3/src/c++98/stl_algobase.h`中，该函数是按照字典序测试[frist1,last1)是否小于[first2,last2)。该函数使用opeartor<或者是comp进行比较。其行为类似于：如果两个序列长度不同，并且短序列和长序列头部完全一样，例如example和examplee.那么，长度大的字典序比短序的大。

```c++

template <class InputIterator1, class InputIterator2>
bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
InputIterator2 first2, InputIterator2 last2)
{
    while (first1!=last1)
    {
        if (first2==last2 || *first2<*first1) return false;
        else if (*first1<*first2) return true;
        ++first1; ++first2;
    }
    return (first2!=last2);
}
```

使用：

```c++
int main() {
     vector<char> v1{'h','e','l','l','o'};
    vector<char> v2{'h','e','l','l','o','o'};
    vector<char> v3{'h','e','l','m','o'};
    cout<<"v1=";
    for(char i:v1)
        cout<<i<<" ";
    cout<<endl;
    cout<<"v2=";
    for(char i:v2)
        cout<<i<<" ";
    cout<<endl;
    cout<<"v3=";
    for(char i:v3)
        cout<<i<<" ";
    cout<<endl;

    if(lexicographical_compare(v1.begin(),v1.end(),v2.begin(),v2.end()))
        cout<<"v1 is less than v2 "<<endl;
    else
        cout<<"v2 is less than v1 "<<endl;

    if(lexicographical_compare(v1.begin(),v1.end(),v3.begin(),v3.end()))
        cout<<"v1 is less than v3 "<<endl;
    else
        cout<<"v3 is less than v1 "<<endl;
}
```

其它重载操作符如下：

```c++
template<typename _Tp, typename _Alloc>
inline bool
operator<(const list<_Tp, _Alloc> &__x, const list<_Tp, _Alloc> &__y) {
    return std::lexicographical_compare(__x.begin(), __x.end(),
                                        __y.begin(), __y.end());
}

/// Based on operator==
template<typename _Tp, typename _Alloc>
inline bool
operator!=(const list<_Tp, _Alloc> &__x, const list<_Tp, _Alloc> &__y) { return !(__x == __y); }

/// Based on operator<
template<typename _Tp, typename _Alloc>
inline bool
operator>(const list<_Tp, _Alloc> &__x, const list<_Tp, _Alloc> &__y) { return __y < __x; }

/// Based on operator<
template<typename _Tp, typename _Alloc>
inline bool
operator<=(const list<_Tp, _Alloc> &__x, const list<_Tp, _Alloc> &__y) { return !(__y < __x); }

/// Based on operator<
template<typename _Tp, typename _Alloc>
inline bool
operator>=(const list<_Tp, _Alloc> &__x, const list<_Tp, _Alloc> &__y) { return !(__x < __y); }
```

### 1.2 list基类源码

`_list_base`中有一个结构体:`_List_impl`,而`_List_impl`中有一个`List_node_base`。

```c++
class _List_base
{
protected:
  typedef typename _Alloc::template rebind<_List_node<_Tp> >::other
    _Node_alloc_type;

  typedef typename _Alloc::template rebind<_Tp>::other _Tp_alloc_type;

  struct _List_impl
  : public _Node_alloc_type
  {
    __detail::_List_node_base _M_node;

    _List_impl()
    : _Node_alloc_type(), _M_node()
    { }

    _List_impl(const _Node_alloc_type& __a) _GLIBCXX_NOEXCEPT
    : _Node_alloc_type(__a), _M_node()
    { }

    #if __cplusplus >= 201103L
    _List_impl(_Node_alloc_type&& __a) _GLIBCXX_NOEXCEPT
    : _Node_alloc_type(std::move(__a)), _M_node()
    { }
    #endif
  };

  _List_impl _M_impl;
};

```

最后形成的图就是:

![list's iterator_design](https://raw.githubusercontent.com/Light-City/cloudimg/master/list_iterator_design.png)

所以如果求:

```c++
sizeof(list<int>)=16
```

原因是:

`list`的sizeof为1,所以sizeof来源于基类`_list_base`,而`_list_base`中有一个结构体:`_List_impl`,而`_List_impl`中有一个`_List_node_base`.

我们知道`_List_node_base`,里面有两个指针,在64位上,每个为8字节,共16字节。

## 2.list's Iterator剖析

### 2.1 iterator

list的iterator定义

```c++
template<typename _Tp>
struct _List_iterator
{
  typedef _List_iterator<_Tp>                _Self;
  typedef _List_node<_Tp>                    _Node;

  typedef ptrdiff_t                          difference_type;
  typedef std::bidirectional_iterator_tag    iterator_category;
  typedef _Tp                                value_type;
  typedef _Tp*                               pointer;
  typedef _Tp&                               reference;

   // The only member points to the %list element.
   __detail::_List_node_base* _M_node;         
   //  _List_node(节点的数据部分) -> _List_node_base(前指针与后指针)

  _List_iterator() _GLIBCXX_NOEXCEPT
  : _M_node() { }

  explicit
  _List_iterator(__detail::_List_node_base* __x) _GLIBCXX_NOEXCEPT
  : _M_node(__x) { }

  _Self
  _M_const_cast() const _GLIBCXX_NOEXCEPT
  { return *this; }
 
  // The only member points to the %list element.
  __detail::_List_node_base* _M_node;
};

```

内部重载函数:

```c++
// Must downcast from _List_node_base to _List_node to get to _M_data.
// 重载*操作符
reference operator*() const _GLIBCXX_NOEXCEPT
{ 
	return static_cast<_Node*>(_M_node)->_M_data; 
}

// 重载->操作符
pointer operator->() const _GLIBCXX_NOEXCEPT
{ 
	return std::__addressof(static_cast<_Node*>(_M_node)->_M_data); 
}

// 重载前置++操作符  ++i
_Self& operator++() _GLIBCXX_NOEXCEPT   
{ 
	_M_node = _M_node->_M_next;
	return *this;
}

// 重载后置++操作符 i++
_Self operator++(int) _GLIBCXX_NOEXCEPT
{
    _Self __tmp = *this;			 // 记录原值  *调用的是拷贝构造函数
    _M_node = _M_node->_M_next;		 // 进行操作
    return __tmp;					 // 返回原值
}

// 重载前置--操作符  --i
_Self& operator--() _GLIBCXX_NOEXCEPT
{
	_M_node = _M_node->_M_prev;
	return *this;
}
// 重载后置--操作符  --i
_Self operator--(int) _GLIBCXX_NOEXCEPT
{
    _Self __tmp = *this;
    _M_node = _M_node->_M_prev;
    return __tmp;
}
// 重载++操作符
bool operator==(const _Self& __x) const _GLIBCXX_NOEXCEPT
{ 
	return _M_node == __x._M_node; 
}
// 重载!=操作符
bool operator!=(const _Self& __x) const _GLIBCXX_NOEXCEPT
{ 
	return _M_node != __x._M_node; 
}
```

### 2.2 结点设计

iterator内部的`_List_node`,这里可以得到继承自`_List_node_base`.

`_List_node`放数据部分

`_List_node_base`放前后指针

```c++
/// An actual node in the %list.
template<typename _Tp>
struct _List_node : public __detail::_List_node_base
{
  ///< User's data.
  _Tp _M_data;

#if __cplusplus >= 201103L
  template<typename... _Args>
    _List_node(_Args&&... __args)
: __detail::_List_node_base(), _M_data(std::forward<_Args>(__args)...) 
    { }
#endif
};
```

`_List_node_base`代码:

```c++
namespace __detail
{
    _GLIBCXX_BEGIN_NAMESPACE_VERSION
    /// Common part of a node in the %list. 
    struct _List_node_base
    {
      _List_node_base* _M_next;
      _List_node_base* _M_prev;

      static void
      swap(_List_node_base& __x, _List_node_base& __y) _GLIBCXX_USE_NOEXCEPT;

      void
      _M_transfer(_List_node_base* const __first,
          _List_node_base* const __last) _GLIBCXX_USE_NOEXCEPT;

      void
      _M_reverse() _GLIBCXX_USE_NOEXCEPT;

      void
      _M_hook(_List_node_base* const __position) _GLIBCXX_USE_NOEXCEPT;

      void
      _M_unhook() _GLIBCXX_USE_NOEXCEPT;
    };

    _GLIBCXX_END_NAMESPACE_VERSION
} // namespace detail
```

迭代器在设计的时候,总是保持前闭后开原则,例如iter->begin()指向第一个元素,iter->end()指向实际最后一个元素的下一个元素,故最后的设计刻意在环形list尾部加一个空白结点,用以符合STL前闭后开原则.