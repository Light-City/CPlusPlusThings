//
// Created by light on 19-11-3.
//

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

// [introducer](optional)mutable throwSpec->retType {}
// mutable决定[]能够被改写  mutable throwSpec retType都是选择的,只要有一个存在就得写()
// retType 返回类型
// ()放参数
// []放外面变量 passed by value or reference


class UnNamedLocalFunction {
private:
    int localVar;
public:
    UnNamedLocalFunction(int var) : localVar(var) {}

    bool operator()(int val) {
        return val == localVar;
    }
};

class Person {
public:
    string firstname;
    string lastname;
};

class LambdaFunctor {
public:
    LambdaFunctor(int a, int b) : m_a(a), m_b(b) {}

    bool operator()(int n) const {
        return m_a < n && n < m_b;
    }

private:
    int m_a;
    int m_b;
};

class X {
private:
    int __x, __y;
public:
    X(int x, int y) : __x(x), __y(y) {}

    int operator()(int a) { return a; }

    int f() {
        // 下列 lambda 的语境是成员函数 X::f
        // 对于[=]或[&]的形式，lambda 表达式可以直接使用 this 指针
        return [&]() -> int {
            return operator()(this->__x + __y); // X::operator()(this->x + (*this).y)
            // 拥有类型 X*
        }();
    }


    int ff() {
        return [this]() {
            return this->__x;
        }();
    }
};

int main() {

    [] {
        cout << "hello" << endl;
    }();

    auto I = [] {
        cout << "hello" << endl;
    };
    I();
    int id = 0;
    // 先看前面的id 如果没有mutable error: increment of read-only variable ‘id’
    auto f = [id]()mutable {
        cout << "id=" << id << endl;
        ++id;
    };
    id = 42;
    f();    // 0
    f();    // 1
    f();    // 2
    cout << id << endl;

    // 上述lambda就相当于
//    class Functor {
//    private:
//        int id;
//    public:
//        void operator() {
//            cout << "id=" << id << endl;
//            ++id;
//        }
//    };
//    Functor f;

    int id1 = 0;
    // 加不加mutable没影响,且传引用只要后面id1被修改了,就会使用修改后的值进行操作
    auto f1 = [&id1]() {
        cout << "id1=" << id1 << endl;
        ++id1;
    };
    id1 = 42;
    f1();    // 42
    f1();    // 43
    f1();    // 44
    cout << id1 << endl;


    // 传参与返回
    int id2 = 0;
    auto f2 = [&id2](int &param) {
        cout << "id2=" << id2 << endl;
        ++id2;
        ++param;
        cout << "param=" << param << endl;
        static int x = 5;
        return id2;
    };
    int param = 1;
    f2(param);
    cout << "param=" << param << endl;

    // [=] =表示默认以by value传递外部所有变量
    // [&] &表示默认以by reference传递外部所有变量
    auto f3 = [&]() {
        cout << "id=" << id << endl;
        cout << "id1=" << id1 << endl;
        cout << "id2=" << id2 << endl;
        cout << "param=" << param << endl;
    };
    f3();

    // 一部分传引用,其余传值
    cout << "id=" << id << endl;
    auto f4 = [=, &id]() {       // =不可以放在&id后面
        cout << "id=" << id << endl;
        id++;
        cout << "id1=" << id1 << endl;
        cout << "id2=" << id2 << endl;
        cout << "param=" << param << endl;
    };
    f4();

    // 一部分传值,其余传引用
    cout << "id=" << id << endl;
    auto f5 = [&, id]() {       // &不可以放在id后面
        cout << "id=" << id << endl;
        cout << "id1=" << id1 << endl;
        cout << "id2=" << id2 << endl;
        cout << "param=" << param << endl;
    };
    f5();
    // this 指针
    X x_(1, 2);
    cout << "x_.f()=" << x_.f() << endl;   // 1+2=3
    cout << "x_.ff()=" << x_.ff() << endl; // 1



    // 下面lambda函数等价于上述的UnNamedLocalFunction
    int tobefound = 5;
    auto lambda1 = [tobefound](int val) {
        return val == tobefound;
    };
    bool b1 = lambda1(5);
    UnNamedLocalFunction lambda2(tobefound);
    bool b2 = lambda2(5);
    cout << b1 << " " << b2 << endl;

    auto ll1 = [](int x) -> int {
        return x + 10;
    };
    // lambda 匿名函数
    function<int(int x)> ll = [](int x) -> float {
        return x + 10.0;
    };
    cout<<ll(1)<<endl;

    // decltype+lambda
    // 比大小
    function<bool(const Person&p1,const Person&p2)> cmp = [](const Person &p1, const Person &p2) {
        return p1.lastname < p2.lastname;
    };


    // 对于lambda,我们往往只有object,很少有人能够写出它的类型，而有时就需要知道它的类型,要获得其type,就要借助其decltype
    set<Person, decltype(cmp)> col(cmp);

    // 要申明lambda对象的类型，可以使用template或者auto进行自动推导。
    // 如果需要知道类型，可以使用decltype，比如，让lambda函数作为关联容器或者无序容器的排序函数或者哈希函数。
    // 上面代码给出了事例（decltype的第三种用法中的事例），定义了一个lambda函数用cmp表示，用来比较Person对象的大小，传入到Set容器中去，
    // 但根据右边的set容器的定义，我们传入的不仅是cmp（构造函数），还要传入模板的cmp类型（Set内部需要声明cmp类型），
    // 所以必须使用decltype来推导出类型。
    // （如果没有向构造函数传入cmp，调用的是默认的构造函数，也就是set() : t(Compare()), 这里会报错, 现在不会出问题了!
    // 因为Compare()指的是调用默认的lambda构造函数，而lambda函数没有默认构造函数和赋值函数）


    vector<int> vec{5, 28, 50, 83, 70, 590, 245, 59, 24};
    int x = 30, y = 100;
    // 函数对象是很强大的，封装代码和数据来自定义标准库的行为，但需要写出函数对象需要写出整个class，这是不方便的，而且是非本地的，
    // 用起来也麻烦，需要去看怎样使用，另外编译出错的信息也不友好，而且它们不是inline的，效率会低一些（算法效率还是最重要的）。
//    vec.erase(remove_if(vec.begin(), vec.end(), LambdaFunctor(x, y)), vec.end());
//    for(auto i:vec) cout<<i<<" ";
//    cout<<endl;
    // 而lambda函数的提出解决了这个问题，简短有效清晰，上面的事例很好的说明了这个问题，用lambda要简短许多，功能一样，很直观。
    vec.erase(remove_if(vec.begin(), vec.end(), [x, y](int n) { return x < n && n < y; }), vec.end());
    for_each(vec.begin(), vec.end(), [](int i) { cout << i << " "; });
    cout << endl;
    return 0;

}
