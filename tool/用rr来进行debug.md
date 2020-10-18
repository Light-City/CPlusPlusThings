# 如何用rr来debug你的C/C++程序(Linux)

想象一下如果你的程序某时会崩溃，但是不能稳定复现，你会如何debug它?

用传统debugger面临的问题就是你不知道这次运行的时候能不能复现，你猜测可能某段代码出现了问题，所以进行了一番检查。但如果最后不能复现的话，刚刚所做的工作就是无用的。gdb通过一个反向调试的功能解决了这个问题。但是gdb的反向调试代价很大，在大型项目上运行很吃力。Bug出现的几率高还好，但如果出现的概率是1/100，甚至1/1000呢？

要么就用大量printf，通过print出数据来进行debug，好处是重复运行程序的成本较低，可以写个脚本，反复可以多次尝试，直到程序崩溃，然后反回去看log。但也有一样的问题就是效率不高，哪怕复现成功了，也很有可能现有的printf并不能解决问题，需要加更多的printf，又要反复运行程序来复现。

rr的出现就是来解决上面提到的问题。rr是一个debugger，通过记录程序运行时的状态，来提供一个可以反复反向调试的debug环境。

### rr的优点
- 比gdb的反向调试更成熟，并且有更少的消耗
- 对多线程进程友好，可以只调试某个进程
- 支持gdb的指令

### 安装
推荐按着[官方文档](https://github.com/mozilla/rr/wiki/Building-And-Installing)来进行本地编译，然后安装。请注意rr现在只能在Linux上运行并且对CPU有一定的要求，具体要求请看[官方文档](https://github.com/mozilla/rr#system-requirements)。

### 基础使用方法
使用rr总共分成两步
  1. 第一步是`rr record` - 运行程序，并且记录下程序运行时的状态。
  2. 第二部是`rr replay` - 回放记录好的程序。

### 实例
```cpp
  #include <cstdio>
  #include <thread>
  #include <stdlib.h>

  void inc(int& x, int id) {
    id = id + 1; //这一行有点多余，主要是为了展示rr的reverse-continue 功能
    if (x == 2 && id == 3) {
      abort();
    }
    ++x;
    printf("x=%d\n", x);
  }

  int main () {
    int x = 0;
    std::thread t1(inc, std::ref(x), 1);
    std::thread t2(inc, std::ref(x), 2);
    std::thread t3(inc, std::ref(x), 3);
    std::thread t4(inc, std::ref(x), 4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
  }
```

上面这个程序启动了多个线程，会在当`t2`这个线程运行时`x == 2 && id == 3`的时候崩溃。因为线程运行的顺序是不可控的，所以这个程序并不会总是崩溃。

我们首先编译上面这个程序
  - 运行`g++ -g -lpthread main.cpp -o main`。编译好后，我们得到了 `main` 这个二进制文件。

然后我们来`record`
  - 这里我们运行`rr record --chaos ./main`。正常情况我们 `rr record ../main` 就可以了，但我们这里用了`--chaos`让rr可以更加随机的进行调度，从而增加复现这个bug的概率。我们还可以自动化这个步骤通过`while rr record --chaos ./main; do :; done` 来反复运行这个程序，这个while循环会在`main`崩溃的时候自动结束。

![rr_debugger_1](https://user-images.githubusercontent.com/8703745/96356858-4909a200-10c2-11eb-829d-f973716b4111.png)
![rr_debugger_2](https://user-images.githubusercontent.com/8703745/96356872-853d0280-10c2-11eb-9adf-4b0165539b3d.png)

我这次运行了32次才成功复现这个崩溃。

`rr`　会把每次的记录存放在`~/.local/share/rr`里面，这时候我们`ls`看一下。

![rr_debugger_3](https://user-images.githubusercontent.com/8703745/96356873-866e2f80-10c2-11eb-89f2-f3acd00fdc9d.png)

每个`main-<数字>`就是rr存放的记录，`rr`支持我们回放每一个记录，`rr replay`
默认会回放最新的那个，我们直接`rr replay`就好了。

![rr_debugger_4](https://user-images.githubusercontent.com/8703745/96356875-8706c600-10c2-11eb-89e6-e481fa25e061.png)

上图就是rr debugger的样子。因为rr使用的是gdb protocol，所以我们可以运行各种gdb的指令。

我们接下来利用gdb的`continue`指令 (`continue`指令会让程序一直运行，直到程序结束，程序崩溃，或者命中断点)，让程序自动停在崩溃的地方。

![rr_debugger_5](https://user-images.githubusercontent.com/8703745/96356876-8837f300-10c2-11eb-9735-679a3be7ebdd.png)

我们看到rr的回放也print出了x=1 和 x=2，
跟当时运行时是完全一样的。这里我们输入`bt`可以检查call stack的内容。可以看到`abort()`是让程序崩溃的原因。

![rr_debugger_6](https://user-images.githubusercontent.com/8703745/96356877-89692000-10c2-11eb-8f4f-1142f71bffa9.png)

输入`up 2`，来到`inc()`的这个frame。再配合`list`，就可以看到具体造成崩溃的代码。这里我们输入 `p id` 可以看到当前`id`的数据，确实是`id == 3`造成了崩溃。

![rr_debugger_7](https://user-images.githubusercontent.com/8703745/96356892-b61d3780-10c2-11eb-8289-ddec50aea5a4.png)

这时候我们需要寻找为什么`id`是3的原因，这段代码很明显是由`id = id + 1`造成的，但是假如我们不知道是哪里造成`id`变化，我们可以输入`watch -l id` 来观察这个变量，再通过`rc`来回到造成变化的代码。

![rr_debugger_8](https://user-images.githubusercontent.com/8703745/96357406-6db54800-10c9-11eb-8f24-cebb0c0fc7f4.png)

注意这里我们输入了`rc`两次，第一次`rc`的时候，`rr`收到了程序崩溃的信号，所以停止了，我们需要再输入一次让它继续。rr自动就帮我们回到了`id`变化的这一行代码，这时候我们再看`p id`，`id`就是2了。通过追逐`id`的变化，我们就可以对代码进行必要的修改了。

这就是rr的能力，不单单我们返回到了这一行代码，连程序相应的状态都回到了这个时刻。rr的强大之处就是给我们的一种回到过去的能力。
