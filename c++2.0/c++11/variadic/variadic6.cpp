//
// Created by light on 19-11-4.
//

#include <iostream>


using namespace std;
// 上一个例子:variadic5.cpp为递归调用
// 当前这个例子为递归继承
namespace light {
    template<typename...Values>
    class tuple;

    template<>
    class tuple<> {
    };

    template<typename Head, typename...Tail>
    class tuple<Head, Tail...> : private tuple<Tail...> {
        typedef tuple<Tail...> inherited;
    protected:
        Head m_head;
    public:
        tuple() {}

        tuple(Head h, Tail...tail) : m_head(h), inherited(tail...) {}

        // decltype()中的m_head必须放到前面,否则编译器找不到
        auto head() -> decltype(m_head) { return m_head; }
        // 或者 Head head()  { return m_head; }

        inherited &tail() { return *this; }
    };
}
/**
 * string 32   8字节对齐
 * float  4
 * int    4
 * 4+4+32=40 自底向上
 */

int main() {
    using light::tuple;
    tuple<int, float, string> t(41, 6.3, "nico");
    cout << sizeof(t) << endl;
    cout << t.head() << endl;
    cout << t.tail().head() << endl;
    cout << t.tail().tail().head() << endl;

    return 0;
}
