# C++惯用法之enum class

在Effective modern C++中Item 10: Prefer scoped enums to unscoped enum，调到要用有范围的enum class代替无范围的enum。

例如：

```cpp
enum Shape {circle,retangle};
auto circle = 10;  // error
```

上述错误是因为两个circle在同一范围。
对于enum等价于：
```cpp
#define circle 0
#define retangle 1
```
因此后面再去定义circle就会出错。

所以不管枚举名是否一样,里面的成员只要有一致的,就会出问题。
例如：
```cpp
enum A {a,b};
enum B {c,a};
```
a出现两次,在enum B的a处报错。

根据前面我们知道,enum名在范围方面没有什么作用,因此我们想到了namespace,如下例子:
```cpp
// 在创建枚举时，将它们放在名称空间中，以便可以使用有意义的名称访问它们:
namespace EntityType {
    enum Enum {
        Ground = 0,
        Human,
        Aerial,
        Total
    };
}

void foo(EntityType::Enum entityType)
{
    if (entityType == EntityType::Ground) {
        /*code*/
    }
}
```
将命名空间起的有意思点,就可以达到想要的效果。

但是不断的使用命名空间,势必太繁琐,而且如果我不想使用namespace,要达到这样的效果,便会变得不安全,也没有约束。

因此在c++11后,引入enum class。

enum class 解决了为enum成员定义类型、类型安全、约束等问题。
回到上述例子：
```cpp
// enum class
enum class EntityType {
    Ground = 0,
    Human,
    Aerial,
    Total
};

void foo(EntityType entityType)
{
    if (entityType == EntityType::Ground) {
        /*code*/
    }
}
```
这便是这一节要阐述的惯用法:enum class。