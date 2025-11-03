#include <iostream>
#include <fstream>
#include <string>
#include <coroutine>
#include <future>
#include <boost/asio.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/experimental/as_tuple.hpp>

namespace asio = boost::asio;
using boost::system::error_code;

// 自定义task类型，用于封装协程的状态和结果
template <typename T>
struct task {
    // promise_type是协程的核心，负责协程的初始化、挂起和恢复
    struct promise_type {
        T value;
        std::exception_ptr exception;
        std::coroutine_handle<promise_type> handle;

        // 获取协程的返回对象
        task<T> get_return_object() {
            handle = std::coroutine_handle<promise_type>::from_promise(*this);
            return task<T>(handle);
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

// 异步读取文件的协程函数
asio::awaitable<std::string> async_read_file(const std::string& filename) {
    asio::io_context io_context;
    asio::steady_timer timer(io_context, std::chrono::seconds(1));

    // 模拟异步IO操作，等待1秒
    co_await timer.async_wait(asio::use_awaitable);

    // 打开文件并读取内容
    std::ifstream file(filename);
    if (!file.is_open()) {
        co_return "Failed to open file: " + filename;
    }

    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }

    co_return content;
}

int main() {
    try {
        asio::io_context io_context;
        asio::co_spawn(io_context, async_read_file("test.txt"), [](std::exception_ptr e, std::string result) {
            if (e) {
                std::rethrow_exception(e);
            }
            std::cout << "File content: " << result << std::endl;
        });

        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
