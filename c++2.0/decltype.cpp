//
// Created by light on 19-11-3.
//

#include <iostream>
#include <map>
#include <set>

using namespace std;

// 1.used to declare return tyoes  返回类型

// c++2.0之前编译不出来
//template<typename T1, typename T2>
//decltype(x + y) add(T1 x, T2 y);

// 编译器编不出来在2.0之后变成下面这个,先用auto暂定,后面返回类型为decltype(x+y)
template<typename T1, typename T2>
auto add(T1 x, T2 y) -> decltype(x + y) {       // -> 也用在lambda
    return x + y;
}

class Person {
public:
    string firstname;
    string lastname;
};
int main() {

    // 1.used to declare return tyoes
    cout << add(1, 2) << endl;

    // 2.模板元编程 例如在一个模板函数或类获取容器的value_type,这里就不封装了,直接写在main函数里面
    // 获得表达式的type 有点像typeof()特点
    map<string, float> coll;
    // 获取上述类型
    decltype(coll)::value_type m{"as", 1};  // value_type为pair<string,int> m
    cout << m.first << " " << m.second << endl;
    pair<string, int> p{"a", 2};
    cout << p.first << " " << p.second << endl;
    // 3.used to pass the type of a lambda

    // 比大小
    auto cmp = [](const Person &p1, const Person &p2) {
        return p1.lastname< p2.lastname;
    };

    // 对于lambda,我们往往只有object,很少有人能够写出它的类型，而有时就需要知道它的类型,要获得其type,就要借助其decltype
    set<Person, decltype(cmp)> col(cmp);

    return 0;
}