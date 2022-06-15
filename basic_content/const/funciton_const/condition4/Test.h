#ifndef TEST_H
#define TEST_H

class Test
{
public:
    Test();
    //类型转换构造函数(有参构造） 用于隐式转换
    Test(int a);
    //拷贝构造函数  用于显示转换
    Test(const Test& t);

    void fun() const;

    ~Test();
    mutable int a;
    int b;

};
#endif // TEST_H
