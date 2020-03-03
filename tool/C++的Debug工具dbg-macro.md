# C++ 的 debug 工具：dbg-macro

打日志是 C++ 开发中必不可少的一种 debug 方式，dbg-macro 受 Rust 语言中 的 `dbg` 启发，提供比 `printf` 和 `std::cout` 更好的宏函数。主要有如下特点：

- 美观的彩色输出（当输出不是交互式终端时，颜色将自动禁用）
- 兼容 C++11，并且是 header-only
- 支持基础类型和 STL 容器类型的输出
- 可以输出文件名、行号、函数名和原始表达式

例如下面的代码：

```cpp
#include <vector>
#include <dbg.h>

// You can use "dbg(..)" in expressions:
int factorial(int n) {
  if (dbg(n <= 1)) {
    return dbg(1);
  } else {
    return dbg(n * factorial(n - 1));
  }
}

int main() {
  std::string message = "hello";
  dbg(message);  // [example.cpp:15 (main)] message = "hello" (std::string)

  const int a = 2;
  const int b = dbg(3 * a) + 1;  // [example.cpp:18 (main)] 3 * a = 6 (int)

  std::vector<int> numbers{b, 13, 42};
  dbg(numbers);  // [example.cpp:21 (main)] numbers = {7, 13, 42} (size: 3) (std::vector<int>)

  dbg("this line is executed");  // [example.cpp:23 (main)] this line is executed

  factorial(4);

  return 0;
}
```

效果如下：

![](https://camo.githubusercontent.com/3cde47c8db560b3ed42763f2ef306d78d4b19087/68747470733a2f2f692e696d6775722e636f6d2f4e4845596b39412e706e67)

## 安装

```bash
git clone https://github.com/sharkdp/dbg-macro
sudo ln -s $(readlink -f dbg-macro/dbg.h) /usr/include/dbg.h
```

这样，你就可以在任何地方使用 `dbg.h` 了。

### 在 Arch Linux 上

```bash
yay -S dbg-macro
```

### 使用 vcpkg

```bash
vcpkg install dbg-macro
```

更多内容详见 [官方文档](https://github.com/sharkdp/dbg-macro)。
