#pragma once

#include <coroutine>
#include <exception>

// 协程在中断自身的时候可以返回值吗？
// 协程在中断自身的时候可以使用 co_yield 将自身的值返回给协程的调用者
// 协程的调用者如何获取值呢？ 使用协程接口获取值。

namespace eon {
/**
 * @brief 支持 co_yield 的协程接口
 */
template<typename T>
class [[nodiscard]] CoroYield {
public:
    struct promise_type;
    using CoroHdl = std::coroutine_handle<promise_type>;

private:
    CoroHdl hdl;

public:
    struct promise_type {

        T m_value{};

        auto get_return_object() {
            return CoroYield{ CoroHdl::from_promise(*this) };
        }

        void unhandled_exception() noexcept {
            std::terminate();
        }

        auto initial_suspend() const {
            return std::suspend_always{};
        }

        /**
         * @note
         *      该函数必须为 noexcept
         */
        auto final_suspend() const noexcept {
            return std::suspend_always{};
        }

        auto yield_value(T value) {
            m_value = value;
            return std::suspend_always{};
        }
    };

public:
    explicit CoroYield(auto h):
        hdl(h) {
    }
    ~CoroYield() {
        if (hdl) {
            hdl.destroy();
        }
    }
    CoroYield(const CoroYield&)            = delete;
    CoroYield& operator=(const CoroYield&) = delete;

public:
    T getYieldValue() {
        return hdl.promise().m_value;
    }

    bool resume() {
        if (!hdl || hdl.done()) {
            return false;
        }
        hdl.resume();
        return !hdl.done();
    }
};
} // namespace eon