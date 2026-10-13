> 来自 [C++那些事网站](https://light-city.github.io/stories_things/modern-cpp/cpp11/variadic-templates-cpp/) 的文章，由站点同步脚本导出。

# C++ 可变参数模板

## 《C++那些事》之可变参数模版

最近在看Arrow的源代码发现了一个比较有意思的设计，在一个plan中可以添加多个节点，例如：proj、agg等等。

今天来讲讲这一块的设计与实现。

## 1.添加节点

对于一个plan通常需要添加一堆节点，这里便是调用EmplaceNode来实现的。

```cpp
return plan->EmplaceNode<ScalarAggregateNode>(
    plan, std::move(inputs), schema(std::move(fields)), std::move(target_field_ids),
    std::move(aggregates), std::move(kernels), std::move(states),
    std::move(owned_options));
```

同理，如果添加一个proj节点就是:

```cpp
return plan->EmplaceNode<ProjectNode>(plan, std::move(inputs),
                schema(std::move(fields)), std::move(exprs),
                project_options.async_mode);
```

那么究竟是什么魅力让代码写的如此丝滑？

这里把Plan简单写一下：

```cpp
class ARROW_EXPORT ExecPlan
{
	public:
    template <typename Node, typename... Args>
      Node* EmplaceNode(Args&&... args) {
        std::unique_ptr<Node> node{new Node{std::forward<Args>(args)...}};
        auto out = node.get();
        AddNode(std::move(node));
        return out;
      }
 }
```

看上这个语法非常的复杂，没错，它是11之后的可变参数模版，这里解释一下这段代码的含义，就是将参数包丢给对应的Node构造函数。这个例子可能看起来比较复杂，接下来我们进入Demo环节。

### 2.Demo

接下来我们创建了一个图，图中有两个不同的节点，分别是节点 A与 节点B，为了简单器件这里使用一个函数模版EmplaceNode。

```cpp
NodeA* nodeA = EmplaceNode<NodeA>("Node A", 10, 3.14);
NodeB* nodeB = EmplaceNode<NodeB>(1, "Node B");
```

完整代码如下：

```cpp
#include <iostream>
#include <vector>
#include <memory>

class NodeA {
public:
    NodeA(const std::string& name, int value, double weight)
        : name_(name), value_(value), weight_(weight) {}

    const std::string& GetName() const {
        return name_;
    }

private:
    std::string name_;
    int value_;
    double weight_;
};

class NodeB {
public:
    NodeB(int id, const std::string& desc)
        : id_(id), desc_(desc) {}

    int GetID() const {
        return id_;
    }

private:
    int id_;
    std::string desc_;
};
template<typename Node, typename... Args>
Node* EmplaceNode(Args&&... args) {
    std::unique_ptr<Node> node{new Node{std::forward<Args>(args)...}};
    auto out = node.get();
    // AddNode(std::move(node));
    return out;
}

int main() {
    NodeA* nodeA = EmplaceNode<NodeA>("Node A", 10, 3.14);
    NodeB* nodeB = EmplaceNode<NodeB>(1, "Node B");
    std::cout << "Node A name: " << nodeA->GetName() << std::endl;
    std::cout << "Node B ID: " << nodeB->GetID() << std::endl;
    return 0;
}
```

## 3.拓展-实现一个print函数可以打印任意类型数据

简单实现版本：

使用可变模版参数+递归方式实现即可。

每次拆分成一个+一堆，直到只剩下一个，所以有一个处理一个的函数与处理一个+一堆的函数，分别对应第一个print_impl与第二个print_impl。

```cpp
#include <iostream>
#include <utility>

template <typename T>
void print_impl(T&& t) {
    std::cout << t;
}

template <typename T, typename... Args>
void print_impl(T&& t, Args&&... args) {
    std::cout << t;
    print_impl(std::forward<Args>(args)...);
}

template <typename... Args>
void print(Args&&... args) {
    print_impl(std::forward<Args>(args)...);
}

int main() {
    int x = 1;
    const char* str = "Hello, world!";
    print("The value of x is ", x, ", and the string is ", str, ".\n");
    return 0;
}

```