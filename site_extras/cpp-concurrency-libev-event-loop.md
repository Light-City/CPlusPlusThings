> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-concurrency/libev-event-loop/) 的文章，由站点同步脚本导出。

# libev 事件循环原理

大纲

[TOC]

`libev` 是一个跨平台的高效事件通知库，主要用于多路复用事件机制（例如 `select`、`poll`、`epoll` 等）的实现。它的设计目标是提供简单、快速的事件循环机制，特别适合需要高并发和低延迟的场景，如高性能 Web 服务器、网络代理和其他 IO 密集型应用。它的代码量非常少，并且使用在一些生产环境中，非常推荐阅读，本节来讲讲其中的实现原理与使用。

## 1.基本框架

libev 的核心是事件循环（event loop），它是应用程序与操作系统之间的接口，用于监听和分发不同类型的事件。libev 的架构非常简洁，主要包含以下几个部分：

### 1.1 事件句柄（ev_watcher）

libev 通过事件句柄（watcher）来管理和监听各种事件。每种事件类型（如 IO 事件、定时器事件等）都有一个对应的 watcher 类型。常见的事件句柄包括：

- **ev_io**：用于监听文件描述符的可读/可写事件。
- **ev_timer**：用于定时器事件，支持一次性定时器和重复定时器。
- **ev_signal**：用于捕捉信号事件（如 `SIGINT`、`SIGTERM` 等）。

像上面都可以称之为ev_watcher的子类：

```cpp
typedef struct ev_watcher
{
  int active;
  int pending;
  int priority;
  void *data;
  void (*cb)(struct ev_loop *loop, struct ev_watcher *w, int revents);
} ev_watcher;
```

实际使用时，会涉及到两个关键函数，例如：注册一个可读的标准输入事件

```cpp
ev_io stdin_watcher;
ev_io_init(&stdin_watcher, stdin_cb, /*STDIN_FILENO*/ 0, EV_READ);
ev_io_start(loop, &stdin_watcher);
```

ev_io_init是一个宏，最后展开就是下面代码，主要是对watcher进行初始化，例如：设置callback、fd、events等。

```cpp
  do
  {
    do
    {
      ((ev_watcher *)(void *)((&stdin_watcher)))->active = ((ev_watcher *)(void *)((&stdin_watcher)))->pending = 0;
      ((ev_watcher *)(void *)(((&stdin_watcher))))->priority = (0);
      ((((&stdin_watcher)))->cb = ((stdin_cb)), memmove(&((ev_watcher *)(((&stdin_watcher))))->cb, &(((&stdin_watcher)))->cb, sizeof((((&stdin_watcher)))->cb)));
    } while (0);
    do
    {
      ((&stdin_watcher))->fd = ((0));
      ((&stdin_watcher))->events = ((EV_READ)) | EV__IOFDSET;
    } while (0);
  } while (0);
```

ev_io_start主要是修改anfs与fdchanges数组。

- 对于已经active的watcher直接返回
- 如果还不活跃，则设置active与priority
- 使用头插法插入到anfds对应fd的链表中，并将当前watcher设置为head

```cpp
wlist_add(&((loop)->anfds)[fd].head, (WL)w);

static __inline__ void
wlist_add(WL *head, WL elem)
{
  elem->next = *head;
  *head = elem;
}
```

- 将fd插入到fdchanges数组末尾

```cpp
++((loop)->fdchangecnt);
((loop)->fdchanges)[((loop)->fdchangecnt) - 1] = fd;
```

- 设置标记

```cpp
w->events &= ~EV__IOFDSET;
```

### 1.2 IO多路复用

在libev中支持epoll、poll、select等，其中在loop中有2个回调函数：

```cpp
void (*backend_modify)(struct ev_loop *loop, int fd, int oev, int nev);
void (*backend_poll)(struct ev_loop *loop, ev_tstamp timeout);
```

我们以epool为例，backend_modify就是epoll_modify，backend_poll就是epoll_poll。

epoll_modify就是大家通常使用的epoll_ctl，当事件发生变化使用EPOLL_CTL_MOD，否则使用EPOLL_CTL_ADD。

```cpp
epoll_ctl (backend_fd, oev && oldmask != nev ? EPOLL_CTL_MOD : EPOLL_CTL_ADD, fd, &ev)
```

epoll_poll执行逻辑如下：

- epoll_wait获取就绪事件列表(loop->epoll_events)
- 对于就绪事件，执行fd_event放入loop的pendings对应优先级的数组中。

