/**
 * @file copy_construct.cpp
 * @brief g++ -o copy_construct copy_construct.cpp -fno-elide-constructors
 * -fno-elide-constructors选项(关闭返回值优化)
 * @author 光城
 * @version v1
 * @date 2019-08-09
 */

#include <iostream>
using namespace std;

class Copyable {
public:
    Copyable(){}
    Copyable(const Copyable &o) {
        cout << "Copied" << endl;
    }
};
Copyable ReturnRvalue() {
    return Copyable(); //返回一个临时对象
}
void AcceptVal(Copyable a) {

}
void AcceptRef(const Copyable& a) {

}

int main() {
    cout << "pass by value: " << endl;
    AcceptVal(ReturnRvalue()); // 应该调用两次拷贝构造函数
    cout << "pass by reference: " << endl;
    AcceptRef(ReturnRvalue()); //应该只调用一次拷贝构造函数
}
