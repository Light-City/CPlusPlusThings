## 0.回顾

前面一节编写了一个RAII的例子：

```cpp
class shape_wrapper {
public:
    explicit shape_wrapper(
            shape* ptr = nullptr)
            : ptr_(ptr) {}
    ~shape_wrapper()
    {
        delete ptr_;
    }
    shape* get() const { return ptr_; }
private:
    shape* ptr_;
};
```

这个类可以完成智能指针的最基本的功能：对超出作用域的对象进行释放。但它缺了点东
西：

- 这个类只适用于 shape 类

- 该类对象的行为不够像指针

- 拷贝该类对象会引发程序行为

## 1.手写auto_ptr与scope_ptr

针对**"这个类只适用于 shape 类"**，我们想到了模板，于是改造为：

```cpp
template <typename  T>
class smater_ptr {
public:
    explicit smater_ptr(
            T* ptr = nullptr)
            : ptr_(ptr) {}
    ~smater_ptr()
    {
        delete ptr_;
    }
    T* get() const { return ptr_; }
private:
    T* ptr_;
};
```

针对**"该类对象的行为不够像指针"**,我们想到了指针的基本操作有`*`,`->`,布尔表达式。

于是添加三个成员函数：

```cpp
template <typename  T>
class smater_ptr {
public:
   	...
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    operator bool() const { return ptr_; }
    ...
private:
    T* ptr_;
};
```

针对**"拷贝该类对象会引发程序行为"**，我们想到了拷贝构造和赋值。

现考虑如下调用：

```cpp
smart_ptr<shape> ptr1{create_shape(shape_type::circle)};
smart_ptr<shape> ptr2{ptr1};
```

对于第二行，究竟应当让编译时发生错误，还是可以有一个更合理的行为？我们来逐一检查
一下各种可能性。
最简单的情况显然是禁止拷贝。我们可以使用下面的代码：

```cpp
template <typename T>
class smart_ptr {
    …
    smart_ptr(const smart_ptr&)
    = delete;
    smart_ptr& operator=(const smart_ptr&)
    = delete;
    …
};
```

当然，也可以设为private。

禁用这两个函数非常简单，但却解决了一种可能出错的情况。否则，`smart_ptr<shape> ptr2{ptr1};` 在编译时不会出错，但在运行时却会有未定义行为——**由于会对同一内存释放两次，通常情况下会导致程序崩溃。**

我们是不是可以考虑在拷贝智能指针时把对象拷贝一份？不行，通常人们不会这么用，因为使用智能指针的目的就是要减少对象的拷贝啊。何况，虽然我们的指针类型是 shape，但实际指向的却应该是 circle 或 triangle 之类的对象。在 C++ 里没有像 Java 的clone 方法这样的约定；**一般而言，并没有通用的方法可以通过基类的指针来构造出一个子类的对象来。**

那关键点就来了，**所有权！**，我们可以拷贝时转移指针的所有权！下面实现便是`auto_ptr`的核心实现：

```cpp
template<typename T>
class auto_ptr {
public:
    explicit auto_ptr(
            T *ptr = nullptr) noexcept
            : ptr_(ptr) {}

    ~auto_ptr() noexcept {
        delete ptr_;
    }
	// 返回值为T&，允许*ptr=10操作
    T &operator*() const noexcept { return *ptr_; }

    T *operator->() const noexcept { return ptr_; }

    operator bool() const noexcept { return ptr_; }

    T *get() const noexcept { return ptr_; }

    // 拷贝构造,被复制放释放原来指针的所有权,交给复制方
    auto_ptr(auto_ptr &other) noexcept {
        ptr_ = other.release();
    }

    // copy and swap
    auto_ptr &operator=(auto_ptr &rhs) noexcept {
//        auto_ptr tmp(rhs.release());
//        tmp.swap(*this);
        // s上述两行等价于下面一行
        auto_ptr(rhs.release()).swap(*this);
        return *this;
    }

    // 原来的指针释放所有权
    T *release() noexcept {
        T *ptr = ptr_;
        ptr_ = nullptr;
        return ptr;
    }

    void swap(auto_ptr &rhs) noexcept {
        using std::swap;
        swap(ptr_, rhs.ptr_);    // 转移指针所有权
    }

private:
    T *ptr_;
};

template<typename T>
void swap(auto_ptr<T> &lhs, auto_ptr<T> &rhs) noexcept {
    lhs.swap(rhs);
}

int main() {
    auto_ptr<shape> ptr1{create_shape(shape_type::circle)};
    auto_ptr<shape> ptr2{ptr1};
    if (ptr1.get() == nullptr && ptr2.get())
        cout << "拷贝构造：ptr1释放了所有权,ptr2获得了所有权" << endl;
    ptr1 = ptr1;

    auto_ptr<shape> ptr3{create_shape(shape_type::rectangle)};
    ptr1 = ptr3;

    if (ptr3.get() == nullptr && ptr1.get())
        cout << "赋值操作：始终只有一个对象管理一个区块!ptr3释放了所有权,ptr1获得了所有权" << endl;
}
```

