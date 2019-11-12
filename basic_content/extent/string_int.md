# C++中如何将string类型转换为int类型？

个人公众号：

![](../img/wechat.jpg)

首先提出解决方案：

- atoi
- strtol
- stoi

这几个有什么不同呢？下面测试对比。

> C语言风格函数

atoi与strtol对比：

```cpp
string str = "16s";
int a = atoi(str.c_str());
int b = strtol(str.c_str(), nullptr, 10);
```

输出：

```
atoi的结果为:16
strtol的结果为:16
```

这两个函数都是从字符串开始寻找数字或者正负号或者小数点,遇到非法字符终止。

所以到上述s字符就不输出了，提前结束，也就是说当你的字符串不是数字的时候，或者小数点等非数字，不会报异常！直接输出0.

例如：

```cpp
string str = "asdsa";
int a = atoi(str.c_str());
int b = strtol(str.c_str(), nullptr, 10);
```

输出：

```
0
0
```

strtol相比与atoi来说，支持多种进制转换,例如8进制等

例如：

```
int b = strtol(str.c_str(), nullptr, 8);
```

> C++风格

在C++中可以使用stoi来转int，这个函数相比于前两个一个最大特点是：异常！

我们知道C++相比于C语言多了异常，这也是这个函数在C++中具有的最显著功能。

例如：

```cpp
string str1 = "asq,";
//    int c = stoi(str1);    // 报异常
string str2 = "12312";
int c = stoi(str2);     // ok
cout << c << endl;
```

异常如下：

```
terminate called after throwing an instance of 'std::invalid_argument'
what():  stoi
```

> 自定义

也就是自己写呗，如下：

```cpp
int stringToInt(const string &s) {
    int v;
    stringstream ss;
    ss << s;
    ss >> v;
    return v;
}
int main() {
    int i = stringToInt("2.3");
    cout<<i<<endl;
}
```