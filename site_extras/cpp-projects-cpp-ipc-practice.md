> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-projects/cpp-ipc-practice/) 的文章，由站点同步脚本导出。

# C++ 进程间通信实战

进程间通信是一个非常重要的话题，特别是像一些大型项目都有它的影子，例如：PostgreSQL使用了管道完成copy的进程间通信，那么本节也将基于这个主题，使用C++去搭建一个进程间通过管道通信的demo出来，本节将会结合前面几节讲的内容，给出一个比较完整的实战例子出来：

- mutex
- condition_variable
- thread

注：本节的完整代码会放在星球中。

首先一个很简单的管道用法为：

- 创建一个管道

它会creating a pipe, forking, and invoking the shell.

```cpp
// FILE *popen(const char *command, const char *type);
pipe = popen(command.c_str(), "w");
```

- 关闭一个管道

```cpp
// int pclose(FILE *stream);
pclose(pipe);
```

接下来我们就只需要往里面写与读了，原理也很简单，例如：

此时便会把data写入到前面创建的管道中。

```cpp
// size_t fwrite( const void* buffer, size_t size, size_t count, FILE* stream );
size_t written = fwrite(data.c_str(), sizeof(char), data.size(), pipe);
```

如果从管道中读取，我们可以使用fread或者fgets，`fgets` 会在读取到换行符或达到缓冲区大小时停止，而 `fread` 会尽可能多地读取数据，直到达到指定的字节数或遇到文件结尾。同理，上面的fwrite可以替换为fputs。

现在，一个管道的基本使用介绍完了，如何写一个完整的demo？

很简单，我们只需要创建两个线程，一个读，一个写，然后写线程往pipe里面写数据，读线程从pipe里面读即可。

例如：

```cpp
std::thread writer_thread(write_to_pipe);
std::thread reader_thread(read_from_pipe);

writer_thread.join();
reader_thread.join();
```

写线程需要做的事情就是调用fwrite丢数据即可

```cpp
ProcessWriter writer("cat > /tmp/pipe");

// 写入数据
std::string message = "Hello pipe!\n";
if (!writer.write(message)) {
  std::cerr << "Failed to write data to pipe" << std::endl;
}
```

同理，读线程取数据：

```cpp
ProcessReader reader("/tmp/pipe");
// 读取数据
std::string message;
while (reader.read(message)) {
  // do something
}
```

最后的效果就是：

```cpp
Read from pipe: Hello pipe!
```