上述通过copy-swap技术完成了避免自我赋值与保证了强异常安全！

如果你觉得这个实现还不错的话，那恭喜你，你达到了 C++ 委员会在 1998 年时的水平：上面给出的语义本质上就是 C++98 的 auto_ptr 的定义。如果你觉得这个实现很别扭的话，也恭喜你，因为 C++ 委员会也是这么觉得的：**auto_ptr 在 C++17 时已经被正式从C++ 标准里删除了**。

上面会导致什么问题呢？

看一下输出结果：

```cpp
shape
circle
拷贝构造：ptr1释放了所有权,ptr2获得了所有权
shape
rectangle
赋值操作：始终只有一个对象管理一个区块!ptr3释放了所有权,ptr1获得了所有权
```

shape与circle实在create_shape时候输出的，我们重点关注最后一句话，发现了一个很大的问题：**它的行为会让程序员非常容易犯错。一不小心把它传递给另外一个 auto_ptr，你就不再拥有这个对象了。**

上述拷贝构造与拷贝赋值分别如下面两张图所示：





针对这个问题，在C++11标准出来之前，C++98标准中都一直只有一个智能指针auto_ptr，我们知道，这是一个失败的设计。它的本质是**管理权的转移**，这有许多问题。而这时就有一群人开始扩展C++标准库的关于智能指针的部分，他们组成了boost社区，他们负责boost库的开发和维护。其目的是为C++程序员提供免费的、同行审查的、可移植的程序库。boost库可以和C++标准库完美的共同工作，并且为其提供扩展功能。现在的**C++11标准库**的智能指针很大程度上“借鉴”了boost库。

boost::scoped_ptr 属于 boost 库，定义在 namespace boost 中，包含头文件`#include<boost/smart_ptr.hpp> `可以使用。scoped_ptr 跟 auto_ptr 一样，可以方便的管理单个堆内存对象，特别的是，scoped_ptr 独享所有权，避免了auto_ptr恼人的几个问题。

<u>scoped_ptr是一种简单粗暴的设计，它本质就是**防拷贝**，避免出现管理权的转移。</u>这是它的最大特点，所以他的拷贝构造函数和赋值运算符重载函数都只是声明而不定义，而且为了防止有的人在类外定义，所以将函数声明为private。但这也是它最大的问题所在，就是不能赋值拷贝，也就是说功能不全。但是这种设计比较高效、简洁。没有 release() 函数，不会导致先前的内存泄露问题。下面我也将模拟实现scoped_ptr的管理机制(实际上就是前面提到的禁止拷贝)：

```cpp
template<class T>
class scoped_ptr // noncopyable
{

public:
    explicit scoped_ptr(T *ptr = 0) noexcept : ptr_(ptr) {
    }

    ~scoped_ptr() noexcept {
        delete ptr_;
    }

    void reset(T *p = 0) noexcept {
        scoped_ptr(p).swap(*this);
    }

    T &operator*() const noexcept {
        return *ptr_;
    }

    T *operator->() const noexcept {
        return ptr_;
    }

    T *get() const noexcept {
        return ptr_;
    }

    void swap(scoped_ptr &rhs) noexcept {
        using std::swap;
        swap(ptr_, rhs.ptr_);
    }

private:
    T *ptr_;

    scoped_ptr(scoped_ptr const &);
    scoped_ptr &operator=(scoped_ptr const &);
};

template<typename T>
void swap(scoped_ptr<T> &lhs, scoped_ptr<T> &rhs) noexcept {
    lhs.swap(rhs);
}
```

