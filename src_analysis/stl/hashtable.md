# C++ STL源码剖析之哈希表

## 0.导语

哈希表，是作为`unordered_map`与`unordered_set`等的底层容器，自gcc2.9后源码量大增！

这次阅读的代码仍旧是gcc4.9.1，代码量非常多，就不全部展开，重点研究底层哈希的艺术与技术，似乎这两个词语很押韵哦，哈哈，进入正文～


## 1.Hashtable初识

先来看一眼Hashtable源码：

```cpp
template<typename _Key, typename _Value, typename _Alloc,
    typename _ExtractKey, typename _Equal,
    typename _H1, typename _H2, typename _Hash,
    typename _RehashPolicy, typename _Traits>
class _Hashtable
: 　public __detail::_Hashtable_base<_Key, _Value, _ExtractKey, _Equal,
    public __detail::_Map_base<_Key, _Value, _Alloc, _ExtractKey, _Equal,  _H1, _H2, _Hash, _RehashPolicy, _Traits>,
    public __detail::_Insert<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, _RehashPolicy, _Traits>,
    public __detail::_Rehash_base<_Key, _Value, _Alloc, _ExtractKey, _Equal,_H1, _H2, _Hash, _RehashPolicy, _Traits>,
    public __detail::_Equality<_Key, _Value, _Alloc, _ExtractKey, _Equal,_H1, _H2, _Hash, _RehashPolicy, _Traits>,
    private __detail::_Hashtable_alloc<typename __alloctr_rebind<_Alloc,__detail::_Hash_node<_Value,_Traits::__hash_cached::value> >::__type>
{
};
```

没学过类模板的一脸懵逼，数一下模板参数都晕死。。。

还有它的继承，一下子整出这么多父亲来。。。

下面就来一一分析它的父亲，然后再回到哈希表。


## 2._Hashtable_base

其中注释中如下：

> Helper class adding management of _Equal functor to _Hash_code_base type.


帮助程序类，将仿函数_Equal的管理添加到_Hash_code_base中。

对比代码就可以看出来是啥意思了：
```cpp
template<typename _Key, typename _Value,
    typename _ExtractKey, typename _Equal,
    typename _H1, typename _H2, typename _Hash, typename _Traits>
struct _Hashtable_base
: public _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2, _Hash, _Traits::__hash_cached::value>,
private _Hashtable_ebo_helper<0, _Equal>
{

};
```
对比一下`_Hash_code_base`与`_Hashtable_base`，两者就差一个`_Equal`，据此这句话解释完毕。

它的基类又有两个分别是：

```cpp
__detail::_Hash_code_base
__detail::_Hashtable_ebo_helper
```

我们继续追踪这两个类！

### 2.1 _Hash_code_base

这个类最后一个`__cache_hash_code`表示是否缓存hash code。
```cpp
template<typename _Key, typename _Value, typename _ExtractKey,
typename _H1, typename _H2, typename _Hash,bool __cache_hash_code>
struct _Hash_code_base;
```

根据是否缓存，得到其偏特化版本：

- 使用范围哈希(实际上就是我们通常说的除留余数法)，不缓存hash code。

```cpp
template<typename _Key, typename _Value, typename _ExtractKey,
typename _H1, typename _H2, typename _Hash>
struct _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2, _Hash, false>
: private _Hashtable_ebo_helper<0, _ExtractKey>,
private _Hashtable_ebo_helper<1, _Hash>
}
```
- 使用范围哈希(实际上就是我们通常说的除留余数法)，缓存hash code。

对于这个偏特化，缓存是没有必要的，所以代码中只是声明，并没有定义！

```cpp
template<typename _Key, typename _Value, typename _ExtractKey,
typename _H1, typename _H2, typename _Hash>
struct _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2, _Hash, true>;
```
- 有哈希函数以及范围哈希函数，不缓存hash code。

```cpp
template<typename _Key, typename _Value, typename _ExtractKey,
typename _H1, typename _H2>
struct _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2,
_Default_ranged_hash, false>
: private _Hashtable_ebo_helper<0, _ExtractKey>,
private _Hashtable_ebo_helper<1, _H1>,
private _Hashtable_ebo_helper<2, _H2>
{
};
```
- 上述的缓存hash code

