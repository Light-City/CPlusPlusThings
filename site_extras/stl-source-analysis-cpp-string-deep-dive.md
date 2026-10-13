> 来自 [C++那些事网站](https://light-city.github.io/stories_things/stl-source-analysis/cpp-string-deep-dive/) 的文章，由站点同步脚本导出。

# STL string 深析

当我们使用C++时，<string>库的基础知识比较熟悉，尤其是在C++中创建字符串时使用的std::string。这无疑是对旧的C风格“字符串”（使用以空字符结尾的字符数组）的一种改进。然而，C++标准库在C++17和C++20中引入了更有用的组件，可以帮助你编写更高效的代码。

在<string>头文件中，std::basic_string类是一个模板类，为各种字符串类型提供了特化，包括常见字符串的std::string（即std::basic_string<char>）和宽字符串的std::wstring（即std::basic_string<wchar_t>）。

还有一些固定宽度的特定字符串，比如std::u32string（即std::basic_string<char32_t>）和std::u16string（即std::basic_string<char16_t>）。

在C++20中，引入了char8_t，这也带来了std::u8string（即std::basic_string<char8_t>）。我不知道为什么要等到C++20才引入char8_t，那么提个问题**char16_t和char32_t在哪个标准存在呢？留言区见**

### C++11

在C++11标准下，字符串处理主要依赖于`std::string`类，这是一个动态分配内存的字符串类。此外，引入了新的固定宽度字符串类型，如`std::u32string`和`std::u16string`，为处理Unicode字符提供了更好的支持。

```cpp
#include <iostream>
#include <string>

int main() {
    // 使用std::string
    std::string str = "Hello, C++11!";

    // 使用std::u32string和std::u16string
    std::u32string u32str = U"Hello, Unicode!";
    std::u16string u16str = u"Hello, Unicode!";

    std::cout << str << std::endl;
    std::wcout << u32str << std::endl;
    std::wcout << u16str << std::endl;

    return 0;
}
```

### C++17

在C++17中，引入了<string_view>头文件，提供了一种轻量级的只读替代方案，用于使用<string>头文件中的字符串类型。这些字符串视图类似于先前描述的字符串。例如，std::u32string的<string_view>等价物是std::u32string_view（即std::basic_string_view<char32_t>）。

这在需要读取字符串但不需要修改它的函数中特别有用。与**为函数创建字符串副本**不同，我们可以简单地查看现有字符串！此外，<string_view>非常灵活，不仅可以将C++样式的字符串转换为字符串视图，甚至可以将C样式的字符串转换为字符串视图。以下是演示std::string_view简单用法的示例：

```cpp
#include <iostream>
#include <string>
#include <string_view>

void printString(std::string_view str) {
  std::cout << str << std::endl;
}

int main() {
  std::string_view strv{"strv"}; // C++ string_view
  printString(strv);

  std::string str{"str"}; // C++ string
  printString(str);

  char cstr[] = "cstr"; // C-style string
  printString(cstr);

  return 0;
}
```

输出：

```
strv
str
cstr
```

在这个例子中，printString函数以std::string_view作为参数，这使得这个函数非常轻量级和灵活，因为没有制作任何字符串的副本，这个函数可以通过传递C和C++样式的字符串以各种方式使用。

### C++20

在C++20中，引入了一些新的有用成员函数，包括starts_with(…)和ends_with(…)。正如名称所示，这些函数确定一个字符串（或字符串视图）是否以某个字符或某个std::string_view开头/结尾。以下是一个简单的例子：

```cpp
#include <cassert>
#include <string>
#include <string_view>

int main() {
  const std::string str{"Hello World!"};

  assert(str.starts_with('H'));
  assert(str.ends_with('!'));

  assert(str.starts_with("Hello")); // 隐式转换为std::string_view
  assert(str.ends_with("World!")); // 隐式转换为std::string_view

  return 0;
}
```

### C++23

C++23中引入了`contains`，用于检查字符串是否包含指定的子字符串。

```cpp
// 这是一个假设的C++23示例，实际上并不可用
#include <iostream>
#include <string>

int main() {
    std::string str = "C++23 introduces contains function.";

    if (str.contains("introduces")) {
        std::cout << "The string contains 'introduces'." << std::endl;
    } else {
        std::cout << "The string does not contain 'introduces'." << std::endl;
    }

    return 0;
}
```

> https://en.cppreference.com/w/cpp/string/basic_string/contains

### 结论

在处理新的C++代码中的字符串时，应考虑使代码尽可能灵活和内存高效。使用<string_view>可以极大地帮助这些努力。此外，考虑使用最新的字符串成员函数，如starts_with和ends_with，以获得可读且易于实现的字符串解析代码。而C++23使得contains计算更加简单，越来越现代化了。