> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-memory-debug/valgrind-massif-memory/) 的文章，由站点同步脚本导出。

# Valgrind Massif 内存分析

gpstart -a --wrapper=valgrind --wrapper-args=" --tool=massif --time-unit=B --detailed-freq=1 --massif-out-file=/tmp/massif.out.%p"

![](../assets/images/2024/mem/massif/mss.png)

```
ms_print xxx.out | more
```

https://valgrind.org/docs/manual/ms-manual.html

![](../assets/images/2024/mem/massif/ms_print.png)



valgrind -v --tool=massif --time-unit=B --detailed-freq=1 --massif-out-file=./massif.out  ./a.out