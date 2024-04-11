#pragma once

#include <coroutine>
#include <exception>

namespace eon {

/**
 * @brief 支持 co_return 的协程接口
 * @tparam T
 */
template<typename T>
class ResultTask {
public:
    struct promise_type;
    using CoroHdl = std::coroutine_handle<promise_type>;

private:
    CoroHdl hdl;

public:
    struct promise_type {
        T res{};
        T m_tmp{};

        auto get_return_object() {
            return ResultTask{ CoroHdl::from_promise(*this) };
        }

        auto initial_suspend() {
            return std::suspend_always{};
        }

        auto final_suspend() noexcept {
            return std::suspend_always{};
        }

        /**
         * @brief 使用 co_return 返回值
         * @param value
         */
        void return_value(T value) {
            res = value;
        }

        /**
         * @brief 使用 co_yield 返回值
         */
        auto yield_value(T tmp) {
            m_tmp = tmp;
            return std::suspend_always{};
        }

        void unhandled_exception() {
            std::terminate();
        }
    };

public:
    explicit ResultTask(auto handler):
        hdl(handler) {}

    ~ResultTask() {
        if (hdl) {
            hdl.destroy();
        }
    }

    ResultTask(const ResultTask&)            = delete;
    ResultTask& operator=(const ResultTask&) = delete;

public:
    //////////// API
    bool resume() const {
        if (!hdl || hdl.done()) {
            return false;
        }
        hdl.resume();
        return !hdl.done();
    }

    T getReturnValue() {
        return hdl.promise().res;
    }

    T getYieldValue() {
        return hdl.promise().m_tmp;
    }
};

} // namespace eon