> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-tools/bazel-debugging/) 的文章，由站点同步脚本导出。

# Bazel 与 LLDB 联合调试

今天在调试时发现通过bazel编译出来的文件在mac上没法调试了，竟然全是汇编代码。

```
* thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 1.1
    frame #0: 0x0000000100000fd0 hello_world`main
hello_world`main:
->  0x100000fd0 <+0>: pushq  %rbp
    0x100000fd1 <+1>: movq   %rsp, %rbp
    0x100000fd4 <+4>: subq   $0x60, %rsp
    0x100000fd8 <+8>: movl   $0x0, -0x4(%rbp)
(lldb) quit
```

如何解决呢？

默认是standalone，我们需要disable，这里采用local，这意味着输出将与其依赖项链接，而不是采用默认的`standalone`策略。

```cpp
build --spawn_strategy=local
```

这样的话可以看到代码了。

```cpp
* thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 1.1
    frame #0: 0x0000000100000fdf hello_world`main at hello_world.cc:14:21
   11  
   12   int main() {
   13     // Create a thread pool
-> 14     auto threadPool = GetCpuThreadPool();
   15     if (!threadPool) {
```