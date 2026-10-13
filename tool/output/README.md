> 本文档由 [C++那些事网站](https://light-city.github.io/stories_things/cpp-tools/stl-container-output/) 同步生成，网站版本含多语言与更新排版。

# C++ STL 容器快捷输出

对吴老师的代码进行了修改，[点击此处直通代码](https://github.com/Light-City/CPlusPlusThings/tree/master/tool/output/output_container.h)

输入：

```cpp
map<int, int> mp{
            {1, 1},
            {2, 4},
            {3, 9}};
    cout << mp << endl;
```

输出结果显示：

```cpp
{ 1 => 1, 2 => 4, 3 => 9 }
```

#### 