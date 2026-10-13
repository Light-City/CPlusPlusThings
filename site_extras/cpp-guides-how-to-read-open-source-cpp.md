> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-guides/how-to-read-open-source-cpp/) 的文章，由站点同步脚本导出。

# 如何阅读开源 C++ 代码

## 如何秒懂阅读开源项目代码？

在实际项目或开源代码中，特别是C代码比较喜欢用宏，那么造成了一个很大问题：阅读代码非常的困难，调试也是另外一个问题，那么给你一个项目带有大量的宏，如何快速理解？

通常我们在面试的时候会遇到一个非常重要的知识点：gcc编译过程，大家可能对答如流，那么为了区分候选人的水平，引申出：你知道gcc哪些编译参数，各自参数有什么用？预处理阶段会发生什么？一般你会在这个阶段做些什么，对你有何帮助？

本节就一个开源项目为例，如何快速秒懂代码！

下面是一段libev的代码，其中有若干个宏，比如：

- EV_FREQUENT_CHECK
- assert
- `EV_P_`
- ANHE_w
- ANHE_at_cache

```cpp
void noinline
ev_timer_start (EV_P_ ev_timer *w) EV_THROW
{
  if (expect_false (ev_is_active (w)))
    return;

  ev_at (w) += mn_now;

  assert (("libev: ev_timer_start called with negative timer repeat value", w->repeat >= 0.));

  EV_FREQUENT_CHECK;

  ++timercnt;
  ev_start (EV_A_ (W)w, timercnt + HEAP0 - 1);
  array_needsize (ANHE, timers, timermax, ev_active (w) + 1, EMPTY2);
  ANHE_w (timers [ev_active (w)]) = (WT)w;
  ANHE_at_cache (timers [ev_active (w)]);
  upheap (timers, ev_active (w));

  EV_FREQUENT_CHECK;

  /*assert (("libev: internal timer heap corruption", timers [ev_active (w)] == (WT)w));*/
}
```

看起来是不是一愣一愣的，这段代码引出几个问题：

- 函数参数是几个？
- 函数类型是？
- 代码中ev_at是啥玩意？

为了秒懂代码，我们需要使用`-E`来得到预处理阶段宏展开之后的代码，方便我们理解与阅读。

展开之后，我们会得到：

```cpp
void __attribute__ ((__noinline__))
ev_timer_start (struct ev_loop *loop, ev_timer *w)
{
  if (__builtin_expect ((!!((0 + ((ev_watcher *)(void *)(w))->active))),(0)))
    return;

  ((WT)(w))->at += ((loop)->mn_now);

  
# 3881 "ev.c" 3 4
 ((
# 3881 "ev.c"
 ("libev: ev_timer_start called with negative timer repeat value", w->repeat >= 0.)
# 3881 "ev.c" 3 4
 ) ? (void) (0) : __assert_fail (
# 388libev: ev_timer_start called with negative timer repeat value\", w->repeat >= 0.)"
# 3881 "ev.c" 3 4
 , "ev.c", 3881, __PRETTY_FUNCTION__))
# 3881 "ev.c"
                                                                                            ;

  do { } while (0);

  ++((loop)->timercnt);
  ev_start (loop, (W)w, ((loop)->timercnt) + (4 - 1) - 1);
  if (__builtin_expect ((!!((((W)(w))->active + 1) > (((loop)->timermax)))),(0))) { int __attribute__ ((__unused__)) ocur_ = (((loop)->timermax)); (((loop)->timers)) = (ANHE *)array_realloc (sizeof (ANHE), (((loop)->timers)), &(((loop)->timermax)), (((W)(w))->active + 1)); ; };
  (((loop)->timers) [((W)(w))->active]).w = (WT)w;
  (((loop)->timers) [((W)(w))->active]).at = (((loop)->timers) [((W)(w))->active]).w->at;
  upheap (((loop)->timers), ((W)(w))->active);

  do { } while (0);


}
```

此时我们去理解代码，基本上没啥障碍了，但是还是不够清晰明了，为了突出你与其他候选人之间的差异，我提出几个问题：

- 上面显示了很多行数，可以去掉吗，怎么解决？
- 上面assert这种系统宏非常的简单好理解，不需要我们展开它，你知道怎么做？
- 在上面这二步展开中可能会出现问题，你知道发生了什么，如何解决？

好了，本节就先聊这么多，下一节继续分享，欢迎留言区探讨上面的答案！