//
// Created by light on 19-12-16.
//


#include <complex>        // std::complex
#include <iostream>       // std::cout/endl
#include <unordered_map>  // std::unordered_map
#include <unordered_set>  // std::unordered_set
#include "../container1/output_container.h"

using namespace std;

namespace std {

    template <typename T>
    struct hash<complex<T>> {
        size_t
        operator()(const complex<T>& v) const
        noexcept
        {
            hash<T> h;
            return h(v.real()) + h(v.imag());
        }
    };

}  // namespace std

int main()
{
    unordered_set<int> s{
            1, 1, 2, 3, 5, 8, 13, 21
    };
    cout << s << endl;

    unordered_map<complex<double>,
            double>
            umc{{{1.0, 1.0}, 1.4142},
                {{3.0, 4.0}, 5.0}};
    cout << umc << endl;
}