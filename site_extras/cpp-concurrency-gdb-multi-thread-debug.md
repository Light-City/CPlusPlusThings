> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-concurrency/gdb-multi-thread-debug/) 的文章，由站点同步脚本导出。

# GDB 多线程 All-Stop 调试

当我们使用gdb调试多线程程序时，线程可以在任何时间执行，当我们想尝试调试某一个线程的时候(例如：next)，会导致切换到其他线程，不符合我们在当前线程切到下一行的预期。

```cpp
Thread 2 "arrow-compute-n" hit Breakpoint 1, arrow::compute::NestedLoopJoinNode::InputReceived (this=0x5bbf90, input=0x5ba840, batch=...)
    at /code/arrow/cpp/src/arrow/compute/exec/nested_loop_node.cc:399
399         size_t thread_index = thread_indexer_();
(gdb) n
[Switching to Thread 0x7ffff2c08700 (LWP 29121)]

Thread 3 "arrow-compute-n" hit Breakpoint 1, arrow::compute::NestedLoopJoinNode::InputReceived (this=0x5bbf90, input=0x5bb060, batch=...)
    at /code/arrow/cpp/src/arrow/compute/exec/nested_loop_node.cc:399
399         size_t thread_index = thread_indexer_();
```

此时我们需要怎么做呢？

那便是今天要引入的调度器锁定(scheduler-locking)。GDB 无法锁步执行所有线程。由于线程调度取决于你调试目标的操作系统（而不是由 GDB 控制），因此在当前线程完成单步操作时，其他线程可能执行多个语句。此外，一般来说，当程序停止时，其他线程停在语句的中间，而不是在清晰的语句边界处停止。甚至在继续或单步执行后，你可能会发现你的程序停在另一个线程中。每当其他线程在第一个线程完成你请求的操作之前遇到断点、信号或异常时，就会发生这种情况。

而通过gdb提供的scheduler-locking可以帮助你锁定调度器以仅允许单个线程运行来修改 GDB 的默认行为。

默认情况下scheduler-locking值等于replay

```
(gdb) show scheduler-locking
Mode for locking scheduler during execution is "replay".
```

在回放模式下的行为类似于on，在记录模式下或正常执行期间的行为类似于关闭。这是默认模式。

- on

例如执行`t 2`会切换到线程2上面，此时设置on会只在当前线程进行调试。

- off

与on相反，没有锁定，任何线程随时都可以运行。

- step

在单步执行时的行为类似于打开，否则类似于关闭。当你step时，除了当前线程之外的线程永远不会有机会运行，而当你使用像`continue`、`until`或`finish`这样的命令时，它们完全可以运行。

例如：next、step这种会在你想要的进程内进行调试，其他线程hang住；contine之类的会让所有的线程运行。

## 示例

可以看到当设置step时，continue会在线程之间切换。

```
(gdb) info threads
  Id   Target Id                                           Frame 
  1    Thread 0x7ffff7e88780 (LWP 31464) "arrow-compute-n" 0x00007ffff37e3de2 in pthread_cond_timedwait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
  2    Thread 0x7ffff3409700 (LWP 31494) "arrow-compute-n" arrow::compute::NestedLoopJoinNode::InputReceived (this=0x5bbf90, input=0x5bb060, batch=...)
    at /code/arrow/cpp/src/arrow/compute/exec/nested_loop_node.cc:399
* 3    Thread 0x7ffff2c08700 (LWP 31495) "arrow-compute-n" arrow::compute::NestedLoopJoinNode::InputReceived (this=0x5bbf90, input=0x5bb060, batch=...)
    at /code/arrow/cpp/src/arrow/compute/exec/nested_loop_node.cc:399
(gdb) c
Continuing.
[Switching to Thread 0x7ffff3409700 (LWP 31494)]

Thread 2 "arrow-compute-n" hit Breakpoint 1, arrow::compute::NestedLoopJoinNode::InputReceived (this=0x5bbf90, input=0x5bb060, batch=...)
    at /code/arrow/cpp/src/arrow/compute/exec/nested_loop_node.cc:399
399         size_t thread_index = thread_indexer_();
(gdb) info threads
  Id   Target Id                                           Frame 
  1    Thread 0x7ffff7e88780 (LWP 31464) "arrow-compute-n" 0x00007ffff37e3de2 in pthread_cond_timedwait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
* 2    Thread 0x7ffff3409700 (LWP 31494) "arrow-compute-n" arrow::compute::NestedLoopJoinNode::InputReceived (this=0x5bbf90, input=0x5bb060, batch=...)
    at /code/arrow/cpp/src/arrow/compute/exec/nested_loop_node.cc:399
  3    Thread 0x7ffff2c08700 (LWP 31495) "arrow-compute-n" 0x00007ffff605a372 in arrow::compute::NestedLoopJoinNode::InputReceived (this=0x5bbf90, input=0x5bb060, batch=...)
    at /code/arrow/cpp/src/arrow/compute/exec/nested_loop_node.cc:399
```

