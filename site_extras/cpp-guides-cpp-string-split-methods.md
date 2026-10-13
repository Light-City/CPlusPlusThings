> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-guides/cpp-string-split-methods/) 的文章，由站点同步脚本导出。

# C++ 字符串六种分割方法

通常我们在开发的时候会对一个长字符串依据分隔符进行分割，从而得到我们想要的字符串，大家都说C++字符串分割并没有那么方便，今天来总结一下5种方法分割字符串。

包含了从C++11到C++20的写法，以及C风格。

为了方便后续文章的讲解，我们先定义一个函数原型：

- 参数：两个字符串，一个表示原字符串，另一个表示分隔符

- 返回值：字符串数组

注：懒人版，可以从星球获取完整代码。

## 1.find分割

第一种方法便是大家常用的先找到分割符的位置，然后划分字符串，添加到数组中，于是有了下面的代码。

```cpp
nt start = 0;
int end = s.find(delim);
std::vector<std::string> result;
while (end != std::string::npos) {
  result.push_back(s.substr(start, end - start));
  start = end + delim.size();
  end = s.find(delim, start);
}

result.push_back(s.substr(start, end - start));
```

这种办法是否还能进行优化呢？

答案是肯定的，为了使得代码看起来更加优雅，我们可以每次删除前面已经输出的子串，然后find逻辑一致。

```cpp
while (end != std::string::npos) {
  result.push_back(s.substr(0, end));
  s.erase(0, end + delim.size());
  end = s.find(delim);
}
```

## 2.C++20 Ranges与Views分割

C++20中我们可以使用 `std::ranges::views::split()` 创建一个分隔符的视图，然后使用transform，将每个子视图转换为 `std::string`。

```cpp
auto split_transform_view = s | std::ranges::views::split(delim) |
                              std::ranges::views::transform([](auto&& str) {
                                return std::string(str.begin(), str.end());
                              });
```

是不是很简单，可能一些人没看懂，诶关系去看之前的C++20文章吧。

## 3.正则分割

使用 `std::regex` 对象构造分隔符模式，使用`std::sregex_token_iterator` 遍历由正则表达式模式分隔的子字符串，-1表示提取不匹配的字符串，即除了分割符的字符串。

```cpp
std::regex r(delim);
return {std::sregex_token_iterator(s.begin(), s.end(), r, -1),
        std::sregex_token_iterator()};
```

## 4.istringstream分割

getline每次只能传递一个分隔符，于是分隔之后，需要继续跳过后续的分隔符。

```cpp
std::istringstream ss(s);
while (std::getline(ss, token, *(it = delim.begin()))) {
    result.push_back(token);
    it++;

    while (it != delim.end() && ss.peek() == *(it)) {
        it++;
        ss.get();
    }
}
```

## 5.C风格的strtok分割

使用 `strtok()` 将字符串分割成标记，然后while循环依次提取即可。

```cpp
token = strtok((char*)s.c_str(), delim.c_str());
while (token != NULL) {
  result.push_back(token);
  token = strtok(NULL, delim.c_str());
}
```

以上便是常用的几种方法，你还有其他方法吗？欢迎留言区见，本节懒人版可以从星球获取完整代码。