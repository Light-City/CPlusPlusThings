> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-memory-debug/cpp-memory-leak-detection/) 的文章，由站点同步脚本导出。

# C++ 内存泄漏检测

## C++那些事之如何检测内存

我们在编写C/C++代码时，内存问题无处不在，无论是工作还是面试，都离不开内存的话题。

今天使用一个例子来Step by Step的上手valgrind检测C++内存。

模拟三个问题：

- 释放内存问题检测
- 写溢出检测
- 读溢出检测

你将会学习到：

- 如果检测这三个问题
- 如果使用gdb + valgrind一起调试

欢迎一起讨论。

## 1.示例引入

大家看一下下面这个代码，我们会发现有很多问题：

- strcpy不安全，究竟多大的内存大小，可能会越界
- print时msg访问越界
- 析构函数没有释放内存

是不是像极了自己曾经写的代码，哈哈。

本节就来检测出这些问题。

```cpp
class Message {
private:
    char *msg;

public:
    Message(const char *text) {
        msg = new char[3];
        std::strcpy(msg, text);
    }

    void printOutOfRange() {
        std::cout << msg[5] << std::endl;
    }

    ~Message() {
        
    }
};

int main() {
    Message message("hello");
    message.printOutOfRange();

    return 0;
}
```

## 2.valgrind

valgrind安装:

```
brew install valgrind // mac
sudo apt-get install install // ubuntu
```

更多系统的安装参考官方地址：

> https://valgrind.org/downloads/

## 3.使用

首先编译时，建议使用debug，-g，这样的话可以检测到代码哪一行有问题。

```
g++ val_test.cc -g
```

然后我们执行：

```
valgrind --leak-check=full ./a.out 
```

我们便得到了valgrind给我们的信息：

![](../assets/images/2024/mem/img/detect.png)

此时，在上面我们可以看到几个问题：

- Invalid write

发生在val_test.cc::11

- Invalid read

发生在val_test.cc:10

- 内存泄漏

1 allocs, 0 frees，3 bytes allocated.

这些信息可以快速让我们定位到哪一行代码有问题。

## 4.gdb调试

当我们给它加上`--vgdb-error=0`后，我们便可以很方便的调试代码了，一边调试，一边看哪里有问题。

```cpp
valgrind --leak-check=full --vgdb-error=0 ./a.out 
```

![](../assets/images/2024/mem/img/attach.png)

随后，我们gdb上去

```cpp
gdb -q a.out
```

把上面的target那一行粘贴到 gdb里面，然后continue，我们便可以看到对应行的代码以及valgrind会继续输出的信息。

![](../assets/images/2024/mem/img/gdb.png)

![](../assets/images/2024/mem/img/continue.png)

本节带大家实战内存问题，如果对你有帮助欢迎转发。