然后再continue会进入到下一个断点：

```cpp
(gdb) info threads
  Id   Target Id                                           Frame 
  1    Thread 0x7ffff7e88780 (LWP 31464) "arrow-compute-n" 0x00007ffff37e3a35 in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
  2    Thread 0x7ffff3409700 (LWP 31494) "arrow-compute-n" 0x00007ffff5781b50 in arrow::compute::NestedLoopJoinNode::OnBuildSideBatch(unsigned long, arrow::compute::ExecBatch)@plt ()
   from /code/arrow/cpp/build/debug/libarrow.so.200
* 3    Thread 0x7ffff2c08700 (LWP 31495) "arrow-compute-n" 0x00007ffff5781b50 in arrow::compute::NestedLoopJoinNode::OnBuildSideBatch(unsigned long, arrow::compute::ExecBatch)@plt ()
   from /code/arrow/cpp/build/debug/libarrow.so.200
(gdb) c
Continuing.
[Switching to Thread 0x7ffff3409700 (LWP 31494)]

Thread 2 "arrow-compute-n" hit Breakpoint 3, 0x00007ffff5781b50 in arrow::compute::NestedLoopJoinNode::OnBuildSideBatch(unsigned long, arrow::compute::ExecBatch)@plt ()
   from /code/arrow/cpp/build/debug/libarrow.so.200
(gdb) info threads
  Id   Target Id                                           Frame 
  1    Thread 0x7ffff7e88780 (LWP 31464) "arrow-compute-n" 0x00007ffff37e3a35 in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
* 2    Thread 0x7ffff3409700 (LWP 31494) "arrow-compute-n" 0x00007ffff5781b50 in arrow::compute::NestedLoopJoinNode::OnBuildSideBatch(unsigned long, arrow::compute::ExecBatch)@plt ()
   from /code/arrow/cpp/build/debug/libarrow.so.200
  3    Thread 0x7ffff2c08700 (LWP 31495) "arrow-compute-n" 0x00007ffff5781b56 in arrow::compute::NestedLoopJoinNode::OnBuildSideBatch(unsigned long, arrow::compute::ExecBatch)@plt ()
   from /code/arrow/cpp/build/debug/libarrow.so.200
```

可以看到线程之间切换，如果是想在线程内，可以直接next，例如：

```cpp
(gdb) t 2
[Switching to thread 2 (Thread 0x7ffff3409700 (LWP 32396))]
#0  0x00007ffff605ae0d in arrow::compute::NestedLoopJoinNode::OnBuildSideBatch (this=0x5bbf90, thread_index=1, batch=...) at /code/arrow/cpp/src/arrow/compute/exec/nested_loop_node.cc:487
487         if (!batch.length) {
(gdb) n
490         std::lock_guard<std::mutex> guard(build_side_mutex_);
(gdb) n
491         build_accumulator_.InsertBatch(std::move(batch));
```

如果是执行print操作，引起了函数调用，会停止，输出不了我们符合预期的结果。

```cpp
(gdb) p batch.ToString()
[Switching to Thread 0x7ffff2c08700 (LWP 32397)]

Thread 3 "arrow-compute-n" hit Breakpoint 1, arrow::compute::NestedLoopJoinNode::InputReceived (this=0x5bbf90, input=0x5ba840, batch=...)
    at /code/arrow/cpp/src/arrow/compute/exec/nested_loop_node.cc:399
399         size_t thread_index = thread_indexer_();
The program stopped in another thread while making a function call from GDB.
Evaluation of the expression containing the function
(arrow::compute::ExecBatch::ToString() const) will be abandoned.
When the function is done executing, GDB will silently stop.
```

我们此时可以设置on，此时便可以进行print了!

```
(gdb) set scheduler-locking on
(gdb) p batch.ToString()
$2 = "ExecBatch\n    # Rows: 2\n    0: Array[null,4]\n    1: Array[true,false]\n"
```

> https://sourceware.org/gdb/current/onlinedocs/gdb.html/All_002dStop-Mode.html