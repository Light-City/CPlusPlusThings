#include <iostream>
#include <coroutine>
#include <future>

// 自定义task类型，用于封装协程的状态和结果
template <typename T>
struct task {
    // promise_type是协程的核心，负责协程的初始化、挂起和恢复
    struct promise_type {
        T value;
        std::exception_ptr exception;

        // 获取协程的返回对象
        task<T> get_return_object() {
            return task<T>(std::coroutine_handle<promise_type>::from_promise(*this));
        }

        // 初始挂起状态，返回std::suspend_always表示协程创建后立即挂起
        std::suspend_always initial_suspend() {
            return {};
        }

        // 最终挂起状态，返回std::suspend_always表示协程完成后挂起
        std::suspend_always final_suspend() noexcept {
            return {};
        }

        // 处理协程的返回值
        void return_value(T v) {
            value = std::move(v);
        }

        // 处理协程中的异常
        void unhandled_exception() {
            exception = std::current_exception();
        }
    };

    std::coroutine_handle<promise_type> handle;

    // 构造函数
    explicit task(std::coroutine_handle<promise_type> h) : handle(h) {}

    // 析构函数，确保协程被正确销毁
    ~task() {
        if (handle) {
            handle.destroy();
        }
    }

    // 禁止拷贝构造和拷贝赋值
    task(const task&) = delete;
    task& operator=(const task&) = delete;

    // 允许移动构造和移动赋值
    task(task&& other) noexcept : handle(other.handle) {
        other.handle = nullptr;
    }

    task& operator=(task&& other) noexcept {
        if (this != &other) {
            if (handle) {
                handle.destroy();
            }
            handle = other.handle;
            other.handle = nullptr;
        }
        return *this;
    }

    // 恢复协程执行
    bool resume() {
        if (!handle || handle.done()) {
            return false;
        }
        handle.resume();
        return !handle.done();
    }

    // 获取协程的结果
    T get_result() {
        if (handle && handle.done()) {
            if (handle.promise().exception) {
                std::rethrow_exception(handle.promise().exception);
            }
            return std::move(handle.promise().value);
        }
        throw std::runtime_error("Coroutine not completed");
    }
};

// 简单的协程函数，打印"Hello, "，然后挂起，恢复后打印"Coroutine!"
task<std::string> hello_coroutine() {
    std::cout << "Hello, ";
    co_await std::suspend_always{}; // 挂起协程
    std::cout << "Coroutine!\n";
    co_return "Hello from coroutine";
}

int main() {
    auto coro = hello_coroutine(); // 创建协程，此时协程被挂起
    std::cout << "Coroutine created\n";
    coro.resume(); // 恢复协程执行，打印"Hello, "
    std::cout << "Coroutine resumed\n";
    coro.resume(); // 再次恢复协程执行，打印"Coroutine!"
    std::cout << "Coroutine completed\n";
    std::cout << "Result: " << coro.get_result() << std::endl;
    return 0;
}
