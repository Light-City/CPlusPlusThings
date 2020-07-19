/**
 * @file virtual_function.cpp
 * @brief 虚函数可以被私有化，但有一些细节需要注意。
 * 基类指针指向继承类对象，则调用继承类对象的函数；
 * int main()必须声明为Base类的友元，否则编译失败。 编译器报错： ptr无法访问私有函数。 
 * 当然，把基类声明为public， 继承类为private，该问题就不存在了。----> 见另外一个例子！
 * @author 光城
 * @version v1
 * @date 2019-07-24
 */

#include<iostream> 
using namespace std; 

class Derived; 

class Base { 
    private: 
        virtual void fun() { cout << "Base Fun"; } 
        friend int main(); 
}; 

class Derived: public Base { 
    public: 
        void fun() { cout << "Derived Fun"; } 
}; 

int main() 
{ 
    Base *ptr = new Derived; 
    ptr->fun(); 
    return 0; 
}
