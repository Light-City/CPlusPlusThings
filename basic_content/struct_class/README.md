> 本文档由 [C++那些事网站](https://light-city.github.io/stories_things/cpp-things/struct-vs-class/) 同步生成，网站版本含多语言与更新排版。

# C++ struct 与 class 区别

## 关于作者：

个人公众号：

![](https://raw.githubusercontent.com/Light-City/CPlusPlusThings/master/img/wechat.jpg)

关于C与C++中struct内容：见[struct那些事](struct.md)

总的来说，struct 更适合看成是一个数据结构的实现体，class 更适合看成是一个对象的实现体。

区别:

最本质的一个区别就是默认的访问控制

默认的继承访问权限。struct 是 public 的，class 是 private 的。

struct 作为数据结构的实现体，它默认的数据访问控制是 public 的，而 class 作为对象的实现体，它默认的成员变量访问控制是 private 的。