scoped_ptr特点总结：

1）与auto_ptr类似，采用栈上的指针去管理堆上的内容，从而使得堆上的对象随着栈上对象销毁时自动删除；

2）scoped_ptr有着更严格的使用限制——不能拷贝，这也意味着scoped_ptr不能转换其所有权，所以它管理的对象不能作为**函数的返回值**，对象生命周期仅仅局限于一定区间（该指针所在的{}区间，而std::auto_ptr可以）；

3）由于防拷贝的特性，使其管理的对象**不能共享所有权**，这与std::auto_ptr类似，这一特点使该指针简单易用，但也造成了功能的薄弱。

## 2.手写unique_ptr之子类向基类转换

在上述auto_ptr基础上，我们把拷贝构造与拷贝赋值，改为移动构造与移动赋值。

```cpp
template<typename T>
class unique_ptr {
public:
    explicit unique_ptr(
            T *ptr = nullptr) noexcept
            : ptr_(ptr) {}

    ~unique_ptr() noexcept {
        delete ptr_;
    }

    T &operator*() const noexcept { return *ptr_; }

    T *operator->() const noexcept { return ptr_; }

    operator bool() const noexcept { return ptr_; }

    T *get() const noexcept { return ptr_; }

    unique_ptr(unique_ptr &&other) noexcept {
        ptr_ = other.release();
    }

    // copy and swap  始终只有一个对象有管理这块空间的权限
    unique_ptr &operator=(unique_ptr rhs) noexcept {
        rhs.swap(*this);
        return *this;
    }

    // 原来的指针释放所有权
    T *release() noexcept {
        T *ptr = ptr_;
        ptr_ = nullptr;
        return ptr;
    }

    void swap(unique_ptr &rhs) noexcept {
        using std::swap;
        swap(ptr_, rhs.ptr_);    // 转移指针所有权
    }

private:
    T *ptr_;
};
template<typename T>
void swap(unique_ptr<T> &lhs, unique_ptr<T> &rhs) {
    lhs.swap(rhs);
}
```

调用：

```cpp
int main() {
    unique_ptr<shape> ptr1{create_shape(shape_type::circle)};
//    unique_ptr<shape> ptr2{ptr1}; // error
    unique_ptr<shape> ptr2{std::move(ptr1)};    // ok

    unique_ptr<shape> ptr3{create_shape(shape_type::rectangle)};
//    ptr1 = ptr3;    // error
    ptr3 = std::move(ptr1); // ok
}
```

把拷贝构造函数中的参数类型 unique_ptr& 改成了 unique_ptr&&；现在它成了移动构造函数。
把赋值函数中的参数类型 unique_ptr& 改成了 unique_ptr，在构造参数时直接生成新的智能指针，从而不再需要在函数体中构造临时对象。现在赋值函数的行为是移动还是拷贝，完全依赖于构造参数时走的是移动构造还是拷贝构造。

最后，一个` circle*` 是可以隐式转换成 `shape*`的，但上面的 `unique_ptr<circle>` 却无法自动转换成 `unique_ptr<shape>`。

现在我们考虑两种情况：

**（1）第一种：当我们只是在原先的移动构造上面添加`template <typename U>`，此时情况是移动构造变为带模板的移动构造，可以进行子类向基类转换，但是与移动构造相关的，则调用的是默认移动构造，除非是子类向基类转换，才调用带模板的移动构造。**

```cpp
template <typename U>
unique_ptr(unique_ptr<U> &&other) noexcept {
    ptr_ = other.release();
}
```

**六个特殊的成员函数其生成规则如下：**

- **默认构造函数，生成规则和C++98一样，在用户没有声明自定义的构造函数的时候并且编译期需要的时候生成。**
- **析构函数，生成规则和C++98一样，在C++11中有点不同的是，析构函数默认是noexcept。**
- **拷贝构造函数，用户自定义了移动操作会导致不生成默认的拷贝构造函数，其它和C++98的行为一致。**
- **拷贝赋值操作符，用户自定义了移动操作会导致不生成默认的拷贝赋值操作，其它和C++98的行为一致。**
- **移动构造函数和移动赋值操作符，仅仅在没有用户自定义的拷贝操作，移动操作和析构操作的时候才会生成。**

