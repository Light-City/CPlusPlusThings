//
// Created by light on 19-12-25.
//

#include <iostream>
#include <cassert>

int main() {
    const int alignment=5;
    assert((alignment & (alignment - 1)) == 0);
    static_assert((alignment & (alignment - 1)) == 0, "Alignment must be power of two");
    return 0;
}