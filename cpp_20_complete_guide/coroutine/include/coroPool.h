#pragma once

#include <coroutine>
#include <exception>
#include <utility>
#include <thread>
#include <list>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

namespace eon {

// 协程池前置声明
class CoroPool;

/**
 * @brief 协程 future 对象
 *
 */
class [[nodiscard]] CoroPoolTask {

public:
    struct promise_type;
    using CoroHdl = std::coroutine_handle<promise_type>;

private:
    CoroHdl hdl; ///<<< 协程句柄
    friend CoroPool;

public:
    explicit CoroPoolTask(auto h):
        hdl(h) {}


    ~CoroPoolTask() {
        if (hdl && !hdl.promise().poolPtr) {
            hdl.destroy();
        }
    }

    CoroPoolTask(const CoroPoolTask&)            = delete;
    CoroPoolTask& operator=(const CoroPoolTask&) = delete;
    CoroPoolTask& operator=(CoroPoolTask&&)      = delete;

    CoroPoolTask(CoroPoolTask&& tc) noexcept:
        hdl(std::exchange(tc.hdl, {})) {
    }


public:
    struct promise_type {
        CoroPool* poolPtr = nullptr; ///<<< 控制该协程生命周期的池
        CoroHdl   contHdl = nullptr; ///<<< 当前协程挂起的协程

        /**
         * @brief Get the return object object
         *
         * @return CoroPoolTask
         */
        CoroPoolTask get_return_object() noexcept {
            return CoroPoolTask{ CoroHdl::from_promise(*this) };
        }

        /**
         * @brief 初始化挂起点
         *
         * @return auto
         */
        auto initial_suspend() const noexcept {
            return std::suspend_always{};
        }

        /**
         * @brief 异常处理
         *
         */
        void unhandled_exception() {
            std::terminate();
        }

        /**
         * @brief co_return 为空
         *
         */
        void return_void() {}

        /**
         * @brief 协程执行完毕，最终挂起的状态
         *
         * @return auto
         */
        auto final_suspend() const noexcept {

            /**
             * @brief 最终 awaiter 结构体，控制当前协程结束之后的操作
             *
             */
            struct FinalAwaiter {
                bool await_ready() const noexcept {
                    return false;
                }
                std::coroutine_handle<> await_suspend(CoroHdl h) noexcept {
                    if (h.promise().contHdl) {
                        return h.promise().contHdl;
                    }
                    return std::noop_coroutine();
                }

                void await_resume() noexcept {}
            };

            return FinalAwaiter{};
        }
    };

    struct CoAwaitAwaiter {
        CoroHdl newHdl;
        bool    await_ready() const noexcept { return false; }

        void await_suspend(CoroHdl awaitingHdl) const noexcept;

        void await_resume() noexcept {}
    };

    /**
     * @brief 重载了 co_await 操作符号
     *
     * @note co_await CoroPoolTask{} 将会转到这里执行，受到 CoAwaitAwaiter 的控制
     *
     */
    auto operator co_await() noexcept {
        return CoAwaitAwaiter{ std::exchange(hdl, nullptr) };
    }
};


class CoroPool {
private:
    std::list<std::jthread>          threads;
    std::list<CoroPoolTask::CoroHdl> coros;
    std::mutex                       corosMx;
    std::condition_variable_any      corosCV;
    std::atomic<int>                 numCoros = 0;

public:
    /**
     * @brief Construct a new Coro Pool object
     *
     * @param num
     */
    explicit CoroPool(int num) {
        for (int i = 0; i < num; ++i) {
            std::jthread worker_thread{ [this](const std::stop_token& st) {
                threadLoop(st);
            } };
            threads.push_back(std::move(worker_thread));
        }
    }

    ~CoroPool() {
        // 请求停止所有线程
        for (auto& t: threads) {
            t.request_stop();
        }
        // 等待所有线程
        for (auto& t: threads) {
            t.join();
        }
        // 销毁残留的协程
        for (auto& c: coros) {
            c.destroy();
        }
    }

    /**
     * @brief  工作线程
     *
     * @param st
     */
    void threadLoop(std::stop_token st) {

        // 当没有收到停止通知，则持续执行
        while (!st.stop_requested()) {

            CoroPoolTask::CoroHdl coro;

            // 从队列中获取下一个协程
            {
                std::unique_lock lock(corosMx);
                // 持续等待直到不为空
                if (!corosCV.wait(lock, st, [&] {
                        return !coros.empty();
                    })) {
                    // 接收到 stop_request 则直接返回
                    return;
                }
                // 从队列中获取一个协程句柄
                coro = coros.back();
                coros.pop_back();
            }
            // 恢复该协程
            coro.resume();

            std::function<void(CoroPoolTask::CoroHdl)> destoryDone;

            destoryDone = [&destoryDone, this](auto hdl) {
                // 如果该协程已经结束了，则销毁它
                if (hdl && hdl.done()) {
                    auto nextHdl = hdl.promise().contHdl;
                    hdl.destroy();
                    --numCoros;
                    destoryDone(nextHdl);
                }
            };
            destoryDone(coro);
            numCoros.notify_all();
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    }

    void waitUntilNoCoros() {
        int num = numCoros.load();
        while (num > 0) {
            numCoros.wait(num);
            num = numCoros.load();
        }
    }

    /**
     * @brief 传入一个协程句柄，让该协程句柄被协程池管理，将协程句柄压入到协程队列中
     *
     * @param coro 协程句柄
     */
    void runCoro(CoroPoolTask::CoroHdl coro) noexcept {
        ++numCoros;
        coro.promise().poolPtr = this;
        {
            std::scoped_lock lock(corosMx);
            coros.push_back(coro);
            corosCV.notify_one();
        }
    }

    void runTask(CoroPoolTask&& coroTask) noexcept {
        auto hdl = std::exchange(coroTask.hdl, nullptr);
        if (hdl.done()) {
            hdl.destroy();
        }
        {
            runCoro(hdl);
        }
    }
};

void CoroPoolTask::CoAwaitAwaiter::await_suspend(CoroHdl awaitingHdl) const noexcept {
    newHdl.promise().contHdl = awaitingHdl;
    awaitingHdl.promise().poolPtr->runCoro(newHdl);
}


} // namespace eon
