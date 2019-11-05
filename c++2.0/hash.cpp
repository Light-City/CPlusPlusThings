//
// Created by light on 19-11-4.
//

#include <iostream>
#include <unordered_set>
#include <complex>


using namespace std;

class Customer {

};

// 形式一 通过一个类，重载了操作符()的方式，形成了一个仿函数（函数对象）
class CustomerHash {
public:
    size_t operator()(const Customer &c) const {
//        return ...;
    }
};

// 形式二 这个方法实际是通过指定了一个函数指针的方式来通知容器，对于这个自定义类到底应该使用哪个hash function
size_t customer_hash_func(const Customer &t) {
//    return ...;
}
// 形式三 以struct hash 偏特化形式实现hash function
namespace std {     // 必须放在std里面
    template<>
    struct hash<Customer> : public __hash_base<size_t, Customer> {      // 继承不继承都可以!
        size_t
        operator()(const Customer&s) const noexcept {
//            return ...;
        }
    };
}


// 万能的HashFunction 使用variadic templates

// (4)
template<typename T>
inline void hash_combine(size_t& seed, const T& val){
    seed = std::hash<T>()(val) + 0x9e3779b9
           + (seed << 6) + (seed >> 2);
}

// (3)
template<typename T>
inline void hash_val(size_t& seed, const T& val){
    hash_combine(seed, val);
}

// (2)
template<typename T, typename... Types>
inline void hash_val(size_t& seed, const T& val, const Types&... args){
    hash_combine(seed, val);
    hash_val(seed, args...);
}
// (1)
template<typename... Types>
inline size_t hash_val(const Types&... args){
    size_t seed = 0;
    hash_val(seed, args...);
    return seed;
}


int main() {
    unordered_set<Customer, CustomerHash> unorderedSet;
    unordered_set<Customer,size_t(*)(const Customer&)>custset(20,customer_hash_func);
    unordered_set<Customer> unset;
    int *p= reinterpret_cast<int *>(new Customer());
    cout<<hash_val(p)<<endl;

    return 0;
}
