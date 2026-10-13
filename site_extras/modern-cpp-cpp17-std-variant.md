> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp17/std-variant/) 的文章，由站点同步脚本导出。

# C++17 std::variant

最近想出一些有趣的问题，做一个小总结，你能回答上来几个？

注：完整答案会在星球揭秘。

**问题1：std::variant构造函数会调用几个类型构造，1个？2个？n个？**

```cpp
std::variant<std::string, int> a1;
std::variant<X, Y> a2;
```

**相关问题：**

1. `std::variant` 的默认构造行为如何？
2. 当 `std::variant` 包含类型不具有默认构造函数时会发生什么？
3. 如何指定 `std::variant` 的初始值类型？

**问题2：下面代码可以正常运行吗？如果可以运行说明理由，否则给出几个解决方案。**

```cpp
struct X {
    X(int v) : v_(v) {}
    int v_;
};

struct Y {
    Y(double v) : v_(v) {}
    double v_;
};

std::variant<X, Y> a1;
```

**相关问题：**

1. 如何为 `std::variant` 添加具有默认构造函数的类型？
2. 当 `std::variant` 的类型列表中没有默认构造函数时，有哪些解决方案？
3. 如何显式构造 `std::variant` 以避免默认构造问题？

**问题3：什么是monostate，解决了什么问题？**

**相关问题：**

1. 如何在 `std::variant` 中使用 `std::monostate`？
2. `std::monostate` 的引入对 `std::variant` 的设计有哪些影响？
3. 在什么情况下使用 `std::monostate` 是合适的？

**问题4：defer_lock_t、try_to_lock_t、adopt_lock_t这三者的区别是什么？什么场景下使用？**

**相关问题：**

1. `defer_lock_t` 的使用场景和实现机制是什么？
2. `try_to_lock_t` 如何处理互斥锁的尝试锁定？
3. `adopt_lock_t` 在接管已有锁的所有权时有什么注意事项？