根据《Effective Modern C++》Item17 P115页提到，当类中含有特殊成员函数变为模板特殊成员函数的时候,此时不满足上述生成规则，也就是针对当前例子来说，编译器会默认生成拷贝构造，所以此时上述main调用里面为error的都可以正常运行！

```cpp
int main() {
    unique_ptr<shape> ptr1{create_shape(shape_type::circle)};
    unique_ptr<shape> ptr2{ptr1}; // 由于带模板的移动构造函数引发编译器会默认生成拷贝构造
    if (ptr1.get() != nullptr)      // bitwise copy 此时ptr1不为NULL
        ptr2.get()->print();

    unique_ptr<shape> ptr2_2{std::move(ptr1)};    // 调用的是默认的移动构造,而不是带模板的移动构造 bitwise move
    if (ptr2_2.get() != nullptr && ptr1.get() != nullptr)   // ptr1 不为空
        ptr2_2.get()->print();

    unique_ptr<shape> ptr3{create_shape(shape_type::rectangle)};
    ptr1 = ptr3;    // ok  根据形参先调用默认拷贝,再调用拷贝赋值
    ptr3 = std::move(ptr1); // ok 根据形参先调用默认移动构造，而不是带参数的移动构造,再调用移动赋值
    unique_ptr<shape> ptr4(std::move(new circle));  // ok 调用带模板的移动构造
}
```

调用与结果如上代码所示。

**（2）第二种：移动构造与带模板的移动构造同时存在，可以完成子类向基类的转换，此时是满足上述生成规则，此时不会生成拷贝函数！**

```cpp
int main() {
    unique_ptr<shape> ptr1{create_shape(shape_type::circle)};
//    unique_ptr<shape> ptr2{ptr1}; // error
    unique_ptr<shape> ptr2_2{std::move(ptr1)};    // ok
    if (ptr2_2.get() != nullptr && ptr1.get() == nullptr)
        ptr2_2.get()->print();

    unique_ptr<shape> ptr3{create_shape(shape_type::rectangle)};
//    ptr1 = ptr3;    // error
    ptr3 = std::move(ptr1); // ok
//    unique_ptr<circle> cl{create_shape(shape_type::circle)};  // error 因为create_shape返回的是shape 不能基类转子类
    unique_ptr<circle> cl{new circle()};
    unique_ptr<shape> ptr5(std::move(cl));  // ok unique<circle>转unique<circle>
}
```

**小结：**

（1）我们需要了解子类向基类的隐式转换，通过将移动构造函数变为带模板的移动构造函数，要明白两者共存情况与只有带模板的移动或者其他构造函数对编译器生成规则的影响！上述代码，此时还不能完成基类向子类的转换！例如:`unique_ptr<circle>`转`unique_ptr<shape>`。

（2）auto_ptr与unique_tr都是独占所有权，每次只能被单个对象所拥有，unique_ptr与auto_ptr不同的是使用移动语义来显示的编写。auto_ptr是可以说你随便赋值,但赋值完了之后原来的对象就不知不觉的报废.搞得你莫名其妙。而unique_ptr就干脆不让你可以随便去复制,赋值.如果实在想传个值就哪里,显式的说明内存转移std:move一下。然后这样传值完了之后,之前的对象也同样报废了.只不过整个move你让明显的知道这样操作后会导致之前的unique_ptr对象失效。scope_ptr则是直接不允许拷贝。由于防拷贝的特性，使其管理的对象**不能共享所有权**。

## 3.shared_ptr之引用计数

unique_ptr 算是一种较为安全的智能指针了。但是，一个对象只能被单个 unique_ptr所拥有，这显然不能满足所有使用场合的需求。一种常见的情况是，多个智能指针同时拥有一个对象；当它们全部都失效时，这个对象也同时会被删除。这也就是 shared_ptr 了。

两者区别如下：



多个shared_ptr不仅共享一个对象，同时还得共享同一个计数。当最后一个指向对象(和共享计数)的shared_ptr析构时，它需要删除对象和共享计数。

首先需要一个共享计数的实现：

```cpp
class shared_count {
public:
    shared_count() : count_(1) {

    }

    // 增加计数
    void add_count() {
        ++count_;
    }

    // 减少计数
    long reduce_count() {
        return --count_;
    }

    // 获取当前计数
    long get_count() const {
        return count_;
    }

private:
    long count_;
};
```

