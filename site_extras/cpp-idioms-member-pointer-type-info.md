> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-idioms/member-pointer-type-info/) 的文章，由站点同步脚本导出。

# C++ 成员指针与类型信息

`::*`，估计99%的人都没用过，甚至没见过，可以留言区打出来！本节将会从0讲解语法，并使用一个项目中的实际例子来实战该用法。

[TOC]

## 1.什么是`::*`

`C::*` 是 C++ 中的**成员指针**语法，表示指向某个类 `C` 成员的指针。这个语法用于指向类的成员变量或成员函数。与普通指针不同，成员指针不是指向某个对象的具体内存地址，而是指向类中成员的**位置**，并且需要与类的对象实例一起使用。

### 1.1 语法

- `T C::*` 是一个指向类 `C` 中类型为 `T` 的成员变量的指针。
- `R (C::*)(Args...)` 是一个指向类 `C` 中返回类型为 `R`，参数列表为 `Args...` 的成员函数的指针。

### 1.2 成员变量指针

成员变量指针指向某个类的成员变量，使用 `T C::*` 这种形式。

例如：下面示例中定义了一个成员指针 `ptr`，它指向 `MyClass` 类的 `x` 成员。`obj.*ptr` 通过成员指针访问对象的成员变量。

```cpp
struct MyClass {
    int x;
    double y;
};

int MyClass::*ptr = &MyClass::x;

MyClass obj;
obj.x = 10;
std::cout << "obj.x = " << obj.*ptr << std::endl;
```

### 1.3 成员函数指针

成员函数指针指向类的成员函数，使用 `R (C::*)(Args...)` 这种形式。

示例：通过 `obj.*funcPtr()` 调用成员函数。

```cpp
void (MyClass::*funcPtr)() const = &MyClass::print;
MyClass obj;
(obj.*funcPtr)();
```

## 3.有什么用？

假设你有一个 ORM 框架，需要从成员指针中获取字段类型，生成对应列的类型，可以这样写：

```cpp
struct User {
    std::string username;
    int user_id;
};

// 自动生成数据库列的定义
template <typename T, typename C>
void generate_sql_column(T C::*member_ptr, const std::string& column_name) {
    if constexpr (std::is_same_v<T, std::string>) {
        std::cout << column_name << " VARCHAR(255)" << std::endl;
    } else if constexpr (std::is_integral_v<T>) {
        std::cout << column_name << " INT" << std::endl;
    }
}

int main() {
    // 假设我们想自动为 User 类生成 SQL 表结构
    generate_sql_column(&User::username, "username");
    generate_sql_column(&User::user_id, "user_id");
    
    return 0;
}
```

## 4.实际项目衍生实用工具！

在实际项目中，我们可以使用`C::*`提取出对象与成员类型，怎么实现呢？

答案是：模版 + `C::*`

如下示例，我们便可以通过`value_type_from_member_pointer`与`class_type_from_member_pointer`来提取出对应的成员类型与类类型，非常方便的工具。

```cpp
template <typename T, typename C>
struct from_member_pointer_helper<T C::*> {
    using value_type = T;     
    using class_type = C;     
};

template <auto Ptr>
struct from_member_pointer : from_member_pointer_helper<typename std::remove_cvref_t<decltype(Ptr)>> {};

template <auto Ptr>
using value_type_from_member_pointer = from_member_pointer<Ptr>::value_type;

template <auto Ptr>
using class_type_from_member_pointer = from_member_pointer<Ptr>::class_type;
```