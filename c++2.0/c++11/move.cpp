//
// Created by light on 19-11-4.
//

#include <iostream>
#include <cstring>
#include <vector>
#include <list>
#include <typeinfo>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class MyStringNoMove {
public:
    static size_t DCtor;    // default-ctor
    static size_t Ctor;     // ctor
    static size_t CCtor;    // copy-ctor
    static size_t CAsgn;    // copy-assignment
    static size_t MCtor;    // move-ctor
    static size_t MAsgn;    // move-assignment
    static size_t Dtor;     // dtor
private:
    char *_data;
    size_t _len;

    void _init_data(const char *s) {
        _data = new char[_len];
        memcpy(_data, s, _len);
        _data[_len] = '\0';
    };
public:
    MyStringNoMove() : _data(NULL), _len(0) { ++DCtor; }

    MyStringNoMove(const char *p) : _len(strlen(p)) {
        ++Ctor;
        _init_data(p);
    }

    // copy ctor
    MyStringNoMove(const MyStringNoMove &str) {
        ++CCtor;
        _init_data(str._data);
    }


    // copy assignment
    MyStringNoMove &operator=(const MyStringNoMove &str) {
        ++CAsgn;
        if (this != &str) {
            if (_data) delete _data;
            _len = str._len;
            _init_data(str._data);
        } else {

        }
        return *this;
    }

    ~MyStringNoMove() {       // dtor 默认是noexcept
        ++Dtor;
        if (_data)
            delete _data;
    }

    bool
    operator<(const MyStringNoMove &rhs) const {
        return string(this->_data) < string(rhs._data);
    }

    bool
    operator==(const MyStringNoMove &rhs) const {
        return string(this->_data) == string(rhs._data);
    }

    char *get() const {
        return _data;
    }
};

size_t MyStringNoMove::DCtor = 0;    // default-ctor
size_t MyStringNoMove::Ctor = 0;     // ctor
size_t MyStringNoMove::CCtor = 0;    // copy-ctor
size_t MyStringNoMove::CAsgn = 0;    // copy-assignment
size_t MyStringNoMove::MAsgn = 0;    // move-assignment
size_t MyStringNoMove::MCtor = 0;    // move-ctor
size_t MyStringNoMove::Dtor = 0;     // dtor
// move aware class

class MyString {
public:
    static size_t DCtor;    // default-ctor
    static size_t Ctor;     // ctor
    static size_t CCtor;    // copy-ctor
    static size_t CAsgn;    // copy-assignment
    static size_t MCtor;    // move-ctor
    static size_t MAsgn;    // move-assignment
    static size_t Dtor;     // dtor
private:
    char *_data;
    size_t _len;

    void _init_data(const char *s) {
        _data = new char[_len];
        memcpy(_data, s, _len);
        _data[_len] = '\0';
    };
public:
    MyString() : _data(NULL), _len(0) { ++DCtor; }

    MyString(const char *p) : _len(strlen(p)) {
        ++Ctor;
        _init_data(p);
    }

    // copy ctor
    MyString(const MyString &str) {
        ++CCtor;
        _init_data(str._data);
    }

    // move ctor
    MyString(MyString &&str) noexcept: _data(str._data), _len(str._len) {
        ++MCtor;
        str._len = 0;
        str._data = NULL;   // 重要
    }

    // copy assignment
    MyString &operator=(const MyString &str) {
        ++CAsgn;
        if (this != &str) {
            if (_data) delete _data;
            _len = str._len;
            _init_data(str._data);
        } else {

        }
        return *this;
    }

    // move assignment
    MyString &operator=(MyString &&str) {
        ++MAsgn;
        if (this != &str) {
            if (_data) delete _data;
            _data = str._data;
            _len = str._len;
            str._len = 0;
            str._data = NULL;
        } else {

        }
        return *this;
    }

    ~MyString() {       // dtor 默认是noexcept
        ++Dtor;
        if (_data)
            delete _data;
    }

    bool
    operator<(const MyString &rhs) const {
        return string(this->_data) < string(rhs._data);
    }

