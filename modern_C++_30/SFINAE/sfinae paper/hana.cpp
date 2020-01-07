//
// Created by light on 20-1-6.
//
// https://www.boost.org/doc/libs/1_62_0/libs/hana/doc/html/structboost_1_1hana_1_1type.html#a2d2e7e08e284f7e0bd1bd9c3ad0e0a2b
#include <boost/hana.hpp>
#include <vector>
#include <string>

namespace hana = boost::hana;


int main() {
    // 检查成员
    struct Person {
        std::string name;
    };
    auto has_name = hana::is_valid([](auto &&p) -> decltype(p.name) {});

    Person joe{"Joe"};
    static_assert(has_name(joe), "");
    static_assert(!has_name(1), "");
    // 检查嵌套类型
    auto has_value_type = hana::is_valid([](auto t) -> hana::type<typename decltype(t)::type::value_type> {});
    static_assert(has_value_type(hana::type_c<std::vector<int>>), "");
    static_assert(!has_value_type(hana::type_c<Person>), "");

    return 0;
}