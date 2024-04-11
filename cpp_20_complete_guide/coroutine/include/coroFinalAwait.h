#pragma once

#include <coroutine>
#include <exception>

// co_await AWAITER, 会调用下面的函数

// await_suspend() 的返回类型有三种
// 1、void 挂起线程，在执行 await_suspend() 中的语句之后继续执行挂起，并且返回到协程的调用者
// 2、bool: 表示暂停是否应该发生，如果为 true 表示暂停应该发生。
// 3、std::coroutine_handle<>, 用于恢复另一个协程序

// 3 的使用方式被称为对称传输
// https://lewissbaker.github.io/2020/05/11/understanding_symmetric_transfer


// 假设一个协程结束了，继续使用另一个后续协程，而不把控制权转交给调用者：
// 在final_suspend 中协程处于未挂起状态。必须等待 await_suspend()

namespace eon {


/**
 * @brief Awaiter 对象
 */
class FinalAwaiter;

class CoroTask {
public:
    struct promise_type;
    using CorHdl = std::coroutine_handle<promise_type>;

private:
    CorHdl hdl;

public:
    struct promise_type {
        std::coroutine_handle<> contHdl = nullptr;

        auto get_return_object() {
        }

        auto initial_suspend() {
            return std::suspend_always{};
        }

        void return_void() {}

        void unhandled_exception() {
            std::terminate();
        }
        auto final_suspend() noexcept;
    };
};

class FinalAwaiter {
public:
    constexpr bool await_ready() noexcept {
        return false;
    }

    std::coroutine_handle<> await_suspend(CoroTask::CorHdl h) noexcept {
        if (h.promise().contHdl) {
            return h.promise().contHdl;
        }
        return std::noop_coroutine();
    }
    void await_resume() noexcept {}
};

inline auto CoroTask::promise_type::final_suspend() noexcept {
    return FinalAwaiter{};
}

} // namespace eon
