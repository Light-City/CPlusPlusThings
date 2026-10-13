> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-tools/memory-leak-debug-session/) 的文章，由站点同步脚本导出。

# C++ 内存泄漏排查实战

```
regression=# SELECT t1.a, t1.c, t2.b, t2.c FROM prt1_e t1, prt2_e t2 WHERE (t1.a + t1.b)/2 = (t2.b + t2.a)/2 AND t1.c = 0 ORDER BY t1.a, t2.b;
WARNING:  problem in alloc set ExecutorState: bad single-chunk 0x7cfdd48 in block 0x7cfc580  (seg1 slice1 127.0.1.1:7003 pid=4295)
WARNING:  problem in alloc set ExecutorState: found inconsistent memory block 0x7cfc580  (seg1 slice1 127.0.1.1:7003 pid=4295)
WARNING:  problem in alloc set ExecutorState: bad single-chunk 0x7cfdd48 in block 0x7cfc580  (seg1 slice1 127.0.1.1:7003 pid=4295)
WARNING:  problem in alloc set ExecutorState: found inconsistent memory block 0x7cfc580  (seg1 slice1 127.0.1.1:7003 pid=4295)
WARNING:  problem in alloc set ExecutorState: bad single-chunk 0x87f85c8 in block 0x87f6e00  (seg0 slice1 127.0.1.1:7002 pid=4294)
WARNING:  problem in alloc set ExecutorState: found inconsistent memory block 0x87f6e00  (seg0 slice1 127.0.1.1:7002 pid=4294)
WARNING:  problem in alloc set ExecutorState: bad single-chunk 0x7d3c5c8 in block 0x7d3ae00  (seg2 slice1 127.0.1.1:7004 pid=4296)
WARNING:  problem in alloc set ExecutorState: found inconsistent memory block 0x7d3ae00  (seg2 slice1 127.0.1.1:7004 pid=4296)
WARNING:  problem in alloc set ExecutorState: bad single-chunk 0x87f85c8 in block 0x87f6e00  (seg0 slice1 127.0.1.1:7002 pid=4294)
WARNING:  problem in alloc set ExecutorState: bad single-chunk 0x7d3c5c8 in block 0x7d3ae00  (seg2 slice1 127.0.1.1:7004 pid=4296)
WARNING:  problem in alloc set ExecutorState: found inconsistent memory block 0x87f6e00  (seg0 slice1 127.0.1.1:7002 pid=4294)
WARNING:  problem in alloc set ExecutorState: found inconsistent memory block 0x7d3ae00  (seg2 slice1 127.0.1.1:7004 pid=4296)
```



调试：

```
(gdb) b aset.c:1109 if chunk=0x7cfdd48
Breakpoint 1 at 0x105d420: file aset.c, line 1109.
(gdb) c
Continuing.

Thread 1 "postgres" hit Breakpoint 1, AllocSetAlloc (context=0x787ac50, size=4096) at aset.c:1109
1109		block->freeptr += (chunk_size + ALLOC_CHUNKHDRSZ);
(gdb) p chunk
$1 = (AllocChunk) 0x7cfdd48
(gdb) p chunk->size
$2 = 9187201950435737471
(gdb) p &chunk->size
$3 = (Size *) 0x7cfdd48
(gdb) watch (*((Size *) 0x7cfdd48))
Hardware watchpoint 2: (*((Size *) 0x7cfdd48))
(gdb) i b
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x000000000105d420 in AllocSetAlloc at aset.c:1109
	stop only if chunk=0x7cfdd48
	breakpoint already hit 1 time
2       hw watchpoint  keep y                      (*((Size *) 0x7cfdd48))
(gdb) d 1
(gdb) c

```