> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-things/std-ref-wrapper/) 的文章，由站点同步脚本导出。

# C++ ref 与 reference_wrapper

## 深入理解 C++ 中的 std::cref、std::ref 和 std::reference_wrapper

在 C++ 编程中，有时候我们需要在不进行拷贝的情况下传递引用，或者在需要引用的地方使用常量对象。为了解决这些问题，C++ 标准库提供了三个有用的工具：`std::cref`、`std::ref` 和 `std::reference_wrapper`。这篇文章将深入探讨这些工具的用途、区别以及实际应用。

此外，我们知道Rust语言中，经常实现了Unwrap方法，在C++中如何实现？

这就参考Apache arrow的代码了，有这么一段：

```cpp
std::optional<std::reference_wrapper<const TransportStatusDetail>>
TransportStatusDetail::Unwrap(const Status& status) {
  std::shared_ptr<StatusDetail> detail = status.detail();
  if (!detail) return std::nullopt;
  if (detail->type_id() != kTypeId) return std::nullopt;
  return std::cref(arrow::internal::checked_cast<const TransportStatusDetail&>(*detail));
}

```

随后我们便可以通过这样调用，提取出Status中实际的TransportStatusDetail。

```cpp
Status status(transportDetail);
auto result = Unwrap(status);
```

下面来从几个方面看看，最后给出整个Demo代码。

### 1. std::cref：创建常量引用

`std::cref` 是一个模板函数，用于创建对常量对象的引用。它返回一个 `std::reference_wrapper` 对象，可以在需要引用的地方使用。这在函数参数传递中特别有用，因为它允许我们在不进行拷贝的情况下传递常量对象，同时保持引用的语义。

**示例：**

```cpp
#include <iostream>
#include <functional>

void printValue(const int& value) {
    std::cout << "Value: " << value << std::endl;
}

int main() {
    int number = 42;
    auto crefNumber = std::cref(number);

    printValue(crefNumber);  // 使用常量引用传递参数

    return 0;
}
```

### 2. std::ref：创建可修改的引用

与 `std::cref` 不同，`std::ref` 是一个模板函数，用于创建对可修改对象的引用。它返回一个 `std::reference_wrapper` 对象，允许我们在需要引用的地方使用，同时允许修改被引用的对象。

**示例：**

```cpp
#include <iostream>
#include <functional>

void modifyValue(int& value) {
    value *= 2;
}

int main() {
    int number = 42;
    auto refNumber = std::ref(number);

    modifyValue(refNumber);  // 使用可修改的引用作为参数

    std::cout << "Modified Value: " << number << std::endl;

    return 0;
}
```

### 3. std::reference_wrapper：引用的包装器

`std::reference_wrapper` 是一个模板类，用于包装引用，使其能够在容器中存储或以引用的形式传递。它提供类似引用的语法，并且可以与标准容器一起使用，因为容器无法直接存储引用。

**示例：**

```cpp
#include <iostream>
#include <vector>
#include <functional>

int main() {
    int number1 = 42;
    int number2 = 73;

    std::vector<std::reference_wrapper<int>> numbers = {std::ref(number1), std::ref(number2)};

    for (auto& num : numbers) {
        num.get() += 10;  // 修改原始对象的值
    }

    std::cout << "Number 1: " << number1 << std::endl;
    std::cout << "Number 2: " << number2 << std::endl;

    return 0;
}
```

在这个示例中，`std::reference_wrapper` 允许我们将引用包装在容器中，然后通过 `get()` 方法来访问和修改原始对象的值。

## 4.Unwrap Demo

这里给出具体的代码段，其完整代码参见知识星球内容。

```cpp
std::optional<std::reference_wrapper<const TransportStatusDetail>> Unwrap(const Status& status) {
    std::shared_ptr<StatusDetail> detail = status.detail();
    if (!detail) return std::nullopt;
    if (detail->type_id() != kTypeId) return std::nullopt;
    return std::cref(static_cast<const TransportStatusDetail&>(*detail));
}

int main() {
    // 创建 TransportStatusDetail 对象
    std::shared_ptr<StatusDetail> transportDetail = std::make_shared<TransportStatusDetail>();

    // 创建 Status 对象并传入 TransportStatusDetail 对象
    Status status(transportDetail);

    auto result = Unwrap(status);

    if (result) {
        const TransportStatusDetail& detail = result.value().get();
        // 使用 detail 进行操作
        std::cout << "TransportStatusDetail found." << std::endl;
    } else {
        std::cout << "No valid TransportStatusDetail found." << std::endl;
    }

    return 0;
}
```

欢迎一起探讨C++那些事，更多项目/内容，欢迎加入知识星球。