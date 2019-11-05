/**
 * @file union.cpp
 * @brief UNION
 * @author 光城
 * @version v1
 * @date 2019-08-06
 */

#include<iostream>
/**
 * 默认访问控制符为public
 */
union UnionTest {
    /**
     * 可以含有构造函数、析构函数
     */
    UnionTest() : i(10) {print(i);};
    ~UnionTest(){};
    int i; 
private:
    void print(int i){std::cout<<i<<std::endl;};
};
/**
 * 全局匿名联合必须是静态的 
 */
static union {
    int i;
    double d;
};

int main() {
    UnionTest u;

    union {
        int i;
        double d;
    };

    std::cout << u.i << std::endl;  // 输出 UnionTest 联合的 10

    ::i = 20;
    std::cout << ::i << std::endl;  // 输出全局静态匿名联合的 20
    /**
     * 匿名union在定义所在作用域可直接访问union成员
     */
    i = 30;
    std::cout << i << std::endl;    // 输出局部匿名联合的 30

    return 0;
}
