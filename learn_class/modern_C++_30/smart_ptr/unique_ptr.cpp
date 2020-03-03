//
// Created by light on 19-12-12.
//

#include "../RAII/shape.h"

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
        cout << "move ctor" << endl;
        ptr_ = other.release();
    }

    template<typename U>
    unique_ptr(unique_ptr<U> &&other) noexcept {
        cout << "U move ctor" << endl;
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