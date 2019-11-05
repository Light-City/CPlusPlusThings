# 纯虚函数和抽象类

## 关于作者：

个人公众号：

![](../img/wechat.jpg)

## 1.纯虚函数与抽象类

C++中的纯虚函数(或抽象函数)是我们没有实现的虚函数！我们只需声明它!通过声明中赋值0来声明纯虚函数！

对应的代码：[test.cpp](./test.cpp)

 *  纯虚函数：没有函数体的虚函数
 * 抽象类：包含纯虚函数的类

对应的代码：[pure_virtual.cpp](./pure_virtual.cpp)

抽象类只能作为基类来派生新类使用，不能创建抽象类的对象,抽象类的指针和引用->由抽象类派生出来的类的对象！

## 2.实现抽象类

抽象类中：在成员函数内可以调用纯虚函数，在构造函数/析构函数内部不能使用纯虚函数。

如果一个类从抽象类派生而来，它必须实现了基类中的所有纯虚函数，才能成为非抽象类。

对应的代码：[abstract.cpp](./abstract.cpp)

## 3.重要点

- 纯虚函数使一个类变成抽象类

对应的代码：[interesting_facts1.cpp](./interesting_facts1.cpp)

- 抽象类类型的指针和引用

对应的代码：[interesting_facts2.cpp](./interesting_facts2.cpp)

- 如果我们不在派生类中覆盖纯虚函数，那么派生类也会变成抽象类。

对应的代码：[interesting_facts3.cpp](./interesting_facts3.cpp)

- 抽象类可以有构造函数

对应的代码：[interesting_facts4.cpp](./interesting_facts4.cpp)

- 构造函数不能是虚函数，而析构函数可以是虚析构函数。

对应的代码：[interesting_facts5.cpp](./interesting_facts5.cpp)

当基类指针指向派生类对象并删除对象时，我们可能希望调用适当的析构函数。如果析构函数不是虚拟的，则只能调用基类析构函数。

## 4.完整实例

抽象类由派生类继承实现！

对应的代码：[derived_full.cpp](./derived_full.cpp)