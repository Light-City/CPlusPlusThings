/**
 * @file vir_con.cpp
 * @brief 构造函数不可以声明为虚函数。同时除了inline之外，构造函数不允许使用其它任何关键字。
 *
 * 为什么构造函数不可以为虚函数？
 *
 * 尽管虚函数表vtable是在编译阶段就已经建立的，但指向虚函数表的指针vptr是在运行阶段实例化对象时才产生的。 如果类含有虚函数，编译器会在构造函数中添加代码来创建vptr。 问题来了，如果构造函数是虚的，那么它需要vptr来访问vtable，可这个时候vptr还没产生。 因此，构造函数不可以为虚函数。
 * 我们之所以使用虚函数，是因为需要在信息不全的情况下进行多态运行。而构造函数是用来初始化实例的，实例的类型必须是明确的。
 * 因此，构造函数没有必要被声明为虚函数。
 * 尽管构造函数不可以为虚函数，但是有些场景下我们确实需要 “Virtual Copy Constructor”。 “虚复制构造函数”的说法并不严谨，其只是一个实现了对象复制的功能的类内函数。 举一个应用场景，比如剪切板功能。 复制内容作为基类，但派生类可能包含文字、图片、视频等等。 我们只有在程序运行的时候才知道我们需要复制的具体是什么类型的数据。
 *
 * @author 光城
 * @version v1
 * @date 2019-07-24
 */

#include <iostream> 
using namespace std; 

//// LIBRARY SRART 
class Base 
{ 
    public: 
        Base() { } 

        virtual // Ensures to invoke actual object destructor 
            ~Base() { } 

        virtual void ChangeAttributes() = 0; 

        // The "Virtual Constructor" 
        static Base *Create(int id); 

        // The "Virtual Copy Constructor" 
        virtual Base *Clone() = 0; 
}; 

class Derived1 : public Base 
{ 
    public: 
        Derived1() 
        { 
            cout << "Derived1 created" << endl; 
        } 

        Derived1(const Derived1& rhs) 
        { 
            cout << "Derived1 created by deep copy" << endl; 
        } 

        ~Derived1() 
        { 
            cout << "~Derived1 destroyed" << endl; 
        } 

        void ChangeAttributes() 
        { 
            cout << "Derived1 Attributes Changed" << endl; 
        } 

        Base *Clone() 
        { 
            return new Derived1(*this); 
        } 
}; 

class Derived2 : public Base 
{ 
    public: 
        Derived2() 
        { 
            cout << "Derived2 created" << endl; 
        } 

        Derived2(const Derived2& rhs) 
        { 
            cout << "Derived2 created by deep copy" << endl; 
        } 

        ~Derived2() 
        { 
            cout << "~Derived2 destroyed" << endl; 
        } 

        void ChangeAttributes() 
        { 
            cout << "Derived2 Attributes Changed" << endl; 
        } 

        Base *Clone() 
        { 
            return new Derived2(*this); 
        } 
}; 

class Derived3 : public Base 
{ 
    public: 
        Derived3() 
        { 
            cout << "Derived3 created" << endl; 
        } 

        Derived3(const Derived3& rhs) 
        { 
            cout << "Derived3 created by deep copy" << endl; 
        } 

        ~Derived3() 
        { 
            cout << "~Derived3 destroyed" << endl; 
        } 

        void ChangeAttributes() 
        { 
            cout << "Derived3 Attributes Changed" << endl; 
        } 

        Base *Clone() 
        { 
            return new Derived3(*this); 
        } 
}; 

// We can also declare "Create" outside Base. 
// But is more relevant to limit it's scope to Base 
Base *Base::Create(int id) 
{ 
    // Just expand the if-else ladder, if new Derived class is created 
    // User need not be recompiled to create newly added class objects 

    if( id == 1 ) 
    { 
        return new Derived1; 
    } 
    else if( id == 2 ) 
    { 
        return new Derived2; 
    } 
    else
    { 
        return new Derived3; 
    } 
} 
//// LIBRARY END 

//// UTILITY SRART 
class User 
{ 
    public: 
        User() : pBase(0) 
    { 
        // Creates any object of Base heirarchey at runtime 

        int input; 

        cout << "Enter ID (1, 2 or 3): "; 
        cin >> input; 

        while( (input != 1) && (input != 2) && (input != 3) ) 
        { 
            cout << "Enter ID (1, 2 or 3 only): "; 
            cin >> input; 
        } 

        // Create objects via the "Virtual Constructor" 
        pBase = Base::Create(input); 
    } 

        ~User() 
        { 
            if( pBase ) 
            { 
                delete pBase; 
                pBase = 0; 
            } 
        } 

        void Action() 
        { 
            // Duplicate current object 
            Base *pNewBase = pBase->Clone(); 

            // Change its attributes 
            pNewBase->ChangeAttributes(); 

            // Dispose the created object 
            delete pNewBase; 
        } 

    private: 
        Base *pBase; 
}; 

//// UTILITY END 

//// Consumer of User (UTILITY) class 
int main() 
{ 
    User *user = new User(); 

    user->Action(); 

    delete user; 
} 