接下来实现引用计数智能指针：

构造与析构、swap实现如下所示：

```cpp
template<typename T>
class shared_ptr {
public:
    explicit shared_ptr(
            T *ptr = nullptr) noexcept
            : ptr_(ptr) {
        if (ptr) {
            shared_count_ = new shared_count();
        }
    }

    ~shared_ptr() noexcept {
        // 最后一个shared_ptr再去删除对象与共享计数
        // ptr_不为空且此时共享计数减为0的时候,再去删除
        if(ptr_&&!shared_count_->reduce_count()) {
            delete ptr_;
            delete shared_count_;
        }
    }
	
    void swap(shared_ptr &rhs) noexcept {
        using std::swap;
        swap(ptr_, rhs.ptr_);   
        swap(shared_count_,rhs.shared_count_);
    }

private:
    T *ptr_;
    shared_count *shared_count_;
};
template<typename T>
void swap(shared_ptr<T> &lhs, shared_ptr<T> &rhs) noexcept {
    lhs.swap(rhs);
}
```

之前的赋值函数，编译器可以根据调用来决定是调拷贝构造还是移动构函数，所以不变：

```cpp
// copy and swap  始终只有一个对象有管理这块空间的权限
shared_ptr &operator=(shared_ptr rhs) noexcept {
    rhs.swap(*this);
    return *this;
}
```

拷贝构造与移动构造需要改变：

除复制指针之外，对于拷贝构造的情况，我们需要在指针非空时把引用数加一，并复制共享
计数的指针。对于移动构造的情况，我们不需要调整引用数，直接把 other.ptr_ 置为
空，认为 other 不再指向该共享对象即可

实现如下所示：

```cpp
template<typename U>
shared_ptr(const shared_ptr<T> &other) noexcept {
    ptr_ = other.ptr_;
    if (ptr_) {
        other.shared_count_->add_count();
        shared_count_ = other.shared_count_;
    }
}

template<typename U>
shared_ptr(shared_ptr<U> &&other) noexcept {
    ptr_ = other.ptr_;
    if (ptr_) {
        shared_count_ = other.shared_count_;
        other.shared_count_ = nullptr;
    }
}
```

当运行的时候，报错：

```
‘circle* shared_ptr<circle>::ptr_’ is private
```

错误原因是**模板的各个实例间并不天然就有 friend 关系**，因而不能互访私有成员 `ptr_ `和`shared_count_`。我们需要在 smart_ptr 的定义中显式声明：

```cpp
template<typename U>
friend class shared_ptr;
```

此外，在当前引用计数实现中，我们应该删除release释放所有权函数，编写一个返回引用计数值的函数。

```cpp
long use_count() const noexcept {
    if (ptr_) {
        return shared_count_->get_count();
    } else {
        return 0;
    }
}
```

调用：

```cpp
shared_ptr<circle> ptr1(new circle());
cout << "use count of ptr1 is " << ptr1.use_count() << endl;
shared_ptr<shape> ptr2, ptr3;
cout << "use count of ptr2 was " << ptr2.use_count() << endl;
ptr2 = ptr1;        // shared_ptr<circle>隐式转换shared_ptr<shape> 调用带模板的拷贝构造
//    cout<<"======="<<endl;
//    ptr3 = ptr2;        // 调用的是编译器生成的默认拷贝构造 所以引用计数不会增加 ptr3=ptr2
//    cout<<"======="<<endl;
ptr3 = ptr1;
cout << "此时3个shared_ptr指向同一个资源" << endl;
cout << "use count of ptr1 is now " << ptr1.use_count() << endl;
cout << "use count of ptr2 is now " << ptr2.use_count() << endl;
cout << "use count of ptr3 is now " << ptr3.use_count() << endl;
if (ptr1)
    cout << "ptr1 is not empty" << endl;
// 会先调用赋值函数,由编译器决定调用的是拷贝构造还是移动构造,造出一个新的临时对象出来,临时对象会在跳出作用域后被析构掉。
// 在析构函数中,会先判断该临时对象的是否指向资源,如果没有,析构结束。否则,对引用计数减1,判断引用计数是否为0,如果为0,删除共享引用计数指针,否则不操作。
cout << "此时2个shared_ptr指向同一个资源" << endl;
ptr2 = std::move(ptr1);
if (!ptr1 && ptr2) {      // 调用的是bool重载操作符
    cout << "ptr1 move to ptr2" << endl;
    cout << "use count of ptr1 is now " << ptr1.use_count() << endl;
    cout << "use count of ptr2 is now " << ptr2.use_count() << endl;
    cout << "use count of ptr3 is now " << ptr3.use_count() << endl;
}
```