```cpp
static __inline__ void
fd_event(struct ev_loop *loop, int fd, int revents)
{
  ANFD *anfd = ((loop)->anfds) + fd;
  if (__builtin_expect((!!(!anfd->reify)), (1)))
    fd_event_nocheck(loop, fd, revents);
}

static __inline__ void
fd_event_nocheck(struct ev_loop *loop, int fd, int revents)
{
  ANFD *anfd = ((loop)->anfds) + fd;
  ev_io *w;
  for (w = (ev_io *)anfd->head; w; w = (ev_io *)((WL)w)->next)
  {
    int ev = w->events & revents;
    if (ev)
      ev_feed_event(loop, (W)w, ev);
  }
}

// 省略部分代码
void __attribute__((__noinline__))
ev_feed_event(struct ev_loop *loop, void *w, int revents)
{
   w_->pending = ++((loop)->pendingcnt)[pri];
  ((loop)->pendings)[pri][w_->pending - 1].w = w_;
  ((loop)->pendings)[pri][w_->pending - 1].events = revents;
}
```

### 1.3 Timer

Timer内部实现采用了二叉堆与四叉堆，这里之所用四叉堆，是因为四叉堆的缓存效率更高，其实现如下：

```cpp
#define DHEAP 4
#define HEAP0 (DHEAP - 1) /* index of first element in heap */
#define HPARENT(k) ((((k) - HEAP0 - 1) / DHEAP) + HEAP0)
#define UPHEAP_DONE(p,k) ((p) == (k))
```

注意上面使用了HEAP0作为第一个元素，即offset，堆内部的实现便是upheap、downheap，比较简单就不赘述。其结构为：

```cpp
typedef ev_watcher_time *WT;

typedef struct
{
  ev_tstamp at;
  WT w;
} ANHE;
```

heap的排序是按照at来比较，at为定时器到期时间，堆排序则按照at的值由小到大排序。

### 1.4 事件循环（ev_loop）

通常使用如下：

```cpp
struct ev_loop *loop = EV_DEFAULT;
ev_run(loop, 0);
```

EV_DEFAULT会调用ev_default_loop函数，初始化一个ev_loop结构，里面主要是通过loop_init初始化loop结构，比如：选择epoll还是poll、select。

最核心的点：epoll_init发生在上面loop创建时。

ev_run比较复杂，主要做几件事：

- fd_reify

遍历前面提到的fdchanges数组，根据fd从anfds中取出链表，遍历所有事件链表，得到所有事件，判断当前最新的事件与之前老的事件(events)是否一致，不一致调用epoll_modify(或者第一次添加事件)。

```cpp
typedef struct
{
  WL head;		// 事件链表
  unsigned char events;	// 前面事件
	// other
} ANFD;
```

- backend_poll

见前面backend_poll的实现，会获取就绪事件并把事件放入pendings数组里面，即放到对应的优先级队列的最后。

- timer_reify，事件未过期不做处理，过期了执行下面逻辑：

  - 没有设置repeat的timer
    - 调用ev_timer_stop清除

  - 设置repeat的timer

    例如：`ev_timer_init(&timer_watcher, timer_cb, 2.0, 3.0);``

    ``ev_timer` 的 `at` 设置为当前时间加上 2 秒（即定时器将在 2 秒后触发），并且 `repeat` 设置为 3 秒，那么定时器将首先在 2 秒后触发，然后每隔 3 秒触发一次。

    ```cpp
    if (w->repeat)
    {
      // 更新下次触发时间
      ((WT)(w))->at += w->repeat;
      // 过期了
      if (((WT)(w))->at < ((loop)->mn_now))
        ((WT)(w))->at = ((loop)->mn_now);	// 设置当前时间
      // 设置堆顶元素为前面w->at
      (((loop)->timers)[HEAP0]).at = (((loop)->timers)[HEAP0]).w->at;
      // 调整堆
      downheap(((loop)->timers), ((loop)->timercnt), HEAP0);
    }
    ```

- Invoke_cb

Invoke_cb默认是ev_invoke_pending，从对应优先队列中取出事件，调用callback函数进行事件处理。

```cpp
ANPENDING *p = ((loop)->pendings)[((loop)->pendingpri)] + --((loop)->pendingcnt)[((loop)->pendingpri)];
(p->w)->cb(loop, (p->w), (p->events));
```

## 2.如何使用？

```cpp
git clone git@github.com:enki/libev.git
./configure
make && make install
```

项目编译：

```
gcc main.c -lev
```

## 3.课后作业

最后，留一个课后题目，你是否可以使用libev写一个这样的例子？

当我们输入数据的时候，stdio事件返回ready，如果输入超时，则返回timeout，演示效果：

```cpp
[light@i-7dwsk0ty libev]$ ./a.out 
timeout
[light@i-7dwsk0ty libev]$ ./a.out 
21
stdin ready
```