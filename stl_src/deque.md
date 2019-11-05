
# C++ STL源码剖析之序列式容器deque
## 0.导语
deque是一种双向开口的分段连续线性空间(简单理解为：双端队列)，可以在头尾端进行元素的插入和删除。

deque与vector最大的差异就是：

- deque允许于常数时间内对头端进行插入或删除元素；

- deque是分段连续线性空间，随时可以增加一段新的空间；

deque不像vector那样，vector当内存不够时，需重新分配/复制数据/释放原始空间；不过deque的迭代器设置比vector复杂，因为迭代器不能使用普通指针，因此尽量使用vector。

## 1.deque中控器

用户看起来deque使用的是连续空间，实际上是**分段连续线性空间**。为了管理分段空间deque容器引入了map，称之为中控器，map是一块连续的空间，其中每个元素是指向缓冲区的指针，缓冲区才是deque存储数据的主体。


![deque_r.png](https://raw.githubusercontent.com/Light-City/cloudimg/master/deque_r.png)

在上图中，buffer称为缓冲区，显示map size的一段连续空间就是中控器。

中控器包含了map size,指向buffer的指针，deque的开始迭代器与结尾迭代器。

```cpp
_Tp		**_M_map;
size_t		_M_map_size;
iterator	_M_start;
iterator	_M_finish;
```

由于buffer也是指针，所以`_Tp`是指针的指针。

deque继承自`_Deque_base`,而`_Deque_base`里面有一个`_M_impl`。

![deque_base.png](https://raw.githubusercontent.com/Light-City/cloudimg/master/deque_bacse.png)

根据下图与上述描述，可以知道，中控器是由`_Deque_impl`实现的。

![impl.png](https://raw.githubusercontent.com/Light-City/cloudimg/master/impl.png)

而deque是使用基类`_Deque_base`来完成内存管理与中控器管理。


## 2.高端的迭代器

对于deque来说，它的迭代器设计的非常棒！

如下图所示：
![deque_iterator.png](https://raw.githubusercontent.com/Light-City/cloudimg/master/deque_iterator.png)

首先来看一下比较重要的成员：

```cpp
typedef _Tp				**_Map_pointer;
_Tp		*_M_cur;
_Tp		*_M_first;
_Tp		*_M_last;
_Map_pointer	_M_node;
```

这几个究竟是什么呢，根据名字，很容易知道啥意思，对于deque来说，是分段连续空间，迭代器执行操作，上述的`_M_cur`指向具体的元素，`_M_first`指向这段buffer中的第一个元素,`_M_last`指向最后一个元素(不是有效的元素)，而`_M_node`则是指向中控器。所以它是一个指针的指针。


例如现在迭代器执行++操作，当前buffer不够用了，那么此时需要一个指针能够回到中控器，取下一段buffer，重置`_M_first`与`_M_last`的指针位置，`_M_cur`指向新段buffer中的指定位置。

我们现在回到一开始的图：

![deque_r.png](https://raw.githubusercontent.com/Light-City/cloudimg/master/deque_r.png)

最上面的的iterator就是上面几个指针的区块配图。


那buffer计算是什么实现的呢?

在源码中计算是根据传递进来的类型，如果传递的类型小于512字节，那么buffersize就是512/sizeof(_Tp)，超过512，就是1。


```cpp
static size_t _S_buffer_size()
_GLIBCXX_NOEXCEPT
{
    return(__deque_buf_size( sizeof(_Tp) ) );
}

```
`__deque_buf_size`实现

```cpp
#ifndef _GLIBCXX_DEQUE_BUF_SIZE
#define _GLIBCXX_DEQUE_BUF_SIZE 512
#endif
inline size_t
__deque_buf_size( size_t
            __size )
{
    return(__size < _GLIBCXX_DEQUE_BUF_SIZE
            ? size_t( _GLIBCXX_DEQUE_BUF_SIZE / __size ) : size_t( 1 ) );
}
```
前面几节源码中提到了萃取机技术，针对每个迭代器都需要嵌入下面五种typedef：

```cpp
typedef std::random_access_iterator_tag iterator_category;
typedef _Tp				value_type;
typedef _Ptr				pointer;
typedef _Ref				reference;
typedef ptrdiff_t			difference_type;
```

据此，也可以知道deque迭代器的使用的是随机访问迭代器：`random_access_iterator_tag`。

而vector使用的迭代器也是这个，根据侯捷老师所讲，连续的buffer是vector，这与迭代器的tag类型不谋而合。


下面来看一下这个强大的迭代器的一些操作符重载：

具体的讲解在代码里面说。

> 取值操作符

```cpp
reference
operator*() const
_GLIBCXX_NOEXCEPT
{
    return(*_M_cur);
}


pointer
operator->() const
_GLIBCXX_NOEXCEPT
{
    return(_M_cur);
}
```
当然上述的`->`也可以直接调用`*`操作符来实现，例如：

```cpp
pointer
operator->() const
_GLIBCXX_NOEXCEPT
{
    return &(operator*());
}
```

> ++与--操作符

```cpp

// 前置++操作符
_Self &
operator++()
_GLIBCXX_NOEXCEPT
{
    // 先++，判断是否到了buffer的末尾，如果到了末尾，就要跳到下一个buffer。
    ++_M_cur;
    if ( _M_cur == _M_last ) // _M_last指向的不是有效元素，保留节点  
    {
        _M_set_node( _M_node + 1 );
        _M_cur = _M_first;
    }
    return(*this);
}

// 后置++操作符
_Self
operator++( int )
_GLIBCXX_NOEXCEPT
{
    _Self __tmp = *this;
    ++*this;
    return(__tmp);
}

// 前置--操作符
_Self &
operator--()
_GLIBCXX_NOEXCEPT
{
    // 先判断是否到了起始位置，如果到了，由于需要进行--操作，那么就应该进入前一个buffer
    if ( _M_cur == _M_first )
    {
        _M_set_node( _M_node - 1 );
        _M_cur = _M_last;
    }
    --_M_cur;
    return(*this);
} //先在容器头部插入与第一个元素相同的元素

// 后置--操作符
_Self
operator--( int )
_GLIBCXX_NOEXCEPT
{
    _Self __tmp = *this;    /* 定义一个副本 */
    --*this;                /* 迭代器自减操作 */
    return(__tmp);
}
```

> 跳跃n个距离操作符


```cpp
/*
* 实现随机取,迭代器可以直接跳跃n个距离
* 将迭代器前移n个距离,当n负值时就为下面的operator-=操作
*/
_Self &
operator+=( difference_type __n )

_GLIBCXX_NOEXCEPT
{
    const difference_type __offset = __n + (_M_cur - _M_first);
    /*
        * 若前移n个距离后，目标依然在同一个缓冲区
        * 则直接前移n个距离
        */
    if ( __offset >= 0 && __offset < difference_type( _S_buffer_size() ) )
        _M_cur += __n;
    else {
        /*
            * 若前移n个距离后,目标超出了缓冲区范围
            * __offset>0   __offset / difference_type(_S_buffer_size())计算向后移动多少个缓冲区
            * __offset<=0  -difference_type((-__offset - 1) / _S_buffer_size()) - 1计算向前移动多少个缓冲区
            */
        const difference_type __node_offset =
            __offset > 0 ? __offset / difference_type( _S_buffer_size() )
            : -difference_type( (-__offset - 1)
                        / _S_buffer_size() ) - 1;
        /* 调整到正确的缓冲区,此时_M_first已经修改了 */
        _M_set_node( _M_node + __node_offset );
        /* 修改为正确的指针位置 */
        _M_cur = _M_first + (__offset - __node_offset
                        * difference_type( _S_buffer_size() ) );
    }
    return(*this);
}

```
下面这几个操作符都是调用上面的`+=`操作符实现：


```cpp

/*
    * 操作符+重载
    * 返回操作之后的副本
    */
_Self
operator+( difference_type __n ) const
_GLIBCXX_NOEXCEPT
{
    _Self __tmp = *this;
    /* 调用operator+=操作 */
    return(__tmp += __n);
}


/* 利用operator+=操作实现 */
_Self &
operator-=( difference_type __n )
_GLIBCXX_NOEXCEPT
{
    return(*this += -__n);
}


/*
    * 操作符-重载
    * 返回操作之后的副本
    */
_Self
operator-( difference_type __n ) const
_GLIBCXX_NOEXCEPT
{
    _Self __tmp = *this;    /*  保存副本 */
    return(__tmp -= __n);   /* 调用operator-=操作符 */
}


/* 返回指定位置的元素，即实现随机存取 */
reference
operator[]( difference_type __n ) const
_GLIBCXX_NOEXCEPT
{
    return(*(*this + __n) );    /* 该函数调用operator+,operator* */
}

```


> buffer跳跃

前面的++与--等操作符，会调用到`_M_set_node`函数，该函数的作用是能够进行buffer之间的跳跃，修改`_M_node`、`_M_first`、`_M_last`的指向。

```cpp
/**
    *  Prepares to traverse new_node.  Sets everything except
    *  _M_cur, which should therefore be set by the caller
    *  immediately afterwards, based on _M_first and _M_last.
    */
void
_M_set_node( _Map_pointer __new_node )
_GLIBCXX_NOEXCEPT
{
    _M_node		= __new_node;                                           /* 指向新的节点 */
    _M_first	= *__new_node;                                          /* 指向新节点的头部 */
    _M_last		= _M_first + difference_type( _S_buffer_size() );       /* 指向新节点的尾部 */
}
```
据此，我们就把deque的迭代器实现细节讲解完毕了。



## 3.deque

> begin()函数


返回`_M_start`。

```cpp
iterator
begin()
_GLIBCXX_NOEXCEPT
{
    return(this->_M_impl._M_start);
}
```

> end()函数


返回`_M_finish`。

```cpp
iterator
end()
_GLIBCXX_NOEXCEPT
{
    return(this->_M_impl._M_finish);
}

```

> size()函数

```cpp
size_type
size() const

_GLIBCXX_NOEXCEPT
{
    return(this->_M_impl._M_finish - this->_M_impl._M_start);
}
```

> resize()函数

根据传递进来的大小，如果超过了总size，就重新分配扩充`__new_size-size()`空间，否则删除从`size()-__new_size`数据，例如现在有20个空间，resize(12)，就会把后面8个空间数据删除及空间释放。

```cpp
void
resize( size_type __new_size )
{
    const size_type __len = size();
    if ( __new_size > __len )
        _M_default_append( __new_size - __len );
    else if ( __new_size < __len )
        _M_erase_at_end( this->_M_impl._M_start
                    + difference_type( __new_size ) );
}
```
> empty()函数

判断两个指针位置即可。

```cpp
bool
empty() const

_GLIBCXX_NOEXCEPT
{
    return(this->_M_impl._M_finish == this->_M_impl._M_start);
}
```
> back函数


```cpp
reference
back()
_GLIBCXX_NOEXCEPT       // 指向finish的前一个位置
{
    iterator __tmp = end();
    --__tmp;
    return(*__tmp);
}
```

> push_front函数


```cpp
void
push_front( const value_type &__x )
{
    //若当前缓冲区存在可用空间
    if ( this->_M_impl._M_start._M_cur != this->_M_impl._M_start._M_first )
    {
        this->_M_impl.construct( this->_M_impl._M_start._M_cur - 1, __x );// 直接构造对象
        --this->_M_impl._M_start._M_cur;    // 调整指针所指位置
    } else
        _M_push_front_aux( __x );   // 需分配一段新的连续空间
}
```

> push_back函数


```cpp
void
push_back( const value_type &__x )
{
    //若当前缓冲区存在可用空间
    if ( this->_M_impl._M_finish._M_cur
            != this->_M_impl._M_finish._M_last - 1 )
    {
        this->_M_impl.construct( this->_M_impl._M_finish._M_cur, __x ); // 直接构造对象
        ++this->_M_impl._M_finish._M_cur;       //调整指针所指位置
    } else     // 若当前缓冲区不存在可用空间
    // 需分配一段新的连续空间
        _M_push_back_aux( __x );
}
```
上述对应的pop动作与之相反。

> insert()函数

insert函数比较有意思，根据传递进来的迭代器位置，看是不在开头与结尾，如果是在开头直接调用`push_front`函数，结尾直接调`push_back`函数，否则在容器中直接插入元素。

```cpp
template <typename _Tp, typename _Alloc>
typename deque<_Tp, _Alloc>::iterator
deque<_Tp, _Alloc>::
insert(iterator __position, const value_type& __x)
{
        if (__position._M_cur == this->_M_impl._M_start._M_cur)   
    {
        push_front(__x);
        return this->_M_impl._M_start;
    }
        else if (__position._M_cur == this->_M_impl._M_finish._M_cur)
    {
        push_back(__x);
        iterator __tmp = this->_M_impl._M_finish;
        --__tmp;
        return __tmp;
    }
        else  //否则在容器直接插入数据
        return _M_insert_aux(__position._M_const_cast(), __x);
}

```
而上述在容器中直接插入元素函数，会计算插入点，如果比较靠前面，就在前面插入，靠近后面就在后面插入：


```cpp

template<typename _Tp, typename _Alloc>
typename deque<_Tp, _Alloc>::iterator
deque<_Tp, _Alloc>::
_M_insert_aux(iterator __pos, const value_type& __x)
{
    value_type __x_copy = __x; // XXX copy
    difference_type __index = __pos - this->_M_impl._M_start;  //计算插入点之前元素个数
    if (static_cast<size_type>(__index) < size() / 2)   //若插入点之前的元素较少
        {
        push_front(_GLIBCXX_MOVE(front())); //先在容器头部插入与第一个元素相同的元素
        iterator __front1 = this->_M_impl._M_start;
        ++__front1;
        iterator __front2 = __front1;
        ++__front2;
        __pos = this->_M_impl._M_start + __index;
        iterator __pos1 = __pos;
        ++__pos1;
        _GLIBCXX_MOVE3(__front2, __pos1, __front1); // 元素搬移
        }
    else
    {
        push_back(_GLIBCXX_MOVE(back()));
        iterator __back1 = this->_M_impl._M_finish;
        --__back1;
        iterator __back2 = __back1;
        --__back2;
        __pos = this->_M_impl._M_start + __index;
        _GLIBCXX_MOVE_BACKWARD3(__pos, __back2, __back1);
    }
    *__pos = _GLIBCXX_MOVE(__x_copy);       // 在安插点上设定新值
    return __pos;
}
```