输出：

```cpp
shape
circle
use count of ptr1 is 1
use count of ptr2 was 0
此时3个shared_ptr指向同一个资源
use count of ptr1 is now 3
use count of ptr2 is now 3
use count of ptr3 is now 3
ptr1 is not empty
此时2个shared_ptr指向同一个资源
ptr1 move to ptr2
use count of ptr1 is now 0
use count of ptr2 is now 2
use count of ptr3 is now 2
~circle
~shape
```

有几点注意事项：

- 上述代码没有考虑线程安全性，这里只是简化版

- =赋值重载函数不加`&`，编译器决定调用拷贝构造还是移动构造，来造出一个临时对象出来。
- 根据前面提到的，当类中特殊函数变为带模板的函数，编译器仍然会生成默认拷贝构造与默认移动构造。

针对第一点：例如：`ptr2 = std::move(ptr1);`

会先调用赋值函数,由编译器决定调用的是拷贝构造还是移动构造,造出一个新的临时对象出来,临时对象会在跳出作用域后被析构掉。在析构函数中,会先判断该临时对象的是否指向资源,如果没有,析构结束。否则,对引用计数减1,判断引用计数是否为0,如果为0,删除共享引用计数指针,否则不操作。

针对第二点：

```cpp
shared_ptr<shape> ptr2, ptr3;
ptr3 = ptr2;        // 调用的是编译器生成的默认拷贝构造 所以引用计数不会增加 
```

两者都是一种类型，所以在调用赋值操作后，不会调用带模板的拷贝构造来创建临时变量，而是调用编译器生成的默认拷贝构造，所以此时引用计数不会增加。

## 4.指针类型转换

对应于 C++ 里的不同的类型强制转：

- dynamic_cast
- static_cast
- const_cast
- reinterpret_cast

### 4.1 dynamic_cast

在上述`unique_ptr`处实现了子类向基类的转换，但是却没有实现基类向子类的转换，例如：:`unique_ptr<circle>`转`unique_ptr<shape>`。

实现这种，需要使用`dynamic_cast`，实现如下：

首先为了实现这些转换，我们需要添加构造函数，允许在对智能指针内部的指针对象赋值时，使用一个现有的智能指针的共享计数。

```cpp
// 实现强制类型转换需要的构造函数
template<typename U>
shared_ptr(const shared_ptr<U> &other, T *ptr) noexcept {
    ptr_ = ptr;
    if (ptr_) {
        other.shared_count_->add_count();
        shared_count_ = other.shared_count_;
    }
}
```

其次，就是实现转换函数：

```cpp
template<typename T, typename U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U> &other) noexcept {
    T *ptr = dynamic_cast<T *>(other.get());
    return shared_ptr<T>(other, ptr);
}
```

调用：

```cpp
// shape* -> circle* 使用dynamic_cast转换后,指针为空.此时资源还是被dptr2拥有,dptr1为0
shared_ptr<shape> dptr2(new shape);
shared_ptr<circle> dptr1 = dynamic_pointer_cast<circle>(dptr2);      // 基类转子类

cout << "use count of dptr1 is now " << dptr1.use_count() << endl;    // 0
cout << "use count of dptr2 is now " << dptr2.use_count() << endl;    // 1

// circle* -> circle* 使用dynamic_cast转换后,指针不为空,此时资源被两者共同使用,引用计数为2
shared_ptr<shape> dptr3(new circle);
//    shared_ptr<circle> dptr3(new circle);     // 上面或者当前行,后面输出一样！
shared_ptr<circle> dptr1_1 = dynamic_pointer_cast<circle>(dptr3);      // 基类转子类

cout << "use count of dptr1_1 is now " << dptr1_1.use_count() << endl;    // 2
cout << "use count of dptr3 is now " << dptr3.use_count() << endl;    // 2

// circle* -> circle* 使用dynamic_cast转换后,指针不为空,此时资源被两者共同使用,引用计数为2
shared_ptr<circle> dptr3_1(new circle);
shared_ptr<shape> dptr2_1 = dynamic_pointer_cast<shape>(dptr3_1);      // 子类转基类 上行转换,安全！

cout << "use count of dptr2_1 is now " << dptr2_1.use_count() << endl;    // 2
cout << "use count of dptr3_1 is now " << dptr3_1.use_count() << endl;    // 2
```

