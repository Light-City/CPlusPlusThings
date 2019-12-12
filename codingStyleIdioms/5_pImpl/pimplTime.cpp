#include <iostream>
#include <vector>
using namespace std;

class C {
    vector<int> v;
    string s;
};
class D {
    string s;
};

class X {
private:
    struct XImpl;
    XImpl* pImpl;
};

struct X::XImpl {
    C c;
    D d;
};

int main() {
    X x;
}
