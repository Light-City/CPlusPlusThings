#include <iostream>
#include <coroutine>
#include <vector>
#include <queue>
#include <thread>
#include <chrono>

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

// 任务调度器类，用于管理多个协程任务
class task_scheduler {
public:
    // 添加任务到调度器
    void add_task(task<std::string> t) {
        tasks_.emplace(std::move(t));
    }

    // 执行所有任务
    void run_all() {
        while (!tasks_.empty()) {
            auto& task = tasks_.front();
            bool completed = !task.resume();
            if (completed) {
                try {
                    std::cout << "Task completed with result: " << task.get_result() << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "Task failed with exception: " << e.what() << std::endl;
                }
                tasks_.pop();
            }
        }
    }

private:
    std::queue<task<std::string>> tasks_;
};

// 简单的协程任务，模拟耗时操作
void do_something(task_scheduler& scheduler, int id, int delay) {
    std::thread([&scheduler, id, delay]() {
        std::this_thread::sleep_for(std::chrono::seconds(delay));
        scheduler.add_task([id]() -> task<std::string> {
            std::cout << "Task " << id << " running" << std::endl;
            co_await std::suspend_always{};
            std::cout << "Task " << id << " completed" << std::endl;
            co_return "Task " + std::to_string(id) + " result";
        }());
    }).detach();
}

int main() {
    task_scheduler scheduler;

    // 添加多个任务到调度器
    do_something(scheduler, 1, 2);
    do_something(scheduler, 2, 1);
    do_something(scheduler, 3, 3);

    // 执行所有任务
    scheduler.run_all();

    return 0;
}
