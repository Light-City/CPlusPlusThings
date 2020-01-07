//
// Created by light on 20-1-6.
//

/*
 The compiler will try this overload since it's less generic than the variadic.
 T will be replace by int which gives us void f(const int& t, int::iterator* b = nullptr);
 int doesn't have an iterator sub-type, but the compiler doesn't throw a bunch of errors.
 It simply tries the next overload.

 编译器尝试此重载,因为模板化函数比可变参数函数更精确(通用)。T将被int取代，这将使我们得到
 void f(const int& t, int::iterator* b = nullptr); int 没有迭代器子类型，但是编译器不会抛出一堆错误。
 它只是尝试下一个重载。
*/
template<typename T>
void f(const T &t, typename T::iterator *it = nullptr) {}

// The sink-hole.
void f(...) {}

int main() {
    f(1); // Calls void f(...) { }
}
