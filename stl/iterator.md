# C++ STL源码剖析之实现一个简单的iterator_category

## 0.导语

本节使用上节Traits特性，研究iterator源码，来实现一个简单的iterator_category，同时对iterator的源码结构进行分析。

**知其然，知其所以然，源码面前了无秘密！**

## 1.利用萃取机实现一个简单的iterator_category识别

上一节指出了迭代器的作用，依旧如下图所示：

![](https://raw.githubusercontent.com/Light-City/cloudimg/master/rela.png)

迭代器是指向序列元素的指针的一种抽象。通过使用迭代器，我们可以访问序列中的某个元素、改变序列中的某个元素的值、使迭代器向前或向后行走等等。

迭代器有常见有五种类型: value_type, difference_type, reference_type, pointer_type都比较容易在 traits 和相应偏特化中提取。

但是，iterator_category一般也有5个，这个相应型别会引发较大规模的写代码工程。

- 单向移动只读迭代器 Input Iterator
- 单向移动只写迭代器 Output Iterator
- 单向移动读写迭代器 Forward Iterator
- 双向移动读写迭代器 Bidirectional Iterator

![](https://raw.githubusercontent.com/Light-City/cloudimg/master/iterator.png)

例如：我们实现了 advanceII, advanceBI, advanceRAI 分别代表迭代器类型是Input Iterator，Bidirectional Iterator和Random Access Iterator的对应实现。

```c++
template<class Iterator>
void advance(Iterator& i) {
    if (is_random_access_iterator(i))
        advanceRAI(i,n);
    if (is_bidirectional_iterator(i))
        advanceBI(i,n);
    else
        advanceII(i,n);
}
```

但这样在执行时期才决定使用哪一个版本，会**影响程序效率**。最好能够在编译期就选择正确的版本。

而**重载**这个函数机制可以达成这个目标。

而对于`advanceXX()`都有两个函数参数，型别都未定(因为都是模板参数)。为了令其同名，形成重载函数，我们必须加上一个型别已确定的函数参数，使函数重载机制得以有效运作起来。

设计如下：如果**traits**有能力萃取出迭代器的种类，我们便可利用这个"迭代器类型"相应型别作为advancexx的第三个参数，而这个相应型别必须是一个class type，不能只是数值号码类的东西，因为编译器需依赖它来进行**重载决议**。

下面来进行实现，首先给出一个总体结构图：

![](https://raw.githubusercontent.com/Light-City/cloudimg/master/itera.png)

定义出下面tag：

```c++
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
// 继承的好处就是，当函数需要用 input_iterator_tag 的时候
// 假设你传进一个forward_iterator_tag，它会沿继承向上找，知道符合条件
```

声明了一些列 tag 之后，我们就可以重载 advance函数，我们把这些函数用下滑线来定义，表示在内部使用，外部不可见。

```c++
// 继承的好处就是，当函数需要用 input_iterator_tag 的时候
// 假设你传进一个forward_iterator_tag，它会沿继承向上找，知道符合条件
// input iterator
template<class inputIterator, class distance>
inline void __advance(inputIterator&i, distance n,
                      input_iterator_tag) {
    std::cout << "input tag" << std::endl;
}
// output iterator
template<class outputIterator, class distance>
inline void __advance(outputIterator&i, distance n,
                      output_iterator_tag) {
    std::cout << "output tag" << std::endl;
}

// forward iterator
template<class ForwardIterator, class Distance>
inline void __advance(ForwardIterator &i, Distance n,
                      forward_iterator_tag) {
    std::cout << "forward tag" << std::endl;
}

// bidrectional iterator
template<class BidiectionalIterator, class Distance>
inline void __advance(BidiectionalIterator &i, Distance n,
                      bidiectional_iterator_tag) {
    std::cout << "bidrectional tag" << std::endl;

}

// RandomAccess iterator
template<class RandomAccessIterator, class Distance>
inline void __advance(RandomAccessIterator &i, Distance n,
                      random_access_iterator_tag) {
    std::cout << "randomaccess tag" << std::endl;

}
```

定义萃取机：

```c++
// traits 型别
template<class I>
struct Iterator_traits {
    typedef typename I::iterator_category iterator_category;
};

// 针对原生指针设计的"偏特化版"
template<class I>
struct Iterator_traits<I *> {
    typedef random_access_iterator_tag iterator_category;
};
template<class I>
struct Iterator_traits<const I *> {
    typedef random_access_iterator_tag iterator_category;
};
```

对外暴露接口：

```c++
// 对外接口
template<class InputIterator, class Distance>
inline void advance(InputIterator &i, Distance n) {
    // 通过Ierator_traits询问它的iterator_category是谁
    typedef typename Iterator_traits<InputIterator>::iterator_category category;
    __advance(i, n, category()); // 各型别的重载
}

```

定义class type：

```c++
// class type
template<class Category>
struct iterator {
    typedef Category iterator_category;
};
```

开始测试，我们使用上述定义的class type与原生指针来测试，分别进入萃取机的普通萃取机与偏特化萃取机，看看是否得到相应的Tag。

```c++
int main() {
    iterator<input_iterator_tag> input;
    iterator<output_iterator_tag> output;
    iterator<forward_iterator_tag> forward;
    iterator<bidiectional_iterator_tag> bidect;
    iterator<random_access_iterator_tag> random;
    advance(input, 10);
    advance(output, 10);
    advance(forward, 10);
    advance(bidect, 10);
    advance(random, 10);
    int *p=NULL;
    advance(p,10);
    return 0;
}
```

输出结果：

```c++
input tag
output tag
forward tag
bidrectional tag
randomaccess tag
randomaccess tag
```

一切如我们预期一样，通过萃取机，我们获得了每个迭代器的tag，以及原生指针的tag。

我们再想得复杂一些，如果我们想知道advance的返回类型，那如何做呢？

首先修改`advance`返回：

```c++
// 对外接口
template<class InputIterator, class Distance>
inline typename Iterator_traits<InputIterator>::iterator_category
advance(InputIterator &i, Distance n) {
    // 通过Ierator_traits询问它的iterator_category是谁
    typedef typename Iterator_traits<InputIterator>::iterator_category category;
    return __advance(i, n, category()); // 各型别的重载
}
```

紧接着修改`__advance`返回：

```c++
// input iterator
template<class inputIterator, class distance>
inline typename Iterator_traits<inputIterator>::iterator_category
__advance(inputIterator &i, distance n,
          input_iterator_tag) {
    std::cout << "input tag" << std::endl;
    return input_iterator_tag();
}

// output iterator
template<class outputIterator, class distance>
inline typename Iterator_traits<outputIterator>::iterator_category
__advance(outputIterator &i, distance n,
          output_iterator_tag) {
    std::cout << "output tag" << std::endl;
    return output_iterator_tag();
}

// forward iterator
template<class ForwardIterator, class Distance>
inline typename Iterator_traits<ForwardIterator>::iterator_category
__advance(ForwardIterator &i, Distance n,
          forward_iterator_tag) {
    std::cout << "forward tag" << std::endl;
    return forward_iterator_tag();
}

// bidrectional iterator
template<class BidiectionalIterator, class Distance>
inline typename Iterator_traits<BidiectionalIterator>::iterator_category
__advance(BidiectionalIterator &i, Distance n,
          bidiectional_iterator_tag) {
    std::cout << "bidrectional tag" << std::endl;
    return bidiectional_iterator_tag();
}

// RandomAccess iterator
template<class RandomAccessIterator, class Distance>
inline typename Iterator_traits<RandomAccessIterator>::iterator_category
__advance(RandomAccessIterator &i, Distance n,
          random_access_iterator_tag) {
    std::cout << "randomaccess tag" << std::endl;
    return random_access_iterator_tag();
}
```

只需要把`void`修改为相应的萃取机即可。

最后测试修改，添加上返回：

```c++
int main() {
    iterator<input_iterator_tag> input;
    iterator<output_iterator_tag> output;
    iterator<forward_iterator_tag> forward;
    iterator<bidiectional_iterator_tag> bidect;
    iterator<random_access_iterator_tag> random;
    input_iterator_tag inputIteratorTag = advance(input, 10);
    output_iterator_tag outputIteratorTag = advance(output, 10);
    forward_iterator_tag forwardIteratorTag = advance(forward, 10);
    bidiectional_iterator_tag bidiectionalIteratorTag = advance(bidect, 10);
    random_access_iterator_tag randomAccessIteratorTag = advance(random, 10);
    int *p = NULL;
    random_access_iterator_tag v = advance(p, 10);
    return 0;
}
```

至此，一个简单的迭代器类型在编译器判别实现完毕。

## 2.STL源码剖析Iterator

在`bits/stl_iterator_base_types.h`中也是如上述所示(实际上，上面就是STL源码的简单版，很接近)，来我们一起来看。

（1）`tag`

```c++
 ///  Marking input iterators.
  struct input_iterator_tag { };

  ///  Marking output iterators.
  struct output_iterator_tag { };

  /// Forward iterators support a superset of input iterator operations.
  struct forward_iterator_tag : public input_iterator_tag { };

  /// Bidirectional iterators support a superset of forward iterator
  /// operations.
  struct bidirectional_iterator_tag : public forward_iterator_tag { };

  /// Random-access iterators support a superset of bidirectional
  /// iterator operations.
  struct random_access_iterator_tag : public bidirectional_iterator_tag { };
```

与我上面用的一样。

（2）`iterator_traits`萃取机，里面包含五种，而上面只是实现其中的一种：`iterator_category`。所以在STL中容器与算法之间的桥梁iterator必须包含下面五种 typedef。

```c++
template<typename _Iterator>
struct iterator_traits
{
  typedef typename _Iterator::iterator_category iterator_category;
  typedef typename _Iterator::value_type        value_type;
  typedef typename _Iterator::difference_type   difference_type;
  typedef typename _Iterator::pointer           pointer;
  typedef typename _Iterator::reference         reference;
};
```

（3）`iterator`

上面提到的class type为下面的简单版，对比一下，没有啥区别，就是模板参数多了一些，typedef多了。

```c++
template<typename _Category, typename _Tp, typename _Distance = ptrdiff_t,
       typename _Pointer = _Tp*, typename _Reference = _Tp&>
struct iterator
{
  /// One of the @link iterator_tags tag types@endlink.
  typedef _Category  iterator_category;
  /// The type "pointed to" by the iterator.
  typedef _Tp        value_type;
  /// Distance between iterators is represented as this type.
  typedef _Distance  difference_type;
  /// This type represents a pointer-to-value_type.
  typedef _Pointer   pointer;
  /// This type represents a reference-to-value_type.
  typedef _Reference reference;
};
```

至此，iterator与traits特性分析完毕。欢迎与我共同探讨STL源码奥秘，如侯捷老师所说：**源码面前了无秘密。**