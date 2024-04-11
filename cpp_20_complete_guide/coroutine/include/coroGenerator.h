#pragma once

#include <coroutine>
#include <exception>

namespace eon {

template<typename T>
class Generator {
public:
    struct promise_type;
    using CoroHdl = std::coroutine_handle<promise_type>;

private:
    CoroHdl hdl;

public:
    struct promise_type {
        T m_value{};

        auto get_return_object() {
            return Generator{ CoroHdl::from_promise(*this) };
        }

        auto initial_suspend() const {
            return std::suspend_always{};
        }

        auto final_suspend() const noexcept {
            return std::suspend_always{};
        }

        void unhandled_exception() const {
            std::terminate();
        }

        auto yield_value(T value) {
            m_value = value;
            return std::suspend_always{};
        }
    };

public:
    explicit Generator(auto handler):
        hdl(handler) {}

    ~Generator() {
        if (hdl) {
            hdl.destroy();
        }
    }

    Generator(const Generator&)            = delete;
    Generator& operator=(const Generator&) = delete;

public:
    ///////////////// API
    /**
     * @brief 支持迭代器, 来代替 resume 函数
     */
    struct iterator {
        CoroHdl hdl; ///<<< 协程句柄

        explicit iterator(auto handler):
            hdl(handler) {}

        // 代替 resume() 函数
        void getNext() {
            if (hdl) {
                hdl.resume();
                if (hdl.done()) {
                    hdl = nullptr;
                }
            }
        }

        T operator*() const {
            return hdl.promise().m_value;
        }

        iterator operator++() {
            getNext();
            return *this;
        }

        bool operator==(const iterator&) const = default;
    };

    iterator begin() const {
        if (!hdl || hdl.done()) {
            return iterator(nullptr);
        }

        iterator itor{ hdl };
        itor.getNext();
        return itor;
    }

    iterator end() const {
        return iterator{ nullptr };
    }
};


} // namespace eon