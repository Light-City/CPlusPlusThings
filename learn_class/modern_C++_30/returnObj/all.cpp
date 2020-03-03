//
// Created by light on 19-12-23.
//

#include <iostream>
#include <vector>
#include <string>

struct Test {
    Test()
    {
        //std::cout << "construct a Test object" << std::endl;
    }

    Test(const Test&)
    {
        std::cout << "copy construct  a Test object" << std::endl;
    }

    Test& operator=(const Test &t)
    {
        std::cout << "copy assignment a Test object" << std::endl;
        return *this;
    }

    Test(Test&& t)
    {
        std::cout << "move construct a Test object" << std::endl;
    }

    Test& operator=(Test &&t)
    {
        std::cout << "move assignment a Test object" << std::endl;
        return *this;
    }

    ~Test()
    {
        //std::cout << "destruct a Test object" << std::endl;
    }
};

Test getTest()
{
    // anonymous object
    return Test();
}

Test getTestWithName()
{
    // named return value
    Test temp;
    return temp;
}
/**
 * 1.copy construct本身在RVO和NRVO两种情况下被优化了，如果再加上move反而画蛇添足。
 * 2.加入了move assignment后，默认是调用move assignment而不是copy assignment，可以将move assignment注释后测试。
 * 3.对于RVO和NRVO来说，construction的情况编译器优化得比较好了，加入move语义主要是对于assignment有比较大影响
 */
int main()
{
    std::cout << "==== common case ====" << std::endl;
    Test o1;
    std::cout << "---- Test copy construct: " << std::endl;
    Test o2(o1);  // two ways for copy construct
    Test o3 = o1;
    std::cout << "---- Test move construct: " << std::endl;
    Test o4(std::move(o3));
    std::cout << "---- Test assignment: " << std::endl;
    o2 = o1;
    std::cout << "---- Test move assignment: " << std::endl;
    Test o5; o5 = std::move(o4);

    std::cout << "\n==== test for rvo ===" << std::endl;
    std::cout << "---- Test rvo for copy construct: " << std::endl;
    Test obj11(getTest());
    Test obj1 = getTest();
    std::cout << "---- Test rvo for move construct: " << std::endl;
    Test obj12(std::move(getTest()));
    std::cout << "---- Test rvo for assignment: " << std::endl;
    Test obj2; obj2 = getTest();
    std::cout << "---- Test rvo move assignment: " << std::endl;
    Test obj5; obj5 = std::move(getTest());

    std::cout << "\n==== test for nrvo ===" << std::endl;
    std::cout << "---- Test nrvo for copy construct: " << std::endl;
    Test obj33(getTestWithName());
    Test obj3 = getTestWithName();
    std::cout << "---- Test nrvo for move construct: " << std::endl;
    Test obj34(std::move(getTestWithName()));
    std::cout << "---- Test nrvo for assignment: " << std::endl;
    Test obj4; obj4 = getTestWithName();
    std::cout << "---- Test nrvo move assignment: " << std::endl;
    Test obj6; obj6 = std::move(getTestWithName());

    return 0;
}
