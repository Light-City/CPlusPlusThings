> 来自 [C++那些事网站](https://light-city.github.io/stories_things/cpp-projects/stack-calculator-design/) 的文章，由站点同步脚本导出。

# C++ 栈式计算器设计

写一个栈计算器，设计如下：

- 支持 `+`、`-`、`*`、`/`运算
- 支持后缀输入

例如：
```
2 3 +
```
输出：
```
5
```
如何实现呢？

其中最关键两个问题：

1.如何分词

2.如何实现栈

## 1.如何分词

首先定义好输入是一个字符串，空格风格，我们便可以使用istringstream分割每一个token了，基本框架为：

```cpp
std::istringstream iss(expression);
std::string token;
while (iss >> token) {
}
```

## 2.如何实现栈

对于计算器来说，最核心就是入栈、出栈。

- 碰到数字入栈
- 碰到操作符出栈计算

我们便可以写出框架：

```cpp
if (isNumber(token)) {
  pushNumber(token);
} else if (isOperator(token)) {
  popAndCal(token);
} else {
  throw std::runtime_error("Invalid token in expression.");
}
```

popAndCal是计算的核心，如：

```cpp
void popAndCal(const std::string& token) {
  if (operandStack.size() < 2) {
    throw std::runtime_error("Invalid expression.");
  } else {
    double operand2 = operandStack.top();
    operandStack.pop();
    double operand1 = operandStack.top();
    operandStack.pop();
    double result = performOperation(operand1, operand2, token);
    operandStack.push(result);
  }
}
```

那么我们在栈中变存储了我们想要的结果，最后返回top即可。

是不是很简单，欢迎与我一起探讨。