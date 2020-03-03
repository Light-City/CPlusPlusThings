//
// Created by light on 19-12-25.
//


class A {
public:
    virtual void foo();
    virtual void bar();
    void foobar();
};

class B : public A {
public:
    void foo() override; // OK
    void bar() override final; // OK
    //void foobar() override;
    //  非虚函数不能  override
};

class C final : public B {
public:
    void foo() override; // OK
    //void bar() override;
    // final  函数不可  override
};

class D : public C {
    //  错误：final  类不可派生
};