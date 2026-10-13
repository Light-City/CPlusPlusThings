> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-projects/cpp-short-url-demo/) 的文章，由站点同步脚本导出。

# C++ 短链接服务 Demo

真实的短链接相对来说比较复杂，例如：hash算法、放号系统等等。

今天只是从小项目角度模拟一个短链接实现。

如何通过短短的几十行代码快速实现一个？其中涉及的几个关键点：

- 如何将长链接缩短
- 如何存储映射关系

映射关系就不用说了，整一个map持久化内存，如果涉及到数据库，需要通过数据库的jdbc/odbc等接口访问写入库当中。

长链接缩短这里采用随机算法+固定长度思路来实现。

## 1.随机

通过uniform_int_distribution，生成一个`[a, b]`区间的随机数，这里随机从下列字符串中每次取一个字符，最后拼接起来就是短链接。

```cpp
string characters{
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"};
```

包含：

- 数字
- 小写字母
- 大写字母

```cpp
uniform_int_distribution<int> distribution(0, characters.size() - 1);
string code;
for (int i = 0; i < length; ++i)
  code += characters[distribution(generator)];
```

## 2.存储

每次将其存储到map中即可，例如：

```cpp
if (links.count(originalLink)) {
  return links[originalLink];
}

shortCode = generateRandomCode(len);
string shortenedLink = base_url + shortCode;
links[shortenedLink] = originalLink;
```

这里保留了基本url，例如：

```cpp
string baseUrl = "https://www.google.com.hk/search";
LinkShortener shortener(baseUrl);

string originalLink = "https://www.google.com.hk/search?q=asdsad&oq=asdsad&aqs=chrome.0.69i59j69i57j69i59l2.634j0j4&sourceid=chrome&ie=UTF-8";
string shortenedLink = shortener.shortenLink(originalLink, 8);
```

最后便可以将这个url缩短为：

```
https://www.google.com.hk/searchtXuOlOSt
```

是不是很简单，本节的测试与完整代码已在星球更新，欢迎加入一起学习成长～