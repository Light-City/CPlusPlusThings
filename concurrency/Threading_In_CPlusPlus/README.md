> 本文档由 [C++那些事网站](https://light-city.github.io/stories_things/cpp-concurrency/threading-in-cpp/) 同步生成，网站版本含多语言与更新排版。

# C++ 多线程入门

[TOC]

本文中的所有代码见《C++那些事》仓库。

> https://github.com/Light-City/CPlusPlusThings

## 1.理解线程与进程

线程是调度的基本单位 进程是资源分配的基本单位。可以把一个程序理解为进程，进程又包含多个线程。

例如：浏览器是个进程，而每开一个tab就是一个线程。

两者简单区别：

- **地址空间和其它资源**：进程间相互独立，同一进程的各线程间共享。某进程内的线程在其它进程不可见。  
- **通信**： 进程间通信IPC，线程间可以直接读写进程数据段（如全局变量）来进行通信——需要进程同步和互斥手段的辅助，以保证数据的一致性。  
- **调度和切换**：线程上下文切换比进程上下文切换要快得多。  
- 在**多线程OS**中，进程不是一个可执行的实体。

至于IPC通信与线程通信后面会新开一篇文章。

## 2.五种创建线程的方式

- 函数指针
- Lambda函数吧
- Functor(仿函数)
- 非静态成员函数
- 静态成员函数

### 2.1 函数指针

```cpp
// 1.函数指针
void fun(int x) {
    while (x-- > 0) {
        cout << x << endl;
    }
}
// 调用
std::thread t1(fun, 10);
t1.join();
```

### 2.2 Lambda函数

```cpp
// 注意：如果我们创建多线程 并不会保证哪一个先开始
int main() {
    // 2.Lambda函数
    auto fun = [](int x) {
        while (x-- > 0) {
            cout << x << endl;
        }
    };
//    std::1.thread t1(fun, 10);
    // 也可以写成下面：
    std::thread t1_1([](int x) {
        while (x-- > 0) {
            cout << x << endl;
        }
    }, 11);
//    std::1.thread t2(fun, 10);
//    t1.join();
    t1_1.join();
//    t2.join();
    return 0;
}
```

### 2.3 仿函数

```cpp
// 3.functor (Funciton Object)
class Base {
public:
    void operator()(int x) {
        while (x-- > 0) {
            cout << x << endl;
        }
    }
};
// 调用
thread t(Base(), 10);
t.join();
```

### 2.4 非静态成员函数

```cpp
// 4.Non-static member function
class Base {
public:
    void fun(int x) {
        while (x-- > 0) {
            cout << x << endl;
        }
    }
};
// 调用
thread t(&Base::fun,&b, 10);
t.join();
```

### 2.5 静态成员函数

```cpp
// 4.Non-static member function
class Base {
public:
    static void fun(int x) {
        while (x-- > 0) {
            cout << x << endl;
        }
    }
};
// 调用
thread t(&Base::fun, 10);
t.join();
```

## 3.join与detach

### 3.1 join

- 一旦线程开始，我们要想等待线程完成，需要在该对象上调用join()
- 双重join将导致程序终止
- 在join之前我们应该检查显示是否可以被join,通过使用joinable()

```cpp
void run(int count) {
    while (count-- > 0) {
        cout << count << endl;
    }
    std::this_thread::sleep_for(chrono::seconds(3));
}

int main() {
    thread t1(run, 10);
    cout << "main()" << endl;
    t1.join();
    if (t1.joinable()) {
        t1.join();
    }
    cout << "main() after" << endl;
    return 0;
}
```

### 3.2 detach

- 这用于从父线程分离新创建的线程
- 在分离线程之前，请务必检查它是否可以joinable，否则可能会导致两次分离，并且双重detach()将导致程序终止 
- 如果我们有分离的线程并且main函数正在返回，那么分离的线程执行将被挂起

```cpp
void run(int count) {
    while (count-- > 0) {
        cout << count << endl;
    }
    std::this_thread::sleep_for(chrono::seconds(3));
}

int main() {
    thread t1(run, 10);
    cout << "main()" << endl;
    t1.detach();
    if(t1.joinable())
        t1.detach();
    cout << "main() after" << endl;
    return 0;

```

## 4.临界区与互斥量

### 4.1 什么是临界区(Critical Sections)？

临界段是一段代码，如果要使程序正确运行，一次只能由一个线程执行。如果两个线程(或进程)同时执行临界区内的代码，则程序可能不再具有正确的行为。

### 4.2 只是增加一个变量是临界区吗?

可能是吧。 

增加变量（i ++）的过程分三个步骤：

- 将内存内容复制到CPU寄存器。 load
- 在CPU中增加该值。  increment
- 将新值存储在内存中。 store

如果只能通过一个线程访问该内存位置（例如下面的变量i），则不会出现争用情况，也没有与i关联的临界区。 但是sum变量是一个全局变量，可以通过两个线程进行访问。 两个线程可能会尝试同时增加变量。

```cpp
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

int sum = 0; //shared

mutex m;

void *countgold() {
    int i; //local to each thread
    for (i = 0; i < 10000000; i++) {
        sum += 1;
    }
    return NULL;
}

int main() {
    thread t1(countgold);
    thread t2(countgold);

    //Wait for both threads to finish
    t1.join();
    t2.join();

    cout << "sum = " << sum << endl;
    return 0;
}
```

上面代码的典型输出是sum总和为20000000。由于存在竞争条件，每次运行程序都会打印不同的总和。 该代码不会阻止两个线程同时读写总和。 例如，两个线程都将sum的当前值复制到运行每个线程的CPU中（让我们选择123）。 两个线程都将一个递增到自己的副本。 两个线程都写回该值（124）。 如果线程在不同时间访问了总和，则计数将为125。

### 4.3 如何确保一次只有一个线程可以访问全局变量?

如果一个线程当前处于临界区，我们希望另一个线程等待，直到第一个线程完成。为此，我们可以使用**互斥锁(**互斥的缩写)。

互斥锁形象比喻：

![](http://www.ruanyifeng.com/blogimg/asset/201304/bg2013042408.jpg)

一个防止他人进入的简单方法，就是门口加一把锁。先到的人锁上门，后到的人看到上锁，就在门口排队，等锁打开再进去。这就叫["互斥锁"](http://zh.wikipedia.org/wiki/互斥锁)（Mutual exclusion，缩写 Mutex），防止多个线程同时读写某一块内存区域。

```cpp
m.lock();
sum += 1;
m.unlock();
```

上述代码就可以正常输出：`sum = 20000000`。

## 参考资料

> http://www.ruanyifeng.com/blog/2013/04/processes_and_threads.html?utm_source=com.ideashower.readitlater.pro&utm_medium=social&utm_oi=35626384621568

> https://www.youtube.com/watch?v=eZ8yKZo-PGw&list=PLk6CEY9XxSIAeK-EAh3hB4fgNvYkYmghp&index=4