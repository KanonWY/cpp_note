#pragma once

#include <coroutine>
#include <exception>

namespace eon {

class [[nodiscard]] CoroTaskSub {
public:
    struct promise_type;
    using CoroHdl = std::coroutine_handle<promise_type>;

private:
    CoroHdl hdl;

public:
    struct promise_type {
        CoroHdl subHdl = nullptr;

        auto get_return_object() {
            return CoroTaskSub{ CoroHdl::from_promise(*this) };
        }

        auto initial_suspend() const {
            return std::suspend_always{};
        }

        auto final_suspend() const noexcept {
            return std::suspend_always{};
        }

        void return_void() {}

        void unhandled_exception() const noexcept {
            std::terminate();
        }
    };

public:
    explicit CoroTaskSub(auto h):
        hdl(h) {}

    ~CoroTaskSub() {
        if (hdl) {
            hdl.destroy();
        }
    }

    CoroTaskSub(const CoroTaskSub&)             = delete;
    CoroTaskSub&& operator=(const CoroTaskSub&) = delete;

public:
    bool await_ready() const noexcept {
        return false;
    }

    void await_suspend(auto h) {
        h.promise().subHdl = hdl;
    }

    void await_resume() {
    }

    ////////////// API
    bool resume() const {
        if (!hdl || hdl.done()) {
            return false;
        }
        CoroHdl innerhdl = hdl;
        while (innerhdl.promise().subHdl && !innerhdl.promise().subHdl.done()) {
            innerhdl = innerhdl.promise().subHdl;
        }
        innerhdl.resume();
        innerhdl.promise().subHdl = nullptr;
        return !hdl.done();
    }
};

} // namespace eon