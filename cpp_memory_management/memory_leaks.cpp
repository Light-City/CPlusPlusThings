#include <iostream>
#include <memory>
#include <vector>
#include <chrono>

// 内存泄漏示例1：忘记释放动态分配的内存
void memory_leak_example1() {
    std::cout << "\n--- Memory Leak Example 1 ---" << std::endl;
    int* ptr = new int(42);
    std::cout << "Value of ptr: " << *ptr << std::endl;
    // 忘记释放ptr指向的内存
    // delete ptr;
    std::cout << "Memory leak occurred: ptr is not deleted." << std::endl;
}

// 内存泄漏示例2：在异常情况下未释放内存
void memory_leak_example2() {
    std::cout << "\n--- Memory Leak Example 2 ---" << std::endl;
    int* ptr = new int(42);
    try {
        std::cout << "Value of ptr: " << *ptr << std::endl;
        throw std::runtime_error("An exception occurred.");
        // 异常发生后，以下代码不会执行
        delete ptr;
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
        std::cout << "Memory leak occurred: ptr is not deleted in exception handler." << std::endl;
    }
    // 如果没有在catch块中释放内存，就会发生内存泄漏
}

// 内存泄漏示例3：循环引用
class Node {
public:
    std::shared_ptr<Node> next;
    std::string data;

    Node(const std::string& data) : data(data) {
        std::cout << "Node constructor called: " << data << std::endl;
    }

    ~Node() {
        std::cout << "Node destructor called: " << data << std::endl;
    }
};

void memory_leak_example3() {
    std::cout << "\n--- Memory Leak Example 3 ---" << std::endl;
    std::shared_ptr<Node> node1 = std::make_shared<Node>("Node 1");
    std::shared_ptr<Node> node2 = std::make_shared<Node>("Node 2");
    node1->next = node2;
    node2->next = node1; // 循环引用
    std::cout << "Node 1 reference count: " << node1.use_count() << std::endl;
    std::cout << "Node 2 reference count: " << node2.use_count() << std::endl;
    // 当node1和node2离开作用域时，它们的引用计数都是2，不会被销毁
    std::cout << "Memory leak occurred: nodes are not deleted due to circular reference." << std::endl;
}

// 避免内存泄漏示例：使用智能指针
void avoid_memory_leak_example() {
    std::cout << "\n--- Avoid Memory Leak Example ---" << std::endl;
    // 使用unique_ptr避免内存泄漏
    std::unique_ptr<int> ptr = std::make_unique<int>(42);
    std::cout << "Value of ptr: " << *ptr << std::endl;
    // 不需要手动释放内存，unique_ptr会自动释放

    // 使用shared_ptr避免异常情况下的内存泄漏
    std::shared_ptr<int> ptr2 = std::make_shared<int>(100);
    try {
        std::cout << "Value of ptr2: " << *ptr2 << std::endl;
        throw std::runtime_error("An exception occurred.");
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
        // ptr2会自动释放内存，不会发生内存泄漏
    }

    // 使用weak_ptr避免循环引用
    class Node2 {
    public:
        std::weak_ptr<Node2> next;
        std::string data;

        Node2(const std::string& data) : data(data) {
            std::cout << "Node2 constructor called: " << data << std::endl;
        }

        ~Node2() {
            std::cout << "Node2 destructor called: " << data << std::endl;
        }
    };

    std::shared_ptr<Node2> node3 = std::make_shared<Node2>("Node 3");
    std::shared_ptr<Node2> node4 = std::make_shared<Node2>("Node 4");
    node3->next = node4;
    node4->next = node3; // 使用weak_ptr不会导致循环引用
    std::cout << "Node 3 reference count: " << node3.use_count() << std::endl;
    std::cout << "Node 4 reference count: " << node4.use_count() << std::endl;
    // 当node3和node4离开作用域时，它们的引用计数都是1，会被销毁
}

int main() {
    memory_leak_example1();
    memory_leak_example2();
    memory_leak_example3();
    avoid_memory_leak_example();

    std::cout << "\n--- End of Program ---" << std::endl;
    return 0;
}
