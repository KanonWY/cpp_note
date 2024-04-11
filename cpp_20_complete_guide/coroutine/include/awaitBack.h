#pragma once

#include <coroutine>
#include <exception>
#include <string>

namespace eon {
// 一个协程中断之后再次执行，也许需要从调用者那么获取到一些值来处理？该如何处理


/**
 * @brief 将调用者的值传递给中断的协程
 */
template<typename Hdl>
class BackAwait {
public:
    BackAwait() = default;
    Hdl hdl     = nullptr;
    /**
     * @brief co_await BackAwait{}; 是否不执行挂起操作
     * @return
     */
    bool await_ready() const noexcept {
        return false;
    }

    /**
     * @brief co_await BackAwait{}; 挂起之后立即执行该函数
     * @return
     */
    auto await_suspend(Hdl h) noexcept {
        // 保存当前挂起协程的句柄到 hdl 成员中
        hdl = h;
    }

    /**
     * @brief 挂起恢复时返回一个值
     * @return
     */
    auto await_resume() const noexcept {
        return hdl.promise().backValue;
    }
};


class CoroGenBack {
public:
    struct promise_type;
    using CoroHdl = std::coroutine_handle<promise_type>;

private:
    CoroHdl hdl;

public:
    explicit CoroGenBack(auto handler):
        hdl(handler) {
    }

    ~CoroGenBack() {
        if (hdl) {
            hdl.destroy();
        }
    }

    CoroGenBack(const CoroGenBack&)            = delete;
    CoroGenBack& operator=(const CoroGenBack&) = delete;

public:
    struct promise_type {
        int         coroValue = 0;
        std::string backValue;

        auto yield_value(int val) {
            coroValue = val;
            backValue.clear();
            return BackAwait<CoroHdl>{};
        }

        auto get_return_object() {
            return CoroGenBack(CoroHdl::from_promise(*this));
        }

        auto initial_suspend() {
            return std::suspend_always{};
        }

        void return_void() {}

        auto final_suspend() noexcept {
            return std::suspend_always{};
        }

        void unhandled_exception() noexcept {
            std::terminate();
        }
    };


    bool resume() const {
        if (!hdl || hdl.done()) {
            return false;
        }
        hdl.resume();
        return !hdl.done();
    }

    void setBackValue(const auto& val) {
        hdl.promise().backValue = val;
    }

    int getValue() const {
        return hdl.promise().coroValue;
    }
};


} // namespace eon