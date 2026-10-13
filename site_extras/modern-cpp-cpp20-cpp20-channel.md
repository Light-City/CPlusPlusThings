> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp20/cpp20-channel/) 的文章，由站点同步脚本导出。

# C++20 channel 实战

## C++20实战之channel

继前面两节的直播，讲解了thread、jthread、stop_token、stop_source、stop_callback、cv、cv_any等的用法与底层实现，那么如何基于这些知识实现一个小项目呢？

于是引出了这篇，写一个channel出来。

## 1.设计

### 1.1 接口设计

接口层面我们期望与通用的channel一样，能够一次性创建一个发送、接收的channel。

例如：

```cpp
auto [sender, receiver] = make_channel<int>();
```

这里的实现可以通过类似make_unique提供一个对外的接口，返回值通过C++17的结构化绑定获取，此时便得到了两个channel。

最核心的两个接口：发送与接收。

- 发送

```cpp
 sender.send(counter++);
```

- 接收

```
auto value = receiver.recv();
```

除此之外，还可以在析构函数的时候提供一个shutdown接口，当send、recv阻塞的时候能够被唤醒释放。

例如：

```cpp
bool shutdown() {
    if (!state_) {
        return false;
    }

    light_city_lock lk(state_->mutex);
    if (source_.stop_requested()) {
        return false;
    }
    source_.request_stop();
    state_->cv.notify_all();
    return true;
}
```

### 1.2 内部队列设计

内部队列可以参考前面几天发的如何设计一个线程安全的多生产者多消费者队列，这里可以内部维护一个state，然后带上queue、mutex、cv_any。

```cpp
struct State {
    std::queue<T> queue;
    std::mutex mutex;
    // C++11 condition_variable_any
    std::condition_variable_any cv;
};
```

## 2.使用

使用起来就非常容易，只需要make出来两个channel，然后去send与recv即可。

这里我使用了jthread + stop_token来完成这个工作，例如：

```cpp
std::jthread sender_thread(
    [](Channel<int> sender, std::stop_token token) {
      int counter = 0;
      while (!token.stop_requested()) {
        sender.send(counter++);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    },
    std::move(sender), token);
```

接收端线程类似，在执行的差不多时，我们可以使用

```
stop_source.request_stop();
```

至此，便模拟了一个channel，发送端发送数据，接收端接收数据即可。

运行示例：

```cpp
➜  channel ./a.out                  
Received: 0
Received: 1
Received: 2
Received: 3
Received: 4
Received: 5
Received: 6
Received: 7
Received: 8
Received: 9
```