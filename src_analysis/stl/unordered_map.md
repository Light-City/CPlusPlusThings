# C++ STL源码剖析之unordered_map、unordered_multimap、unordered_set、unordered_multiset

## 0.导语

前面学到了hashtable，而这节是hashtable的容器适配器：unordered_map。

所以无序map的底层容器采用hashtable。

unordered_map与unordered_multimap的源码在`unordered_map.h`这个文件中。


## 1.undered_map与unordered_multimap本质区别
先来看一下undered_map源码：

```cpp
template<class _Key, class _Tp,
class _Hash = hash<_Key>,
class _Pred = std::equal_to<_Key>,
class _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
class unordered_map
{
    typedef __umap_hashtable<_Key, _Tp, _Hash, _Pred, _Alloc>  _Hashtable;
    _Hashtable _M_h;
};
```

去看底层容器的`__umap_hashtable`的声明：

```cpp
template<bool _Cache>
using __umap_traits = __detail::_Hashtable_traits<_Cache, false, true>;

template<typename _Key,
    typename _Tp,
    typename _Hash = hash<_Key>,
    typename _Pred = std::equal_to<_Key>,
    typename _Alloc = std::allocator<std::pair<const _Key, _Tp> >,
    typename _Tr = __umap_traits<__cache_default<_Key, _Hash>::value>>
using __umap_hashtable = _Hashtable<_Key, std::pair<const _Key, _Tp>,
_Alloc, __detail::_Select1st,
_Pred, _Hash,
__detail::_Mod_range_hashing,
__detail::_Default_ranged_hash,
__detail::_Prime_rehash_policy, _Tr>;
```
可以得到下面结论:
hashtable的模板参数：
```cpp
template<typename _Key, typename _Value, typename _Alloc,
typename _ExtractKey, typename _Equal,
typename _H1, typename _H2, typename _Hash,
typename _RehashPolicy, typename _Traits>
```
默认情况下，undered_map采用：
- H1为hash<key>
- H2为_Mod_range_hashing
- _Hash为_Default_ranged_hash
- _RehashPolicy为_Prime_rehash_policy
- _Traits为_Tr
对于最后的_Tr,非常重要，因为正是因为这个参数，才有undered_multimap。
具体分析看下面：


_Tr如下：

```cpp
typename _Tr = __umap_traits<__cache_default<_Key, _Hash>::value>>
```
_Tr使用了`__umap_traits`，我们继续往下看：
```cpp

template<bool _Cache>
using __umap_traits = __detail::_Hashtable_traits<_Cache, false, true>;
```
可以对比上述两个发现__umap_traits里面有一串__cache_default，我们再看一下模板参数为bool类型，故可以打印出来是false还是true，经过实测，为false，表示不缓存hash code。

```cpp
template<typename _Tp, typename _Hash>
using __cache_default
=  __not_<__and_<__is_fast_hash<_Hash>,
__detail::__is_noexcept_hash<_Tp, _Hash>>>;
```
继续看`__umap_traits`，这个实际上是调用`_Hashtable_traits`，我们继续往下：

```cpp
template<bool _Cache_hash_code, bool _Constant_iterators, bool _Unique_keys>
struct _Hashtable_traits
{
    using __hash_cached = __bool_constant<_Cache_hash_code>;
    using __constant_iterators = __bool_constant<_Constant_iterators>;
    using __unique_keys = __bool_constant<_Unique_keys>;
};
```
看到有三个using，理解为三个typedef，依次表示：hash code缓存与否，是否是常迭代器，是否是唯一的key，再往上回头看，传递进来的是三个模板参数，分别是false,false,true，也验证了undered_map是唯一的key，那么对应的undered_multimap就是不唯一的key，最后一个参数为false。我们翻阅到相应代码如下：

```cpp
/// Base types for unordered_multimap.
template<bool _Cache>
using __ummap_traits = __detail::_Hashtable_traits<_Cache, false, false>;
```

小结，在上面分析，我们知道了unordered_map与unordered_multimap的本质区别，也发现了如何在底层源码上用一个容器实现两个容器适配器！

## 2.undered_set与unordered_multiset本质区别
分析同前面一样，先看undered_set:

```cpp
template<class _Value,
    class _Hash = hash<_Value>,
    class _Pred = std::equal_to<_Value>,
    class _Alloc = std::allocator<_Value> >
class unordered_set
{
    typedef __uset_hashtable<_Value, _Hash, _Pred, _Alloc>  _Hashtable;
    _Hashtable _M_h;
}

template<bool _Cache>
using __uset_traits = __detail::_Hashtable_traits<_Cache, true, true>;

template<typename _Value,
    typename _Hash = hash<_Value>,
    typename _Pred = std::equal_to<_Value>,
    typename _Alloc = std::allocator<_Value>,
    typename _Tr = __uset_traits<__cache_default<_Value, _Hash>::value>>
using __uset_hashtable = _Hashtable<_Value, _Value, _Alloc,
                __detail::_Identity, _Pred, _Hash,
                __detail::_Mod_range_hashing,
                __detail::_Default_ranged_hash,
                __detail::_Prime_rehash_policy, _Tr>;
```
可以看到传递给`_Hashtable_traits`的是false,true,true。对于undered_set来说使用的是const iterator与唯一的key,我们再看一下unordered_multiset：

```cpp
template<bool _Cache>
using __umset_traits = __detail::_Hashtable_traits<_Cache, true, false>;
```
再将两者对比一下，本质就是undered_set不允许key重复，而undered_multiset允许key重复。

## 3.三大结论

