//
// Created by light on 19-12-17.
//

#include <vector>
#include <iostream>

// “首先是内存分配。如果 new 出错，按照 C++ 的规则，一般会得到异常 bad_alloc，对象的构造也就失败了。
// 这种情况下，在 catch 捕捉到这个异常之前，所有的栈上对象会全部被析构，资源全部被自动清理。”
// 之所以是栈上对象会全被被析构原因是堆上的东西都是由栈上的变量所引用的，栈上对象析构的过程，
// 堆上相应的资源自然就被释放了。而且被释放的对象的范围还被栈帧限定了。


// -fexceptions（缺省开启）
// g++ test.cpp -std=c++17 -fno-exceptions
// 关闭异常,可看到可执行文件的大小的变化。
int main()
{
    std::vector<int> v{1, 2, 3, 4, 5};
    v.push_back(20);
}