```cpp
template<typename _Key, typename _Value, typename _ExtractKey,
    typename _H1, typename _H2>
struct _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2,
            _Default_ranged_hash, true>
: private _Hashtable_ebo_helper<0, _ExtractKey>,
    private _Hashtable_ebo_helper<1, _H1>,
    private _Hashtable_ebo_helper<2, _H2>
{
```

上述_H1与_H2大家肯定很迷惑，下面来看一下：

（1） Default range hashing function(默认范围哈希函数)

```cpp
h1=hash<key>
```
下面这个就是：

```cpp
h2(h1(key),N)=h1(key)%N
```
具体可以在后面看到阐述。
```cpp
struct _Mod_range_hashing
{
    typedef std::size_t first_argument_type;
    typedef std::size_t second_argument_type;
    typedef std::size_t result_type;

    result_type
    operator()(first_argument_type __num,
            second_argument_type __den) const noexcept
    { return __num % __den; }
};
```
别看使用一个struct定义的，大家会以为是类，实际上重载了()操作符，就是个仿函数。

上面对应到哈希表数据结构中，就是大家知道的散列函数：**除留余数法**。

```
f(__num) = __num mod __den(__den<=__num)
```
其次，是`_Default_ranged_hash`:

```cpp
struct _Default_ranged_hash { };
```
这个只是作为标记用，默认已经计算的范围哈希函数( Default ranged hash function):
```
h(k, N) = h2(h1(k), N),
```
所以到这，底层的哈希表的散列函数很明显了，默认就是这样的。
而刚才提到的标记就是由于类型H1与H2的对象组合成H，会消耗额外的拷贝操作，因此这里引出了这个标记。

至此，上面提到的_H1与_H2讲解完毕，就是分别对应上述两个函数。


（2） rehash操作

紧接着，还有个比较重要的称为rehash，相信大家很清楚rehash，当散列表的冲突到达一定程度，那么就需要重新将key放到合适位置，而哈希表的底层源码就是这样做的，这里封装成了一个rehash policy：
```cpp
struct _Prime_rehash_policy
{
   //...
};
```
rehash操作中提到：桶的大小(bucket size) 默认通常是最小的素数，从而保证装载因子(load factor 容器当前元素数量与桶数量之比。)足够小。装载因子用来衡量哈希表满的程度，最大加载因子默认值为1.0.
```cpp
_Prime_rehash_policy(float __z = 1.0)
: _M_max_load_factor(__z), _M_next_resize(0) { }


> rehash计算下一个素数桶

```
当哈希冲突的时候，怎么rehash呢？
```c++
 inline std::size_t
_Prime_rehash_policy::
_M_next_bkt(std::size_t __n) const
{
    const unsigned long* __p = std::lower_bound(__prime_list, __prime_list + _S_n_primes, __n);
    _M_next_resize = 
        static_cast<std::size_t>(__builtin_ceil(*__p * _M_max_load_factor));
return *__p;
}
```
当发生哈希冲突的时候，该函数会返回一个不小于n的素数来作为一下个桶。

> 素数表

怎么查找素数呢？
发现上面有个`__prime_list`,于是取查找，在`libstdc++v3/src/shared/hashtable-aux.cc`中找到了所有的素数表。
里面总共有256+1+49或者256+49个。
如果sizeof(unsigned long)!=8 就是256+1+49个，否则就是256+49个。


```cpp
extern const unsigned long __prime_list[] = // 256 + 1 or 256 + 48 + 1
  {
    2ul, 3ul, 5ul, 7ul, 11ul, 13ul, 17ul, 19ul, 23ul, 29ul, 31ul,
    37ul, 41ul, 43ul, 47ul, 53ul, 59ul, 61ul, 67ul, 71ul, 73ul, 79ul,
    83ul, 89ul, 97ul, 103ul, 109ul, 113ul, 127ul, 137ul, 139ul, 149ul，
    // 后面还有很多
  }
```
所以一切都变得非常清晰，那就是通过lower_bound在上述表中去找第一个大于等于给定值n的素数。

```
enum { _S_n_primes = sizeof(unsigned long) != 8 ? 256 : 256 + 48 };
```
> 计算元素对应的桶

根据最大加载因子算出最小的桶，然后根据桶计算出对于每个元素对应的最小素数桶。

```cpp
inline std::size_t
_Prime_rehash_policy::
_M_bkt_for_elements(std::size_t __n) const
{
    // 获取最小的桶
    const float __min_bkts = __n / _M_max_load_factor;
    // 获取最小素数p
    const unsigned long* __p = std::lower_bound(__prime_list, __prime_list
                        + _S_n_primes, __min_bkts);
    _M_next_resize =
        static_cast<std::size_t>(__builtin_ceil(*__p * _M_max_load_factor));
    return *__p;
}
```


