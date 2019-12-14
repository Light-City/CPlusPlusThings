//
// Created by light on 19-12-9.
//
#include "../RAII/shape.h"
template<typename T>
class auto_ptr {
public:
    explicit auto_ptr(
            T *ptr = nullptr) noexcept
            : ptr_(ptr) {}

    ~auto_ptr() noexcept {
        delete ptr_;
    }

    T &operator*() const noexcept { return *ptr_; }

    T *operator->() const noexcept { return ptr_; }

    operator bool() const noexcept { return ptr_; }

    T *get() const noexcept { return ptr_; }

    // 拷贝构造,被复制放释放原来指针的所有权,交给复制方   始终只有一个对象管理一块空间
    auto_ptr(auto_ptr &other) noexcept {
        ptr_ = other.release();
    }

    // copy and swap  始终只有一个对象有管理这块空间的权限
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

    scoped_ptr<shape> sptr1{create_shape(shape_type::circle)};
//    scoped_ptr<shape> sptr2{sptr1};       // error  不可拷贝

}