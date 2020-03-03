# C++ STL源码剖析之容器配接器stack与queue、priority_queue

## 0.导语
为何stack与queue不被称为容器呢？

下面本节带着这个问题来深入源码分析。


## 1.stack

在stack的源码中我们关注两点：
- 默认`_Sequence`为`deque`
- 内部函数实现是调用`_Sequence`对应容器的函数。


![stack.png](https://raw.githubusercontent.com/Light-City/cloudimg/master/stack.png)

```cpp
template<typename _Tp, typename _Sequence = deque<_Tp> >
class stack
{

public:
    typedef typename _Sequence::value_type                value_type;
    typedef typename _Sequence::reference                 reference;
    typedef typename _Sequence::const_reference           const_reference;
    typedef typename _Sequence::size_type                 size_type;
    typedef          _Sequence                            container_type;

protected:
    //  See queue::c for notes on this name.
    _Sequence c;

public:
     reference
      top()
      {
        __glibcxx_requires_nonempty();
        return c.back();
      }
    void
      push(const value_type& __x)
      { c.push_back(__x); }
}

```




> 测试stack底层容器

对于stack来说，底层容器可以是`vector`、`deque`、`list`，但不可以是`map`、`set`。
由于编译器不会做全面性检查，当调用函数不存在的时候，就编译不通过，所以对于像set虽然不能作为底层容器，但如果具有某些函数，调用仍然是成功的，直到调用的函数不存在。

```cpp
int test_stack() {
    cout<<"============test_stack============="<<endl;
    clock_t timeStart = clock();
    stack<int, list<int>> c;
    for (long i = 0; i < 100000; i++)
        c.push(i+1);
    cout << "stack.size()= " << c.size() << endl;
    cout << "stack.top()= " << c.top() << endl;
    c.pop();
    cout << "stack.size()= " << c.size() << endl;
    cout << "stack.top()= " << c.top() << endl;
    cout << "use stack milli-seconds : " << (clock() - timeStart) << endl;
    timeStart=clock();
    stack<int, deque<int>> c1;
    for (long i = 0; i < 100000; i++)
        c1.push(i+1);
    cout << "stack.size()= " << c1.size() << endl;
    cout << "stack.top()= " << c1.top() << endl;
    c1.pop();
    cout << "stack.size()= " << c1.size() << endl;
    cout << "stack.top()= " << c1.top() << endl;
    cout << "use stack milli-seconds : " << (clock() - timeStart) << endl;

    // vector可以作为stack的底层容器
    stack<int, vector<int>> c2;
    for (long i = 0; i < 100000; i++)
        c2.push(i+1);
    cout << "stack.size()= " << c2.size() << endl;
    cout << "stack.top()= " << c2.top() << endl;
    c2.pop();
    cout << "stack.size()= " << c2.size() << endl;
    cout << "stack.top()= " << c2.top() << endl;
    cout << "use stack milli-seconds : " << (clock() - timeStart) << endl;
}
```


## 2.queue

在queue的源码中我们关注两点：
- 默认`_Sequence`为`deque`
- 内部函数实现是调用`_Sequence`对应容器的函数。

```cpp
template<typename _Tp, typename _Sequence = deque<_Tp> >
class queue
{
public:
    typedef typename _Sequence::value_type                value_type;
    typedef typename _Sequence::reference                 reference;
    typedef typename _Sequence::const_reference           const_reference;
    typedef typename _Sequence::size_type                 size_type;
    typedef          _Sequence                            container_type;

protected:

    _Sequence c;

public:

    void push(const value_type& __x)
    { c.push_back(__x); }

    void pop()
    { 
        __glibcxx_requires_nonempty();
      c.pop_front();
    }
}
```
其对应的UML类图如下：

![queue_.png](https://raw.githubusercontent.com/Light-City/cloudimg/master/queue_.png)

同理，优先队列则是使用`vector`作为默认容器。

```cpp
template<typename _Tp, typename _Sequence = vector<_Tp>,
typename _Compare  = less<typename _Sequence::value_type> >
class priority_queue
{
public:
    typedef typename _Sequence::value_type                value_type;
    typedef typename _Sequence::reference                 reference;
    typedef typename _Sequence::const_reference           const_reference;
    typedef typename _Sequence::size_type                 size_type;
    typedef          _Sequence                            container_type;

protected:
    //  See queue::c for notes on these names.
    _Sequence  c;
    _Compare   comp;

public:
    reference
    top() 
    {
	    __glibcxx_requires_nonempty();
	    return c.front();
    }

    void
    push(const value_type& __x)
    {
	    c.push_back(__x);
	    std::push_heap(c.begin(), c.end(), comp);
    }

}
```


![priority_queue.png](https://raw.githubusercontent.com/Light-City/cloudimg/master/priority_queue.png)


测试这两个容器配接器支持的底层容器：

> queue


对于queue底层容器可以是`deque`，也可以是`list`，但不能是`vector`,`map`,`set`，使用默认的deque效率在插入方面比其他容器作为底层要快！

```cpp
int test_queue() {
    cout<<"============test_queue============="<<endl;
    clock_t timeStart = clock();
    queue<int, list<int>> c;
    for (long i = 0; i < 100000; i++) {
        c.push(i+1);
    }
    cout << "stack.size()= " << c.size() << endl;
    cout << "stack.front()= " << c.front() << endl;
    c.pop();
    cout << "stack.size()= " << c.size() << endl;
    cout << "stack.front()= " << c.front() << endl;
    cout << "use list milli-seconds : " << (clock() - timeStart) << endl;

    timeStart=clock();
    queue<int, deque<int>> c1;
    for (long i = 0; i < 100000; i++) {
        c1.push(i+1);
    }
    cout << "stack.size()= " << c1.size() << endl;
    cout << "stack.front()= " << c1.front() << endl;
    c1.pop();
    cout << "stack.size()= " << c1.size() << endl;
    cout << "stack.front()= " << c1.front() << endl;

    cout << "use deque milli-seconds : " << (clock() - timeStart) << endl;
}
```

> priority_queue

对于优先队列来说，测试结果发现，采用`deque`要比默认的`vector`插入速度快！
底层支持vector、deque容器，但不支持list、map、set。

```cpp
int test_priority_queue() {
    cout<<"============test_priority_queue============="<<endl;
    clock_t timeStart = clock();

    priority_queue<int, deque<int>> c1;
    for (long i = 0; i < 100000; i++) {
        c1.push(i+1);
    }
    cout << "stack.size()= " << c1.size() << endl;
    cout << "stack.top()= " << c1.top() << endl;
    c1.pop();
    cout << "stack.size()= " << c1.size() << endl;
    cout << "stack.top()= " << c1.top() << endl;

    cout << "use deque milli-seconds : " << (clock() - timeStart) << endl;


    priority_queue<int, vector<int>> c2;
    for (long i = 0; i < 100000; i++)
        c2.push(i+1);
    cout << "stack.size()= " << c2.size() << endl;
    cout << "stack.top()= " << c2.top() << endl;
    c2.pop();
    cout << "stack.size()= " << c2.size() << endl;
    cout << "stack.top()= " << c2.top() << endl;
    cout << "use stack milli-seconds : " << (clock() - timeStart) << endl;
}
```

因此，stack、queue、priority_queue不被称为容器， 把它称为容器配接器。