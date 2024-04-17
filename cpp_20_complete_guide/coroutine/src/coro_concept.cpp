///////////////////////////////////////////////////////////////////////////
// Example source code for blog post:
// "C++ Coroutines: Understanding Symmetric-Transfer"
//
// Implementation of a naive 'task' coroutine type.

// #include <experimental/coroutine>
#include <iostream>
#include <coroutine>
#include <utility>
#include <iostream>

class task
{
public:
    class promise_type
    {
    public:
        task get_return_object() noexcept
        {
            std::cout << "promise get return object" << std::endl;
            return task{
                std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        std::suspend_always initial_suspend() noexcept
        {
            std::cout << "创建 promise 之后第一次挂起" << std::endl;
            return {};
        }

        void return_void() noexcept {}

        void unhandled_exception() noexcept { std::terminate(); }

        /**
         * @brief 协程结束时候调用的 awaiter 对象
         */
        struct final_awaiter
        {
            bool await_ready() noexcept { return false; }
            void await_suspend(std::coroutine_handle<promise_type> h) noexcept
            {
                h.promise().continuation.resume();
            }
            void await_resume() noexcept {}
        };

        auto final_suspend() noexcept { return final_awaiter{}; }

        std::coroutine_handle<> continuation;
    };

    task(task&& t) noexcept : coro_(std::exchange(t.coro_, {})) {}

    ~task()
    {
        if (coro_) coro_.destroy();
    }

    class awaiter
    {
    public:
        bool await_ready() noexcept
        {
            int a;
            return false;
        }

        /**
         * @brief 协程挂起之后做的操作
         * @param continuation 当前挂起的协程
         */
        void await_suspend(std::coroutine_handle<> continuation) noexcept
        {
            std::cout << "当前挂起的协程栈帧地址为: " << continuation.address()
                      << std::endl;
            std::cout << "await 中存储的协程栈帧地址为: " << coro_.address()
                      << std::endl;
            // 将调用该协程的父协程地址保存在当前协程的 promise 中
            coro_.promise().continuation = continuation;

            coro_.resume();
        }

        void await_resume() noexcept {}

        friend task;
        explicit awaiter(std::coroutine_handle<promise_type> h) noexcept
            : coro_(h)
        {
        }

    private:
        std::coroutine_handle<promise_type> coro_;
    };

    /**
     * @brief co_awiat task{} 的时候将会调用此函数
     */
    awaiter operator co_await() && noexcept { return awaiter{coro_}; }

private:
    explicit task(std::coroutine_handle<promise_type> h) noexcept : coro_(h) {}

    std::coroutine_handle<promise_type> coro_;
};

struct sync_wait_task
{
    struct promise_type
    {
        sync_wait_task get_return_object() noexcept
        {
            return sync_wait_task{
                std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        auto initial_suspend() noexcept
        {
            // 创建完毕之后不挂起直接执行
            return std::suspend_never{};
        }
        std::suspend_always final_suspend() noexcept { return {}; }

        void return_void() noexcept {}

        void unhandled_exception() noexcept { std::terminate(); }
    };

    std::coroutine_handle<promise_type> coro_;

    explicit sync_wait_task(std::coroutine_handle<promise_type> h) noexcept
        : coro_(h)
    {
    }

    sync_wait_task(sync_wait_task&& t) noexcept : coro_(t.coro_)
    {
        t.coro_ = {};
    }

    ~sync_wait_task()
    {
        if (coro_)
        {
            coro_.destroy();
        }
    }

    /**
     * @brief 该协程创建之后不挂起，直接运行
     * @param t
     * @return
     */
    static sync_wait_task start(task&& t) { co_await std::move(t); }

    bool done() { return coro_.done(); }
};

struct manual_executor
{
    struct schedule_op
    {
        manual_executor& executor_;
        schedule_op* next_ = nullptr;
        std::coroutine_handle<> continuation_;

        schedule_op(manual_executor& executor) : executor_(executor) {}

        bool await_ready() noexcept { return false; }

        void await_suspend(std::coroutine_handle<> continuation) noexcept
        {
            continuation_ = continuation;
            next_ = executor_.head_;
            executor_.head_ = this;
        }

        void await_resume() noexcept {}
    };

    schedule_op* head_ = nullptr;

    schedule_op schedule() noexcept { return schedule_op{*this}; }

    void drain()
    {
        while (head_ != nullptr)
        {
            auto* item = head_;
            head_ = item->next_;
            item->continuation_.resume();
        }
    }

    void sync_wait(task&& t)
    {
        auto t2 = sync_wait_task::start(std::move(t));
        while (!t2.done())
        {
            drain();
        }
    }
};

task completes_synchronously()
{
    std::cout << "COMPLETES" << std::endl;
    co_return;
}

task loop_synchronously(int count)
{
    std::cout << "loop_synchronously(" << count << ")" << std::endl;
    for (int i = 0; i < count; ++i)
    {
        std::cout << "will call co_await completes" << std::endl;
        co_await completes_synchronously();
    }
    std::cout << "loop_synchronously(" << count << ") returned" << std::endl;
}

int main()
{
    manual_executor ex;
    ex.sync_wait(loop_synchronously(100));
    ex.sync_wait(loop_synchronously(1000));
    ex.sync_wait(loop_synchronously(100'000));
    ex.sync_wait(loop_synchronously(1'000'000)); // Crashes with stack-overflow!
    return 0;
}
