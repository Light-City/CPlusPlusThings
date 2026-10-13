> 来自 [C++那些事网站](https://light-city.github.io/stories_things/stl-source-analysis/cpp-string-interview/) 的文章，由站点同步脚本导出。

# STL string 面试题精讲

今天给大家分享一个字符串相关的面试题目，总共有4个case，可以在留言区评论，说出这四处会发生什么。

std::string大家应该非常熟悉了，std::string_view是17之后引入的视图，不懂的可以查阅一下，下面有两个字面量，分别是string子面量与string_view字面量，用`s`、`sv`代表。

前两个问题都是函数返回一个std::string_view，但有些区别，一个是基于c-style string，另一个基于string。

后面两个问题是从string字面量与string_view字面量构建。

好了，问题描述清楚了，可以在留言区见了～

```cpp
std::string_view getBar() {
  std::string b{"it is bar."};
  return b;
}

std::string_view getFoo() {
  return "It is foo.";
}

using namespace std::string_literals;
using namespace std::string_view_literals;
std::cout << getFoo() << std::endl;   // 1
std::cout << getBar() << std::endl;	  // 2
std::string_view svf{"Hello foo"s};  
std::cout << svf << std::endl;  			// 3
std::string_view svff{"Hello foo"sv}; 
std::cout << svff << std::endl; 			// 4
```