#pragma once

#include <iostream>
#include <coroutine>
#include <exception>

// 协程可以理解为一个可以控制中断和恢复的函数
// 如何中断？
// 如何恢复？

namespace eon {

/**
 * @brief 简单协程接口
 *
 * @note 甚至可以在容器或者调用器中存储或者管理协程接口
 *       甚至可以跳过协程接口（TODO）
 */
class [[nodiscard]] CoroTask {
public:
    struct promise_type;
    using CoroHdl = std::coroutine_handle<promise_type>;

private:
    CoroHdl hdl;

public:
    /**
     *
     * @brief 对于每一个协程，都有一个 promise，在协程调用时自动创建
     * @note 协程的状态存储在协程句柄中，其类型为 std::coroutine_handle<PrmType>
     *       hdl 提供了恢复协程的原生 API（检查是否处于结束状态，销毁其内存）
     *
     *       协程接口是将所有内容组合在一起的场所。保存并管理本机协程句柄，并由协程调用
     *       返回，且提供成员函数来处理协程
     */
    struct promise_type {
        /**
         * @brief promise 类型定义如何创建协协程
         * @return
         */
        auto get_return_object() {
            return CoroTask{ CoroHdl::from_promise(*this) };
        }

        /**
         * @brief 允许协程在创建之后进行额外的操作，并且定义协程是主动启动还是惰性启动
         * @return
         */
        auto initial_suspend() const {
            return std::suspend_always{};
        }

        /**
         * @brief 是否应该最终挂起协程，必须保证不抛出异常
         * @return
         */
        auto final_suspend() const noexcept {
            return std::suspend_always{};
        }

        /**
         * @brief 定义协程无返回值
         */
        void return_void() {}

        /**
         * @brief 定义了协程中未本地处理的异常
         */
        void unhandled_exception() const {
            std::terminate();
        }
    };

public:
    explicit CoroTask(auto h):
        hdl(h) {
    }

    ~CoroTask() {
        if (hdl) {
            hdl.destroy();
        }
    }

    CoroTask(const CoroTask&)            = delete;
    CoroTask& operator=(const CoroTask&) = delete;

    ////// api
    [[nodiscard]] bool resume() const {
        if (!hdl || hdl.done()) {
            return false;
        }
//        std::cout << "before resume()" << std::endl;
        hdl.resume();
//        std::cout << "end resume()" << std::endl;
        return !hdl.done();
    }
};
} // namespace eon