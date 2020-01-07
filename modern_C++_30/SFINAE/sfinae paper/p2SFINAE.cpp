//
// Created by light on 20-1-6.
//


// The compiler will be really unhappy when it will later discover the call to hahahaICrash.
// 当以后发现对hahahaICrash的调用时，编译器将非常不满意。
template <typename T> void f(T t) { t.hahahaICrash(); }
void f(...) { } // The sink-hole wasn't even considered.

int main() {
    f(1);
}