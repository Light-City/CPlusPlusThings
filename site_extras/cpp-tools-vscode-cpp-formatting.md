> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-tools/vscode-cpp-formatting/) 的文章，由站点同步脚本导出。

# VS Code 格式化 C++ 代码

## 《C++那些事》之Step by Step格式化代码风格

## 0.导语

本节从0开始讲解如何格式化自己的代码风格，所讲内容实用vscode插件。

## 1.依赖

本次使用的是clang-format插件，具体安装比较简单：

mac系统：

```cpp
brew install clang-format  #Mac OS
```

如果是ubuntu：

```
apt install clang-format
```

Windows版本的，可以在这里下载：

> https://llvm.org/builds/

安装好之后，进行验证：

```cpp
clang-format --version
```

**如果不想使用vscode的，可以直接clang-format裸命令使用也是可以的！**

随后在vscode里面下载好以下两个插件：

- c/c++
- clang-format

这两个在插件市场里面下载即可。

安装完之后，需要做一些配置，见后文。

## 2.配置

vscode设置里面可以搜索Clang_format_style，默认是file，这里可以不用改，当然你也可以设置为其他的，这里我给大家看一下这里的默认值为file，它表示读取当前文件夹或父目录里面的`.clang-format`文件进行格式化。

所以我们只需要生成这个文件即可，进入代码目录运行：

```cpp
clang-format -style=google -dump-config > .clang-format
```

此时便会生成这个文件，这里采用google风格，你可以对这个文件自行修改。

对照下图设置vscode里面的clang-format插件格式，最后在文件里面alt+shift+f便可以格式化文件了，为了不必每次都按这个快捷键，当文件保存之后就格式化，大家可以勾选format on save。