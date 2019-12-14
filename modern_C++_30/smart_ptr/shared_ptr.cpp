//
// Created by light on 19-12-12.
//

#include "../RAII/shape.h"
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

    // 实现强制类型转换需要的构造函数
    template<typename U>
    shared_ptr(const shared_ptr<U> &other, T *ptr) noexcept {
        ptr_ = ptr;
        if (ptr_) {
            other.shared_count_->add_count();
            shared_count_ = other.shared_count_;
        }
    }

    ~shared_ptr() noexcept {
        // 最后一个shared_ptr再去删除对象与共享计数
        // ptr_不为空且此时共享计数减为0的时候,再去删除
        if (ptr_ && !shared_count_->reduce_count()) {
            delete ptr_;
            delete shared_count_;
        }
    }

    T &operator*() const noexcept { return *ptr_; }

    T *operator->() const noexcept { return ptr_; }

    operator bool() const noexcept { return ptr_; }

    T *get() const noexcept { return ptr_; }

    // 带模板的拷贝与移动构造函数 模板的各个实例间并不天然就有 friend 关系，因而不能互访私有成员 ptr_ 和 shared_count_。
    // 需要下面显示声明
    template<typename U>
    friend
    class shared_ptr;

    template<typename U>
    shared_ptr(const shared_ptr<U> &other) noexcept {
//        cout << "调用了带模板的拷贝构造!" << endl;
        ptr_ = other.ptr_;
        if (ptr_) {
            other.shared_count_
                    ->add_count();
            shared_count_ =
                    other.shared_count_;
        }
    }

    template<typename U>
    shared_ptr(shared_ptr<U> &&other) noexcept {
//        cout << "调用了带模板的移动构造!" << endl;
        ptr_ = other.ptr_;
        if (ptr_) {
            shared_count_ =
                    other.shared_count_;
            other.ptr_ = nullptr;
            other.shared_count_ = nullptr;
        }
    }


    // copy and swap  始终只有一个对象有管理这块空间的权限
    shared_ptr &operator=(shared_ptr rhs) noexcept {
        rhs.swap(*this);
        return *this;
    }

    void swap(shared_ptr &rhs) noexcept {
        using std::swap;
        swap(ptr_, rhs.ptr_);
        swap(shared_count_, rhs.shared_count_);
    }

    long use_count() const noexcept {
        if (ptr_) {
            return shared_count_->get_count();
        } else {
            return 0;
        }
    }

private:
    T *ptr_;
    shared_count *shared_count_;
};

template<typename T>
void swap(shared_ptr<T> &lhs, shared_ptr<T> &rhs) noexcept {
    lhs.swap(rhs);
}

template<typename T, typename U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U> &other) noexcept {
    T *ptr = dynamic_cast<T *>(other.get());
    return shared_ptr<T>(other, ptr);
}

template<typename T, typename U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U> &other) noexcept {
    T *ptr = static_cast<T *>(other.get());
    return shared_ptr<T>(other, ptr);
}

template<typename T, typename U>
shared_ptr<T> const_pointer_cast(
        const shared_ptr<U> &other) noexcept {
    T *ptr = const_cast<T *>(other.get());
    return shared_ptr<T>(other, ptr);
}

template<typename T, typename U>
shared_ptr<T> reinterpret_pointer_cast(
        const shared_ptr<U> &other) noexcept {
    T *ptr = reinterpret_cast<T *>(other.get());
    return shared_ptr<T>(other, ptr);
}

int main() {
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
    shared_ptr<const int> constV(new int);
    shared_ptr<int> s = const_pointer_cast<int>(constV);
    *s =10;
    int a = reinterpret_pointer_cast<int>(s);
    cout<<a<<endl;
}