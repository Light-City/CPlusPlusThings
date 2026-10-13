> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-guides/curl-cpp-notes/) 的文章，由站点同步脚本导出。

# libcurl C++ 开发备忘

代码风格统一有助于团队协作与代码review，业界用的比较多的是google的风格，本节来点不一样的，看看咱们平时用的比较多的工具curl团队的代码规范是什么样子的。

curl简单说一下，它在命令行或脚本中传输数据非常有用，大家也肯定用过，其源码实现可以在github上查看

> https://github.com/curl/curl

下面我们来从几个方面看看curl的C语言开发规范。

[TOC]

## 命名

新的函数和变量名称应该是逻辑的、可理解的，并且根据它们的用途进行命名。

文件局部函数应该被声明为静态的，建议是小写字母名称。

## 缩进

只使用空格进行缩进，不使用制表符。每个新的开放大括号使用两个空格。

```
if(something_is_true) {
  while(second_statement == fine) {
    moo();
  }
}
```

## 注释

由于编写的是 C89 代码，不允许使用 **//** 注释。它们直到 C99 标准才被引入。只使用 **/\* 注释 \*/**。

```
/* 这是一个注释 */
```

## 长行

curl 中的源代码永远不应该超过 79 列，即使在现代大屏幕和高分辨率屏幕时代，仍然有两个原因要保持这一点：

1. 较窄的列比较宽的列更容易阅读。有一个原因是报纸几十年甚至几个世纪来一直使用列。
2. 较窄的列允许开发人员更容易地在不同的窗口中并排显示多个代码片段。它允许在同一个屏幕上并排显示两个或三个源代码窗口 - 以及多个终端和调试窗口。

## 大括号

在 if/while/do/for 表达式中，我们将开放大括号写在与关键字同一行，然后将闭合大括号设置在与初始关键字相同缩进级别的同一行。就像这样：

```
if(age < 40) {
  /* 显然是年轻人 */
}
```

如果它们只包含一个一行语句，则可以省略大括号：

```
if(!x)
  continue;
```

对于函数，开放大括号应该写在单独的一行上：

```
int main(int argc, char **argv)
{
  return 1;
}
```

## 'else' 在下一行

在使用大括号添加 **else** 子句到条件表达式时，我们将其添加到关闭大括号后的新行。就像这样：

```
if(age < 40) {
  /* 显然是年轻人 */
}
else {
  /* 可能是脾气暴躁的 */
}
```

## 括号前不加空格

在使用 if/while/do/for 表达式时，关键字与开放括号之间不应有空格。就像这样：

```
while(1) {
  /* 永远循环 */
}
```

## 使用布尔条件

在 if/while 条件中，我们更喜欢测试条件值，如布尔值与 TRUE 或 FALSE、指针与 NULL 或 != NULL 以及整数与零或非零，而不是：

```
result = do_something();
if(!result) {
  /* 出现了问题 */
  return result;
}
```

## 条件中不要赋值

为了增加可读性并减少条件的复杂性，避免在 if/while 条件中进行变量赋值。不赞成这种风格：

```
if((ptr = malloc(100)) == NULL)
  return NULL;
```

而是鼓励更清晰地表达：

```
ptr = malloc(100);
if(!ptr)
  return NULL;
```

## 新块在新行上

永远不会在同一行上写多个语句，即使是短的 if() 条件也不例外。

```
if(a)
  return TRUE;
else if(b)
  return FALSE;
```

永远不要这样：

```
if(a) return TRUE;
else if(b) return FALSE;
```

## 运算符周围加空格

请在 C 表达式中运算符的两侧使用空格。后缀 **(), [], ->, ., ++, --** 和一元 **+, -, !, ~, &** 操作符除外，它们不应该有空格。

示例：

```
bla = func();
who = name[0];
age += 1;
true = !false;
size += -2 + 3 *

 (a + b);
ptr->member = a++;
struct.field = b--;
ptr = &address;
contents = *pointer;
complement = ~bits;
empty = (!*string) ? TRUE : FALSE;
```

## 返回值不加括号

在 'return' 语句中不加额外的括号：

```
int works(void)
{
  return TRUE;
}
```

## sizeof 参数加括号

在代码中使用 sizeof 运算符时，我们更喜欢在其参数周围加上括号：

```
int size = sizeof(int);
```

## 列对齐

一些语句不能在单行上完成，因为行太长、语句太难读，或者是由于上述其他风格指南。在这种情况下，语句跨越多行。

如果一个连续行是表达式或子表达式的一部分，那么你应该在适当的列上对齐，以便能够清楚地知道它是语句的哪一部分。运算符不应该起始于连续行。在其他情况下，遵循 2 个空格的缩进指南。以下是来自 libcurl 的一些示例：

```
if(Curl_pipeline_wanted(handle->multi, CURLPIPE_HTTP1) &&
   (handle->set.httpversion != CURL_HTTP_VERSION_1_0) &&
   (handle->set.httpreq == HTTPREQ_GET ||
    handle->set.httpreq == HTTPREQ_HEAD))
  /* 没有要求 HTTP/1.0 并且是 GET 或 HEAD 请求 */
  return TRUE;
```

如果没有括号，使用默认缩进：

```
data->set.http_disable_hostname_check_before_authentication =
  (0 != va_arg(param, long)) ? TRUE : FALSE;
```

函数调用时使用开放括号：

```
if(option) {
  result = parse_login_details(option, strlen(option),
                               (userp ? &user : NULL),
                               (passwdp ? &passwd : NULL),
                               NULL);
}
```

与 "current open" 括号对齐：

```
DEBUGF(infof(data, "Curl_pp_readresp_ %d bytes of trailing "
             "server response left\n",
             (int)clipamount));
```

## 平台相关代码

使用 **#ifdef HAVE_FEATURE** 进行条件代码。我们避免在 #ifdef 行中检查特定操作系统或硬件。HAVE_FEATURE 应该由 configure 脚本为类 Unix 系统生成，并且它们在其他系统的 `config-[system].h` 文件中硬编码。

我们还鼓励在 libcurl 构建时，使用可能为空或定义为常量的宏/函数，以使代码无缝。就像这个例子，其中 **magic()** 函数根据构建时的条件不同而工作：

```
#ifdef HAVE_MAGIC
void magic(int a)
{
  return a + 2;
}
#else
#define magic(x) 1
#endif

int content = magic(3);
```

## 不要 typedef 结构体

尽管可以使用结构体，但不要对其进行 typedef。使用 `struct name` 的方式来标识它们：

```
struct something {
   void *valid;
   size_t way_to_write;
};
struct something instance;
```

**不要这样**：

```
typedef struct {
   void *wrong;
   size_t way_to_write;
} something;
something instance;
```

本文翻译自：https://curl.se/dev/code-style.html


!!! note "说明"
    本文为开发备忘，建议结合官方 libcurl 文档与具体项目错误码排查。