现在，我们有了前面基础，依次列出前面四个容器适配器：

(1) undered_map
```cpp
template<bool _Cache>
using __umap_traits = __detail::_Hashtable_traits<_Cache, false, true>;
```
(2) undered_multimap
```cpp
template<bool _Cache>
using __umap_traits = __detail::_Hashtable_traits<_Cache, false, false>;
```
(3) undered_set

```cpp
template<bool _Cache>
using __uset_traits = __detail::_Hashtable_traits<_Cache, true, true>;
```
(4) undered_multiset

```cpp
template<bool _Cache>
using __uset_traits = __detail::_Hashtable_traits<_Cache, true, false>;
```

对比后，得出

- 结论1：undered_map与undered_set不允许key重复,而带multi的则允许key重复；
- 结论2：undered_map与undered_multimap采用的迭代器是iterator，而undered_set与undered_multiset采用的迭代器是const_iterator。
- 结论3：undered_map与undered_multimap的key是key，value是key+value；而undered_set与undered_multiset的key是Value，Value也是Key。

最后一个结论对比看下面(我们看传递给hashtable的第一与第二个参数)：

undered_map与undered_multimap：
```cpp
using __umap_hashtable = _Hashtable<_Key, 
std::pair<const _Key, _Tp>,
_Alloc, __detail::_Select1st,
_Pred, _Hash,
__detail::_Mod_range_hashing,
__detail::_Default_ranged_hash,
__detail::_Prime_rehash_policy, _Tr>;
```
undered_set与undered_multiset：
```cpp
template<typename _Value,
typename _Hash = hash<_Value>,
typename _Pred = std::equal_to<_Value>,
typename _Alloc = std::allocator<_Value>,
typename _Tr = __uset_traits<__cache_default<_Value, _Hash>::value>>
using __uset_hashtable = _Hashtable<_Value, _Value, _Alloc,
__detail::_Identity, _Pred, _Hash,
__detail::_Mod_range_hashing,
__detail::_Default_ranged_hash,
__detail::_Prime_rehash_policy, _Tr>;
```

## 4.undered_map重要函数

> 初始化

可以在下面的构造函数中看到undered_map的默认桶数为10。

在undered_map的底层默认采用hasher(),也就是H1,也就是std::hash

```cpp
unordered_map(size_type __n = 10,
    const hasher& __hf = hasher(),
    const key_equal& __eql = key_equal(),
    const allocator_type& __a = allocator_type())
: _M_h(__n, __hf, __eql, __a)
{ }
```
下面测试是否采用默认的hash：

```cpp
unordered_map<string,int> um;
hash<string> h;
cout<<um.hash_function()("hhhhhawq")<<endl;
cout<<h("hhhhhawq")<<endl;
```
输出：

```cpp
9074142923776869151
9074142923776869151
```
进一步验证了采用默认的hash。

> 是否空、大小、最大大小

```cpp
bool
empty() const noexcept
{ return _M_h.empty(); }
///  Returns the size of the %unordered_map.
size_type
size() const noexcept
{ return _M_h.size(); }

///  Returns the maximum size of the %unordered_map.
size_type
max_size() const noexcept
{ return _M_h.max_size(); }
```

> begin与end

```cpp
iterator
begin() noexcept
{ return _M_h.begin(); }
iterator
end() noexcept
{ return _M_h.end(); }
```
> insert
五种插入方式

```cpp
// value
std::pair<iterator, bool>
insert(const value_type& __x)
{ return _M_h.insert(__x); }

// pair 
std::pair<iterator, bool>
insert(_Pair&& __x)
{ return _M_h.insert(std::forward<_Pair>(__x)); }

// iterator+value
iterator
insert(const_iterator __hint, const value_type& __x)
{ return _M_h.insert(__hint, __x); }


// first到last范围插入
template<typename _InputIterator>
void
insert(_InputIterator __first, _InputIterator __last)
{ _M_h.insert(__first, __last); }

// 初始化列表插入

void
insert(initializer_list<value_type> __l)
{ _M_h.insert(__l); }
```

> 删除

三种删除方式

```cpp
// iterator
iterator
erase(iterator __position)
{ return _M_h.erase(__position); }

// key
size_type
erase(const key_type& __x)
{ return _M_h.erase(__x); }

// first到last范围

iterator
erase(const_iterator __first, const_iterator __last)
{ return _M_h.erase(__first, __last); 
```

> 清除

```cpp
void
clear() noexcept
{ _M_h.clear(); }
```
> hash_function

得到该undered_map的hash_function
```cpp
hasher
hash_function() const
{ return _M_h.hash_function(); }
```
使用：
```cpp
unordered_map<string,int> um;
cout<<um.hash_function()("hhhhhawq")<<endl; //传递的内容要与上面key类型一致。
```

> key_eq
key_eq返回的是std::equal_to
使用如下：
```cpp
unordered_map<string,int> um;
cout<<um.key_eq()("1","2")<<endl;
```
> 查找与获取value

```cpp
iterator
find(const key_type& __x)
{ return _M_h.find(__x); }
mapped_type&
operator[](const key_type& __k)
{ return _M_h[__k]; }
mapped_type&
at(const key_type& __k)
{ return _M_h.at(__k); }
```
除了这些函数还有获取桶，最大桶数、加载因子、rehash等等，就是没有排序，因为hashtable没有提供排序功能。hashtable在查找、删除和插入节点是常数时间，优于RB-Tree红黑树。

同理，unordered_set、unordered_multiset、unordered_multimap与undered_map一样的函数，所以就不阐述了。