dynamic_cast主要用于类层次间的上行转换和下行转换，还可以用于类之间的交叉转换。在类层次间进行上行转换时，dynamic_cast和static_cast的效果是一样的；在进行下行转换时，dynamic_cast具有类型检查的功能，比static_cast更安全。在多态类型之间的转换主要使用dynamic_cast，因为类型提供了运行时信息。

（1）下行转换，基类转换为子类，例如：智能指针转换类似于`shape*` 转换为`circle*` 使用dynamic_cast转换后,指针为空.此时资源还是被dptr2拥有,dptr1为0。比static_cast安全。

（2）平行转换，指向一致的相互转换，例如：智能指针转换类似于`circle*`转换为`circle*`。此时引用计数为两者共享。

（3）上行转换，子类转基类，例如：智能指针转换类似于`circle*`转换为`shape*`，此时引用技术为两者共享。等价于static_cast。

### 4.2 static_cast

同样，编写如下：

```cpp
template<typename T, typename U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U> &other) noexcept {
    T *ptr = static_cast<T *>(other.get());
    return shared_ptr<T>(other, ptr);
}
```

调用：

```
// shape* -> circle* 使用static_cast转换后,指针为空  与dynamic_cast相比,不安全
shared_ptr<shape> sptr2(new shape);
shared_ptr<circle> sptr1 = static_pointer_cast<circle>(sptr2);      // 基类转子类

cout << "use count of sptr1 is now " << dptr1.use_count() << endl;    // 0
cout << "use count of sptr2 is now " << dptr2.use_count() << endl;    // 1

// circle* -> circle* 使用dynamic_cast转换后,指针不为空,此时资源被两者共同使用,引用计数为2
shared_ptr<shape> sptr3(new circle);
//    shared_ptr<circle> sptr3(new circle);     // 上面或者当前行,后面输出一样！
shared_ptr<circle> sptr1_1 = static_pointer_cast<circle>(sptr3);      // 基类转子类

cout << "use count of sptr1_1 is now " << sptr1_1.use_count() << endl;    // 2
cout << "use count of sptr3 is now " << sptr3.use_count() << endl;    // 2

// circle* -> circle* 使用static_cast转换后,指针不为空,此时资源被两者共同使用,引用计数为2 等价于dynamic_cast
shared_ptr<circle> sptr3_1(new circle);
shared_ptr<shape> sptr2_1 = static_pointer_cast<shape>(sptr3_1);      // 子类转基类 上行转换,安全！

cout << "use count of sptr2_1 is now " << sptr2_1.use_count() << endl;    // 2
cout << "use count of sptr3_1 is now " << sptr3_1.use_count() << endl;    // 2
```

输出结果同上dynamic_cast，不同之处，在下行转换的时候(基类转子类)，是不安全的！

### 4.3 const_cast

去掉const属性：

```cpp
template<typename T, typename U>
shared_ptr<T> const_pointer_cast(
        const shared_ptr<U> &other) noexcept {
    T *ptr = const_cast<T *>(other.get());
    return shared_ptr<T>(other, ptr);
}
```

调用：

```cpp
shared_ptr<circle> s = const_pointer_cast<circle>(shared_ptr<const circle>(new circle));
```

### 4.4 reinterpret_cast

例如：想把一个指针转为整数，就可以用reinterpret_cast。

```cpp
template<typename T, typename U>
shared_ptr<T> reinterpret_pointer_cast(
        const shared_ptr<U> &other) noexcept {
    T *ptr = reinterpret_cast<T *>(other.get());
    return shared_ptr<T>(other, ptr);
}
```

调用：

```cpp
int a = reinterpret_pointer_cast<int>(s);
```

