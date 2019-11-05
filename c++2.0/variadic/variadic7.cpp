//
// Created by light on 19-11-4.
//

#include <iostream>

using namespace std;

namespace light {
    template<typename...Values>
    class tuple;

    template<>
    class tuple<> {
    };

    template<typename Head, typename...Tail>
    class tuple<Head, Tail...> {
        typedef tuple<Tail...> composited;
    protected:
        Head m_head;
        composited m_tai;
    public:
        tuple() {}
        int get() { return sizeof(composited);}
        tuple(Head h, Tail...tail) : m_head(h), m_tai(tail...) {}

        // decltype()中的m_head必须放到前面,否则编译器找不到
        auto head() -> decltype(m_head) { return m_head; }
        // 或者 Head head()  { return m_head; }

        composited &tail() { return m_tai; }
    };
}

// 根据string为8字节对齐,我们得出8字节对齐
// 第一次 int 4字节Head调整到边界8 那composite呢?继续往下分析
// 第二次 把第一次的composited拆分为float与一包, float也占4字节,调整到8边界为8字节,那这一步的composited呢?继续往下分析
// 第三次 把第二次的compoisted拆分成sting与一包, string占32字节,已经为8倍数,无需调整,而这一步的composited呢?由于到这里一包已经没了,就会调用它的全特化版本,全特环版本为空,sizeof为1,
// 调整到8的倍数为8.
// 因此最后的内存结构(自底向上)为：8 + 8 + 32 + 8 = 56     64位机器结果

struct A {
    string a;
    float b;
};
int main() {
    using light::tuple;
    tuple<int, float, string> t(41, 6.3, "nico");
    cout<<sizeof(A)<<endl;
    cout<<t.get()<<endl;
    cout << sizeof(t) << endl;
    cout << t.head() << endl;
    cout << t.tail().head() << endl;
    cout << t.tail().tail().head() << endl;
    return 0;
}