_Hashtable_ebo_helper就是前面学习过的EBO空基类 


`_Map_base`主要是通过偏特化，实现重载操作符`[]`与`at`。

｀_Insert｀主要完成插入相关。

｀_Rehash_base｀主要完成上述rehash中的最大加载因子值的传递。

｀_Equality_base｀主要是为类`_Equality`提供公共类型与函数。

到现在为止，上述的`_Hashtable`继承的所有类都阐述完毕。

## 2.hashtable中链表的节点结构

hash node基类，这个只包含指针声明。

```cpp
struct _Hash_node_base
{
    _Hash_node_base* _M_nxt;

    _Hash_node_base() noexcept : _M_nxt() { }

    _Hash_node_base(_Hash_node_base* __next) noexcept : _M_nxt(__next) { }
};
```
带节点值的类继承上述基类
```cpp
template<typename _Value>
struct _Hash_node_value_base : _Hash_node_base
{
    typedef _Value value_type;

    __gnu_cxx::__aligned_buffer<_Value> _M_storage;

    _Value*
    _M_valptr() noexcept
    { return _M_storage._M_ptr(); }

    const _Value*
    _M_valptr() const noexcept
    { return _M_storage._M_ptr(); }

    _Value&
    _M_v() noexcept
    { return *_M_valptr(); }

    const _Value&
    _M_v() const noexcept
    { return *_M_valptr(); }
};

```
前面提到节点是否还有hash code，故在节点中应该得带hash code，而具体在下面中实现：

```cpp
/**
*  Primary template struct _Hash_node.
*/
template<typename _Value, bool _Cache_hash_code>
struct _Hash_node;

/**
*  Specialization for nodes with caches, struct _Hash_node.
*
*  Base class is __detail::_Hash_node_value_base.
*/
template<typename _Value>
struct _Hash_node<_Value, true> : _Hash_node_value_base<_Value>
{
    std::size_t  _M_hash_code;

    _Hash_node*
    _M_next() const noexcept
    { return static_cast<_Hash_node*>(this->_M_nxt); }
};

/**
*  Specialization for nodes without caches, struct _Hash_node.
*
*  Base class is __detail::_Hash_node_value_base.
*/
template<typename _Value>
struct _Hash_node<_Value, false> : _Hash_node_value_base<_Value>
{
    _Hash_node*
    _M_next() const noexcept
    { return static_cast<_Hash_node*>(this->_M_nxt); }
};

```

到这里就很明确了，对于节点，分为包含hash code与不包含，具体是根据传递的模板参数，来调用相应的偏特化版本。


## 3.迭代器

迭代器基类显示使用using的语法，这个语法类似于typedef，后面定义就可以直接使用`__node_type`语法来定义，`_M_incr`函数完成链表下一个节点获取。

```cpp
/// Base class for node iterators.
template<typename _Value, bool _Cache_hash_code>
struct _Node_iterator_base
{
    using __node_type = _Hash_node<_Value, _Cache_hash_code>;

    __node_type*  _M_cur;

    _Node_iterator_base(__node_type* __p) noexcept
    : _M_cur(__p) { }

    void
    _M_incr() noexcept
    { _M_cur = _M_cur->_M_next(); }
};
```

节点迭代器：对下面代码研读，学习到两点：
- 第一：using 的使用
- hashtable的迭代器属于forward_iterator
- 重载了++,--,*,->，这四个操作符

```cpp
template<typename _Value, bool __constant_iterators, bool __cache>
struct _Node_iterator
: public _Node_iterator_base<_Value, __cache>
{
private:
    using __base_type = _Node_iterator_base<_Value, __cache>;
    using __node_type = typename __base_type::__node_type;

public:
    typedef _Value					value_type;
    typedef std::ptrdiff_t				difference_type;
    typedef std::forward_iterator_tag			iterator_category;

    using pointer = typename std::conditional<__constant_iterators,
                    const _Value*, _Value*>::type;

    using reference = typename std::conditional<__constant_iterators,
                        const _Value&, _Value&>::type;

    _Node_iterator() noexcept
    : __base_type(0) { }

    explicit
    _Node_iterator(__node_type* __p) noexcept
    : __base_type(__p) { }

    reference
    operator*() const noexcept
    { return this->_M_cur->_M_v(); }

    pointer
    operator->() const noexcept
    { return this->_M_cur->_M_valptr(); }

    _Node_iterator&
    operator++() noexcept
    {
        this->_M_incr();
        return *this;
    }

    _Node_iterator
    operator++(int) noexcept
    {
        _Node_iterator __tmp(*this);
        this->_M_incr();
        return __tmp;
    }
};
```

