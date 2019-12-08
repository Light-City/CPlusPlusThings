# C++ STL源码剖析之map、multimap、initializer_list
map/multimap 以rb_tree为底层结构，因此有元素自动排序特点，排序的依据是key。

map/multimap提供"遍历"操作及iterators。按正常规则(++iter)遍历，便能够获得排序状态。

我们无法使用map/multimap的iterators改变元素的key(因为key有其严谨排列规则)，但可以用它来改变元素的data。因此map/multimap内部自动将用户指定的key type设定为const，如此便能进制用户对元素key的赋值。

map元素的key必须独立无二，因此其insert使用的是rb_tree的`_M_insert_unique()`，而multimap元素的key可以重复，因此其insert使用的是rb_tree的`_M_insert_equal()`。

对于本节，我们将从下面几个内容阐述：

- map的key为key,value为key+data,与set是不同的，set是key就是value，value就是key。
- map的key不可修改,map与multimap的插入调用函数不同，影响了其key是否对应value。
- initializer_list使用
- map有`[]`操作符，而multimap没有`[]`操作符。


## 1.map

> key为key，value为key+data

下面map中我们可以看到value_type为一个pair。

```cpp
template <typename _Key, typename _Tp, typename _Compare = std::less<_Key>,
typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
class map
{
public:
    typedef _Key                                          key_type;
    typedef _Tp                                           mapped_type;
    typedef std::pair<const _Key, _Tp>                    value_type;
    typedef _Compare                                      key_compare;
    typedef _Alloc                                        allocator_type;
private:
    // key为key,value为key+data
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>,
            key_compare, _Pair_alloc_type> _Rep_type;

    /// The actual tree structure.
    _Rep_type _M_t;
};
```
![map_data.png](https://raw.githubusercontent.com/Light-City/cloudimg/master/map_data.png)

上述默认的仿函数为`_Select1st`，我们在`stl_function`中看到源码如下：

```cpp
template<typename _Pair>
struct _Select1st
: public unary_function<_Pair, typename _Pair::first_type>
{
    typename _Pair::first_type&
    operator()(_Pair& __x) const
    { return __x.first; }
};
```
我们看到上述的`_Select1st`为一个struct，怎么能说它是仿函数呢？
因为里面重载了一个()操作符，哈哈～

下面我们来自己实现一个：

```cpp
template<typename _T1>
struct mySelect1st
        : public unary_function<_T1, typename _T1::first_type>
{
    template<typename _T2>
    typename _T2::first_type&
    operator()(_T2& __x) const
    { return __x.first; }
};
int main() {
     typedef pair<const int,int> value_type;
    _Rb_tree<int, value_type, mySelect1st<value_type>, less<int>> it;
    it._M_insert_unique(make_pair(1,3));
    it._M_insert_unique(make_pair(3,6));
    for(auto each:it)
        cout<<each.first<<" "<<each.second<<endl;
}
```
> key不能改，data可以改

上述源码中：自动为key添加一个const，所以key不能改。
```cpp
typedef std::pair<const _Key, _Tp>                    value_type;
```
## 2.insert

> insert里面采用`_M_insert_unique`

insert的几种方法：

（1） 插入 pair

```cpp
std::pair<iterator, bool> insert(const value_type& __x)
{ return _M_t._M_insert_unique(__x); }
```

map里面

（2） 在指定位置，插入pair
```cpp
iterator insert(iterator __position, const value_type& __x)
{ return _M_t._M_insert_equal_(__position, __x); }
```

（3） 从一个范围进行插入

```cpp
template<typename _InputIterator>
void
insert(_InputIterator __first, _InputIterator __last)
{ _M_t._M_insert_equal(__first, __last); }
```
（4）从list中插入

```cpp
void
insert(initializer_list<value_type> __l)
{ this->insert(__l.begin(), __l.end()); }
```

针对最后一个insert，里面有个`initializer_list`，举个例子大家就知道了。


## 3.initializer_list使用

> 实际编程实践 


```cpp
vector<int> v={1,2,3};   // 底层调用vector的构造函数
v={2,5,6};               // 底层调用vector的=操作符
initializer_list<int> ll={4,5,6};
v.insert(v.begin(),ll);  // 底层调用下面insert函数
for(auto x:v) cout<<x<<" ";
cout<<endl;
vector<int> vv(ll);      // 底层调用vector的构造函数
vector<string> city{"Berlin", "New York", "London", "Cairo","Tokyo", "Cologne"};
```
针对这个vector初始化，大家很熟悉了，为何可以这样初始化呢？
我们看一下vector源码：

```cpp
 vector&
operator=(initializer_list<value_type> __l)
{
    this->assign(__l.begin(), __l.end());
    return *this;
}

iterator
insert(const_iterator __position, initializer_list<value_type> __l)
{ return this->insert(__position, __l.begin(), __l.end()); }

vector(initializer_list<value_type> __l,
const allocator_type& __a = allocator_type())
: _Base(__a)
{
_M_range_initialize(__l.begin(), __l.end(),
    random_access_iterator_tag());
}
```

因为它的构造函数里面，参数有个`initializer_list`,因此我们可以针对这个对map进行使用。
但是map没有类似的构造，它也应用在map构造函数，insert与`=`处，跟上面是一样的，都是三处，哈哈～

使用`initializer_list`三处：

```cpp
// map构造
map(initializer_list<value_type> __l, const allocator_type& __a)
: _M_t(_Compare(), _Pair_alloc_type(__a))
{ _M_t._M_insert_unique(__l.begin(), __l.end()); }
// =操作符重载
map&
operator=(initializer_list<value_type> __l)
{
    this->clear();
    this->insert(__l.begin(), __l.end());
    return *this;
}
// insert插入
void
insert(std::initializer_list<value_type> __list)
{ insert(__list.begin(), __list.end()); }
```
> 实际编程实践

map使用`initializer_list`(set使用一样)：

```cpp
// 这里要注意，pair的first参数必[须是const
initializer_list<pair<const strin[g,int>> l = {{"hello", 1}, {"world", 2}};
map<string,int> mm(l);  // map构造函数
map<string, int> m2{{"hello", 1}, {"world", 2}}; // map构造函数
map<string, int> m1={{"hello", 1}, {"world", 2}};   // map构造函数
m1 = {{"h", 1}, {"w", 3}};             // 底层调用map的=操作符
map<string, int> mp;
mp.insert(l);       // insert插入[
```

上述会引出另一个问题：

```cpp
map<string, int> m1={{"hello", 1}, {"world", 2}};   // map构造函数
m1 = {{"h", 1}, {"w", 3}};             // 底层调用map的=操作符
```
这两个为何一个调用的构造，一个调用=操作符呢？

在初始化的时候，定义及赋值的时候就直接调用构造，后面再次赋值，就是先调用拷贝构造(有可能会被编译器优化)，再调用=操作符。


> 实例分析

下面用一个具体实例来分析一下：

```cpp
template <typename _Tp>
class Foo
{
public:
    Foo(initializer_list<_Tp> &list)
    {
        cout << "Foo(initializer_list<_Tp> &list)"<< endl;
    }

    Foo(int)
    {
        cout << "Foo(int )"<< endl;
    }

    Foo(const Foo& f)
    {
        cout << "调用了拷贝构造函数"<< endl;
    }
    Foo& operator=(initializer_list<_Tp> __l)
    {
        cout<<"调用了=操作符重载"<<endl;
        return *this;
    }

};

```
调用：
```cpp
Foo<int> foo=ll;
foo={1,2};
```
编译器未被优化的结果：

```
Foo(initializer_list<_Tp> &list)
调用了=操作符重载
```
我们通过禁用编译器优化：`g++ -o rb  rbtree.cpp  -std=c++11 -fno-elide-constructors`

```
Foo(initializer_list<_Tp> &list)
调用了拷贝构造函数
调用了=操作符重载
```

## 4.multimap

同map一样multimap不允许修改key。但是插入使用的是`_M_insert_equal`。
```cpp
template <typename _Key, typename _Tp,
typename _Compare = std::less<_Key>,
typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
class multimap
{
public:                                      
    typedef std::pair<const _Key, _Tp> value_type;   
}
```
下面使用multimap与rbtree两种方式来联系。
```cpp
multimap<int, string> c;
c.insert(make_pair(1,"asdqw"));
c.insert(make_pair(1,"qweq"));

for(auto each:c) cout<<each.first<<" "<<each.second<<endl;

typedef pair<const int,string> valueT;
_Rb_tree<int, valueT, _Select1st<valueT>, less<int>> mulm;
mulm._M_insert_equal(make_pair(2,"abc"));
mulm._M_insert_equal(make_pair(2,"cde"));
for(auto each:mulm)
cout<<each.first<<" "<<each.second<<endl;
```
输出：

```
1 asdqw
1 qweq
2 abc
2 cde
```

## 5.map与multimap的重要操作符

map与multimap`[]`操作符，map的`[]`操作符返回传递给map的第二个参数。

传递给`[]`一个key，如果查找到，就是value，否则就是默认值0。
```cpp
mapped_type&
operator[](const key_type& __k)
{
    iterator __i = lower_bound(__k);// 找到__k第一个。
    // __i->first is greater than or equivalent to __k.
    if (__i == end() || key_comp()(__k, (*__i).first))
    #if __cplusplus >= 201103L
    __i = _M_t._M_emplace_hint_unique(__i, std::piecewise_construct,
                    std::tuple<const key_type&>(__k),
                    std::tuple<>());
    #else
        __i = insert(__i, value_type(__k, mapped_type()));
    #endif
    return (*__i).second;  // 返回key的value，此value为传递进map的第二个参数。
}
```

但是multimap没有`[]`操作符！！！

我们思考一下，因为multimap会根据key，有可能会对应多个value，那如果我们通过`[]`获取对应key的value，此时到底获取的是哪个value呢，所以在STL源码中没有重载这个操作符！

