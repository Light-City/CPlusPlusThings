//
// Created by light on 19-12-9.
//
#include <iostream>

// copy and swap : https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
// ADL : https://stackoverflow.com/questions/8111677/what-is-argument-dependent-lookup-aka-adl-or-koenig-lookup

namespace A {
    template<typename T>
    class smart_ptr {
    public:
        smart_ptr() noexcept : ptr_(new T()) {

        }

        smart_ptr(const T &ptr) noexcept : ptr_(new T(ptr)) {

        }

        smart_ptr(smart_ptr &rhs) noexcept {
            std::cout << "copy ctor" << std::endl;
            ptr_ = rhs.release();       // 释放所有权,此时rhs的ptr_指针为nullptr
        }

        // 方法1：为了避免自赋值,通常采用下面写法   不好!  不具备异常安全,只具备自我赋值安全性
//        smart_ptr &operator=(const smart_ptr &rhs) {
//            if (*this != rhs) {
//                delete ptr_;
//                ptr_ = new T(rhs.ptr_);  // 当new 发生异常,此时ptr_指向的而是一块被删除区域,而不是被赋值对象的区域
//                return *this;
//            }
//            return *this;
//        }
        // 方法2：如果new出现异常,ptr_会保持原装!  也可以处理自我赋值! 还是不够好!
//        smart_ptr &operator=(const smart_ptr &rhs) {
//            T *origin = ptr_;
//            ptr_ = new T(rhs.ptr_);
//            delete origin;
//            return *this;
//        }
        // 方法3：copy and swap 很好!
//        smart_ptr &operator=(smart_ptr &rhs) noexcept {
//            smart_ptr tmp(rhs);
//            swap(tmp);
//            return *this;
//        }

        // 方法4：同方法3,改为传值
        // 既适用于copy ctor也适用于 move ctor
        smart_ptr &operator=(smart_ptr rhs) noexcept {
            swap(rhs);
            return *this;
        }
        // move ctor
        smart_ptr(smart_ptr &&rhs) noexcept {
            std::cout << "move ctor" << std::endl;
            ptr_ = rhs.ptr_;
            if (ptr_)
                rhs.ptr_ = nullptr;
        }

        // move assignment
//        smart_ptr &operator=(smart_ptr &&rhs) noexcept {
//            std::cout << "move assignment" << std::endl;
//            smart_ptr tmp(rhs);
//            swap(rhs);
//            return *this;
//        }

        void swap(smart_ptr &rhs) noexcept { // noexcept == throw() 保证不抛出异常
            using std::swap;
            swap(ptr_, rhs.ptr_);
        }

        T *release() noexcept {
            T *ptr = ptr_;
            ptr_ = nullptr;
            return ptr;
        }

        T *get() const noexcept {
            return ptr_;
        }

    private:
        T *ptr_;
    };

// 提供一个非成员swap函数for ADL(Argument Dependent Lookup)
    template<typename T>
    void swap(A::smart_ptr<T> &lhs, A::smart_ptr<T> &rhs) noexcept {
        lhs.swap(rhs);
    }
}
// 注释开启,会引发ADL冲突
//namespace std {
//    // 提供一个非成员swap函数for ADL(Argument Dependent Lookup)
//    template<typename T>
//    void swap(A::smart_ptr<T> &lhs, A::smart_ptr<T> &rhs) noexcept {
//        lhs.swap(rhs);
//    }
//
//}

int main() {

    using std::swap;
    A::smart_ptr<std::string> s1("hello"), s2("world");
    // 交换前
    std::cout << *s1.get() << " " << *s2.get() << std::endl;
    swap(s1, s2);      // 这里swap 能够通过Koenig搜索或者说ADL根据s1与s2的命名空间来查找swap函数
    // 交换后
    std::cout << *s1.get() << " " << *s2.get() << std::endl;
//    s1 = s2;

    A::smart_ptr<std::string> s3 = s1;
    A::smart_ptr<std::string> s4 = std::move(s1);
}