## 4.仔细研究hashtable的重要内部结构

内部结构为在每个元素中维护一个单链表, 然后在单链表上执行元素的插入、搜寻、删除等操作，每个元素被称为桶(bucket)，底层构建先采用H1计算出key的hash code，再通过除留余数法H2得到其对应的桶。

```cpp
template<typename _Key, typename _Value, typename _Alloc,
    typename _ExtractKey, typename _Equal,
    typename _H1, typename _H2, typename _Hash,
    typename _RehashPolicy, typename _Traits>
class _Hashtable
private:
    __bucket_type*		_M_buckets;  //_ Hash_node_base *
    size_type			_M_bucket_count;    // bucket 节点个数
    __node_base		_M_before_begin;    // _NodeAlloc::value_type
    size_type			_M_element_count;   // //hashtable中list节点个数
    _RehashPolicy		_M_rehash_policy;  // rehash策略
    __bucket_type		_M_single_bucket;  // 只需要一个桶用
};
```
hashtable的一些重要函数：

> begin函数

```cpp
iterator
begin() noexcept
{ return iterator(_M_begin()); }
```

调用`_M_begin`:

可以把`_M_before_begin`想象成一个head节点，第一个节点就是下一个节点。

```cpp
__node_type*
_M_begin() const
{ return static_cast<__node_type*>(_M_before_begin._M_nxt); }
```
> end函数

因为是单链表，返回最后一个即可。

```cpp
iterator
end() noexcept
{ return iterator(nullptr); }
```

> size与empty函数

```cpp
size_type
size() const noexcept
{ return _M_element_count; }

bool
empty() const noexcept
{ return size() == 0; }
```

> 桶数量

```cpp
size_type
bucket_count() const noexcept
{ return _M_bucket_count; }
```

> 计算加载因子

当前元素数量除以桶的数量

```cpp
float
load_factor() const noexcept
{
    return static_cast<float>(size()) / static_cast<float>(bucket_count());
}
```
> 桶的index计算

根据传递进来的key获得桶的index。

```cpp
size_type
bucket(const key_type& __k) const
{ return _M_bucket_index(__k, this->_M_hash_code(__k)); }
```
在`_Hash_code_base`中有如下实现：

而`_M_h1`返回的是`_H1`，`_H1`不知道是什么情况下，我们可以在`unordered_map`中查找到是`_Hash=hash<_Key>`，因此下面这个函数就是数学表达式：
`h1(k)`来获取hash code。

返回桶的hash code。

```cpp
__hash_code
_M_hash_code(const _Key& __k) const
{ return _M_h1()(__k); }
```

返回桶的index。

`_M_bucket_index`在同一文件后面找到定义：

```cpp
size_type
_M_bucket_index(const key_type& __k, __hash_code __c) const
{ return __hash_code_base::_M_bucket_index(__k, __c, _M_bucket_count); }
```
我们继续去`__hash_code_base`查找`_M_bucket_index`，可在`bits/hashtable_policy.h`中找到：
```cpp
std::size_t
_M_bucket_index(const _Key&, __hash_code __c,
        std::size_t __n) const
{ return _M_h2()(__c, __n); }
```
同上述h1的查找，可以在`unordered_map`中查到`_H2`默认采用`_Mod_range_hashing`，再看这个源码：
```cpp
struct _Mod_range_hashing
{
    typedef std::size_t first_argument_type;
    typedef std::size_t second_argument_type;
    typedef std::size_t result_type;

    result_type
    operator()(first_argument_type __num,
            second_argument_type __den) const noexcept
    { return __num % __den; }
};
```
对应数学表达式就是`h2(c,n)`。


因此上述`bucket`获取桶的index对应的数学表达式就是：

```cpp
h(k,hash(k))=h(k,hash(k),n)=h(k,hash(k)%n)
```
实际上就是最终的：

```cpp
hash(k)%n
```
这个就是桶的index计算。
