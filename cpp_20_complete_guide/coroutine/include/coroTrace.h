#pragma once

#include <coroutine>
#include <exception>
#include <iostream>
#include <string>

// 追踪协程的行为

namespace trace {

struct MyAwait
{
    bool await_ready() noexcept
    {
        std::cout << "MyAwait::await_ready()" << std::endl;
        return false;
    }

    void await_suspend(auto h) noexcept
    {
        std::cout << "MyAwait::await_suspend()" << std::endl;
    }

    void await_resume() noexcept
    {
        std::cout << "MyAwait::await_resume()" << std::endl;
    }
};

class CoroTraceTask
{
public:
    struct promise_type;
    using CoroHdl = std::coroutine_handle<promise_type>;

private:
    CoroHdl hdl;

public:
    explicit CoroTraceTask(auto h) : hdl(h) {}

    ~CoroTraceTask()
    {
        if (hdl)
        {
            hdl.destroy();
        }
    }

    CoroTraceTask(const CoroTraceTask&) = delete;
    CoroTraceTask& operator=(const CoroTraceTask&) = delete;

public:
    /**
     * @brief 定制协程的行为，定义处理协程的某些自定义点，特定的成员函数定义了在特定情况下调用此函数
     */
    struct promise_type
    {
        // 存储一个值让调用者可以读取到

        int yield_value_store{0};
        std::string return_value_store{};

        auto get_return_object()
        {
            std::cout << "get_return_object()" << std::endl;
            return CoroTraceTask{CoroHdl::from_promise(*this)};
        }

        /**
         * @brief 控制协程启动运行代码之前的操作，是否直接启动？
         */
        auto initial_suspend()
        {
            std::cout << "initial_suspend()" << std::endl;
            return std::suspend_always{};
        }

        /**
         * @brief 最终协程挂起
         * @return
         */
        auto final_suspend() noexcept
        {
            std::cout << "final_suspend()" << std::endl;
            return std::suspend_always{};
            //            return std::suspend_never{};
        }

        /**
         * @brief 此协程不调用 co_return 也 OK
         */
        // void return_void() {
        //}

        /**
         * @brief 处理异常
         */
        void unhandled_exception()
        {
            // 对于异常直接终止
            std::terminate();
        }

        /**
         * @brief co_return 支持返回值
         */
        void return_value(std::string value)
        {
            std::cout << "return_value(): " << std::endl;
            return_value_store = "XXXXX";
        }

        auto yield_value(int value)
        {
            std::cout << "yield_value()" << std::endl;
            yield_value_store = value;
            // 返回一个直接挂起的 awaiter
            return MyAwait{};
        }
    };

public:
    std::string getReturnValue() const
    {
        return hdl.promise().return_value_store;
    }

    int getYieldValue() const { return hdl.promise().yield_value_store; }

    /**
     * @brief 恢复 Future 对应的协程,可以多次调用
     * @return
     */
    bool resume() const
    {
        if (!hdl || hdl.done())
        {
            return false; // hdl 已经结束了或者无需处理了
        }

        hdl.resume(); // 恢复 hdl 对应的协程，将会阻塞在这里，直到 hdl
                      // 对应的协程中断或者结束
        return !hdl.done(); // 协程是否结束了
    }
};

/**
 * @brief 用于追踪 await
 *
 * @note awaite 对象用于
 */
struct TraceAwaiter
{
    /**
     * @brief 如果返回false， 表示直接将当前协程挂起
     *        如果返回false，不挂起，直接使用 await_resume() 返回
     */
    bool await_ready() noexcept
    {
        std::cout << "await_ready()" << std::endl;
        return false;
    }

    /**
     * @brief 作为 co_await 的返回值
     */
    auto await_resume() noexcept
    {
        std::cout << "await_resume()" << std::endl;
        return std::string{"TRACE_AWAIT"};
    }

    /**
     * @brief 可以在 await_ready() 将协程挂起后执行一些操作
     *        根据返回值可以执行对执行流进行切换
     *        1、如果返回值类型为 void，
     * 表示执行完该函数之后，无条件的挂起，然后返回给协程的调用者和恢复者
     *
     *        2、如果返回值为 bool 值， true：
     * 挂起当前协程并返回给当前协程的调用者和恢复者（注意当前协程已经为挂起状态了）
     *                                 false：
     * 直接恢复当前协程（注意当前协程已经为挂起状态了)
     *        3、返回个协程句柄：挂起协程返回给当前协程的调用者和恢复者，随后恢复它所返回的协程句柄
     *
     *
     * @note 在进入该函数调用之前，协程已经被挂起
     *
     * @note 该函数可以控制当前协程的挂起状态
     */
    void await_suspend(CoroTraceTask::CoroHdl hdl) noexcept
    {
        std::cout << "await_suspend" << std::endl;
    }
};

} // namespace trace
