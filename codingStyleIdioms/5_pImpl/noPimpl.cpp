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
    C c;
    D d;
};


int main() {

    X x;
}