    bool
    operator==(const MyString &rhs) const {
        return string(this->_data) == string(rhs._data);
    }

    char *get() const {
        return _data;
    }
};

size_t MyString::DCtor = 0;    // default-ctor
size_t MyString::Ctor = 0;     // ctor
size_t MyString::CCtor = 0;    // copy-ctor
size_t MyString::CAsgn = 0;    // copy-assignment
size_t MyString::MAsgn = 0;    // move-assignment
size_t MyString::MCtor = 0;    // move-ctor
size_t MyString::Dtor = 0;     // dtor
namespace std {
    template<>
    struct hash<MyStringNoMove> : public __hash_base<size_t, MyStringNoMove> {
        size_t
        operator()(const MyStringNoMove &s) const noexcept {
            return hash<string>()(string(s.get()));
        }
    };

    template<>
    struct hash<MyString> : public __hash_base<size_t, MyString> {
        size_t
        operator()(const MyString &s) const noexcept {
            return hash<string>()(string(s.get()));
        }
    };
}

template<typename T>
void output_static_data(const T &myStr) {
    cout << typeid(myStr).name() << "==" << endl;
    cout << "CCtor=" << T::CCtor
         << ",MCtor=" << T::MCtor
         << ",CAsgn=" << T::CAsgn
         << ",MAsgn=" << T::MAsgn
         << ",Dtor=" << T::Dtor
         << ",Ctor=" << T::Ctor
         << ",DCtor=" << T::DCtor
         << endl;
}


template<typename M, typename NM>
void test_moveable(M c1, NM c2, long &value) {
    cout << "\n\ntest, with moveable elements" << endl;
    typedef typename iterator_traits<typename M::iterator>::value_type V1type;
    clock_t timeStart = clock();
    for (long i = 0; i < value; i++) {
        string buf = to_string(i);
        auto ite = c1.end();
        c1.insert(ite, V1type(buf.c_str()));
    }
    cout << "construction,milli-seconds:" << clock() - timeStart << endl;
    cout << "size()=" << c1.size() << endl;
    output_static_data(*(c1.begin()));

    // 容器本身操作
    timeStart = clock();
    M c11(c1);
    cout << "copy, milli-seconds: " << (clock() - timeStart) << endl;
    timeStart = clock();
    M c12(move(c1));
    cout << "move copy, milli-seconds: " << (clock() - timeStart) << endl;
    timeStart = clock();
    c11.swap(c12);
    cout << "swap, milli-seconds: " << (clock() - timeStart) << endl;

    //測試 non-moveable
    cout << "\n\ntest, with non-moveable elements" << endl;
    typedef typename iterator_traits<typename NM::iterator>::value_type V2type;
    timeStart = clock();
    for (long i = 0; i < value; ++i) {
        string buf = to_string(i);
        auto ite = c2.end();
        c2.insert(ite, V2type(buf.c_str()));
    }

    cout << "construction, milli-seconds : " << (clock() - timeStart) << endl;
    cout << "size()= " << c2.size() << endl;
    output_static_data(*(c2.begin()));

    // 容器本身操作
    timeStart = clock();
    NM c21(c2);
    cout << "copy, milli-seconds : " << (clock() - timeStart) << endl;

    timeStart = clock();
    NM c22(std::move(c2));
    cout << "move copy, milli-seconds : " << (clock() - timeStart) << endl;

    timeStart = clock();
    c21.swap(c22);
    cout << "swap, milli-seconds: " << (clock() - timeStart) << endl;
}

int main() {
    long value = 3000;           // vector 测试结果的MCtor与CCtor结果大于3000000,是因为vector的动态扩容,当容量不够的时候,会动态分配并进行拷贝。
//    test_moveable(vector<MyString>(), vector<MyStringNoMove>(), value);
//    test_moveable(list<MyString>(), list<MyStringNoMove>(), value);
//    test_moveable(deque<MyString>(), deque<MyStringNoMove>(), value);
//    test_moveable(multiset<MyString>(), multiset<MyStringNoMove>(), value);
//    test_moveable(unordered_multiset<MyString>(), unordered_multiset<MyStringNoMove>(), value);
    return 0;
}