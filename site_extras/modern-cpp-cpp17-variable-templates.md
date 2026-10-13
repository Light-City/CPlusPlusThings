> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp17/variable-templates/) 的文章，由站点同步脚本导出。

# C++17 变量模板

## C++那些事之变量模版



在编写模板代码时，我们经常需要定义和使用各种类型特征（type traits）。这些类型特征通常用于在编译期对类型进行判断，STL 提供的 `std::is_const`、`std::is_same` 和 `std::is_reference` 就是这样的例子。通过这些特征，开发者可以根据类型属性进行编译期决策。

本文将介绍如何利用 **C++14 变量模板（variable templates）** 来简化类型特征的编写，并提供一个简单的示例。

### C++11

在 C++11 中，编写类型特征通常要使用 `struct` 或 `std::integral_constant` 来封装布尔值。例如，我们要编写一个判断类型是否为 `float` 的类型特征 `is_float`，可以通过如下方式实现：

```cpp
template <typename T>
struct is_float {
    static constexpr bool value = std::is_same<T, float>::value;
};
```

或者稍微简洁一点，使用类型别名和 `std::integral_constant`：

```cpp
template <typename T>
using is_float = std::integral_constant<bool, std::is_same<T, float>::value>;
```

尽管这比直接使用 `struct` 更简洁，最终我们仍然是在定义一个类型，而不是直接得到布尔值。要使用这个特征时，需要通过 `::value` 成员来获取布尔值：

```cpp
template <typename T>
void test(T t){
    if (is_float<T>::value){
        std::cout << "I'm a float" << std::endl;
    } else {
        std::cout << "I'm not a float" << std::endl;
    }
}
```

这样虽然可以工作，但仍存在两方面的不足：
1. 每次使用类型特征时都要访问 `::value`。
2. 定义新的类型特征时必须声明新的类型或类型别名，增加了代码冗余。

### C++14 变量模板的引入

C++14 引入了 **变量模板**，允许我们直接定义依赖于类型的变量。这一特性简化了类型特征的编写，使得我们不再需要使用类型别名或 `struct` 来封装布尔值，而是可以直接定义布尔值。

重写后的 `is_float` 特征如下：

```cpp
template <typename T>
constexpr bool is_float = std::is_same<T, float>::value;
```

这种方式显得更加简洁明了，去除了不必要的代码。使用起来也更方便：

```cpp
template <typename T>
void test(T t){
    if (is_float<T>){
        std::cout << "I'm a float" << std::endl;
    } else {
        std::cout << "I'm not a float" << std::endl;
    }
}
```

相比之前的写法，使用变量模板后，**不再需要通过 `::value` 来访问布尔值**，代码更加简洁直观。

### 变量模板的优势

1. **减少冗余代码**：变量模板允许我们直接定义基于类型的变量，避免了使用 `struct` 或 `std::integral_constant` 封装布尔值的冗余代码。
2. **提高可读性**：变量模板通过简化语法，使代码更清晰，尤其是在编写大量类型特征时。
3. **支持模板特化**：与类型别名模板不同，变量模板可以进行特化，无论是全特化还是部分特化。我们可以为特定类型定义专门的逻辑，而不必担心扩展性问题。

### C++17 的进一步改进

C++17 进一步扩展了变量模板的应用，为每个类型特征提供了变量模板的辅助工具。例如，`std::is_same` 引入了 `std::is_same_v` 变量模板，用于简化访问类型特征值：

```cpp
template <typename T>
constexpr bool is_float = std::is_same_v<T, float>;
```

通过这种方式，类型特征的使用更加简洁，开发者可以直接利用变量模板而无需每次访问 `::value`。

### 实际案例

这项特性在实际领域使用的挺多的，例如：数据库当中分配不同大小的内存块，我们可以通过模版初始化不同类型的块大小。

```cpp
template <typename T>
constexpr size_t block_size = 1024;  // 默认大小

template <>
constexpr size_t block_size<double> = 2048;  // double 类型需要更多内存
```