
#include <coroutine>
#include <thread>
#include <iostream>
#include <syncstream>
#include <atomic>
#include <list>
#include <utility>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <cassert>

constexpr bool syncOutDebug = true;

inline auto syncOut(std::ostream& strm = std::cout) {
    return std::osyncstream(strm);
}

inline auto coutDebug() {
    if constexpr (syncOutDebug) {
        return std::osyncstream{ std::cout };
    }
    else {
        struct devnullbuf: public std::streambuf {
            // basic output primitive without any print statement:
            int_type overflow(int_type c) {
                return c;
            }
        };
        static devnullbuf devnull;
        return std::ostream{ &devnull };
    }
}
class CoroPool;

class [[nodiscard]] CoroPoolTask {
    friend CoroPool;

public:
    struct promise_type;
    using CoroHdl = std::coroutine_handle<promise_type>;

private:
    CoroHdl hdl;
    int     coroId = -1;

public:
    struct promise_type {
        inline static std::atomic<int> maxId   = 0;
        int                            id      = ++maxId;
        CoroPool*                      poolPtr = nullptr;
        CoroHdl                        contHdl = nullptr;

        CoroPoolTask get_return_object() noexcept {
            return CoroPoolTask{ CoroHdl::from_promise(*this) };
        }

        auto initial_suspend() const noexcept {
            return std::suspend_always{};
        }

        void unhandled_exception() noexcept {
            std::terminate();
        }

        void return_void() noexcept {}

        auto final_suspend() const noexcept {
            struct FinalAwaiter {
                bool await_ready() const noexcept { return false; }

                void await_resume() noexcept {
                    coutDebug() << "=await resume()" << std::endl;
                }

                /**
                 * @brief 使用 co_await FinalAwaiter{} 最终会调用此函数, 如果当前协程是被别的协程挂起的，则可以还原调用原本的协程
                 * @param h 当前需要挂起的协程句柄
                 * @return
                 */
                std::coroutine_handle<> await_suspend(CoroHdl h) noexcept {

                    if (h.promise().contHdl) {
                        coutDebug() << "=coro" << h.promise().id << "finally suspend"
                                    << "=> continue with coro" << h.promise().contHdl.promise().id << std::endl;
                        return h.promise().contHdl;
                    }
                    else {
                        coutDebug() << "=coro" << h.promise().id << "finally suspended" << std::endl;
                        return std::noop_coroutine();
                    }
                }
            };
            return FinalAwaiter{};
        }
    };

public:
    explicit CoroPoolTask(auto h):
        hdl(h) {}

    ~CoroPoolTask() {
        if (hdl && !hdl.promise().poolPtr) {
            coutDebug() << "=DESTROY coro" << hdl.promise().id << " by task(not by pool)"
                        << "on thread" << std::this_thread::get_id() << std::endl;
        }
    }

    CoroPoolTask(const CoroPoolTask&)            = delete;
    CoroPoolTask& operator=(const CoroPoolTask&) = delete;
    CoroPoolTask& operator=(CoroPoolTask&&)      = delete;

    /**
     * @brief 支持移动构造函数
     * @param tc
     */
    CoroPoolTask(CoroPoolTask&& tc) noexcept:
        hdl(std::exchange(tc.hdl, {})) {
        coutDebug() << "=MOVE coro" << hdl.promise().id << std::endl;
    }

    /**
     * @brief await 对象
     */
    struct CoAwaitAwaiter {
        CoroHdl newHdl;

        /**
         * @brief co_await CoAwaitAwaiter{} 在阶段1确定挂起
         * @return
         */
        bool await_ready() const noexcept {
            return false;
        }

        void await_resume() const noexcept {
        }

        /**
         * @brief co_await CoAwaitAwaiter{} 在阶段1之后挂起，接下来仍然会挂起，但是会在挂起之前进行一些操作
         * @note 这里的操作是存储当前值到newHdl中
         * @param awaitingHdl
         */
        void await_suspend(CoroHdl awaitingHdl) const noexcept;
    };

    /**
     * @brief 该 future 重载了 co_await 操作符
     *
     * @note co_await CoroPoolTask
     * @return 返回一个 await 对象，协程基于返回对象的函数进行不同的操作流程，这里返回的是 CoAwaitAwait 对象
     */
    auto operator co_await() noexcept {
        coutDebug() << "= OPERATOR co_await() for coro " << hdl.promise().id << std::endl;
        coutDebug() << "= POOL tasks ownership of coro" << hdl.promise().id << std::endl;
        return CoAwaitAwaiter{ std::exchange(hdl, nullptr) };
    }
};

class CoroPool {
private:
    std::list<std::jthread>          threads;
    std::list<CoroPoolTask::CoroHdl> coros;

    std::mutex                  corosMx;
    std::condition_variable_any corosCV;
    std::atomic<int>            numCoros = 0; ///<<< 协程池中拥有的协程计数

public:
    /**
     * @brief 初始化协程池，启动若干个线程来执行线程
     * @param num
     */
    explicit CoroPool(int num) {
        for (int i = 0; i < num; ++i) {
            std::jthread worker_thread{
                [this](const std::stop_token& st) {
                    // TODO: 循环执行任务
                    threadLoop(st);
                }
            };
            threads.push_back(std::move(worker_thread));
        }
    }

    /**
     * @brief 析构协程池，使用 C++20 的 stop_token 来控制持续线程的退出
     */
    ~CoroPool() {
        for (auto& t: threads) {
            // 请求线程停止
            t.request_stop();
        }

        for (auto& t: threads) {
            // 等待线程结束
            t.join();
        }

        for (auto& c: coros) {
            // 销毁协程句柄
            c.destroy();
        }
    }
    // 禁止协程池拷贝
    CoroPool(const CoroPool&)            = delete;
    CoroPool& operator=(const CoroPool&) = delete;

    /**
     * @brief 执行一个协程任务,将该协程任务加入到协程池中
     * @param coroTask
     */
    void runTask(CoroPoolTask&& coroTask) noexcept {
        coutDebug() << "= RUN task coro" << coroTask.hdl.promise().id << "by pool" << std::endl;
        coutDebug() << "= POOL takes ownership of coro" << coroTask.hdl.promise().id << std::endl;
        runCoro(std::exchange(coroTask.hdl, nullptr));
    }

    /**
     * @brief 执行协程
     * @param coro
     */
    void runCoro(CoroPoolTask::CoroHdl coro) noexcept {
        coutDebug() << "== runCoro for coro" << coro.promise().id << " = INCR numCoros" << std::endl;
        ++numCoros;
        coro.promise().poolPtr = this;
        {
            std::scoped_lock lock(corosMx);
            coros.push_back(coro);
            // 通知一个线程去 resume
            corosCV.notify_one();
        }
    }

    void threadLoop(const std::stop_token& st) {
        // 如果没收到退出信息则持续执行
        while (!st.stop_requested()) {
            // 尝试从协程句柄队列中获取一个协程
            CoroPoolTask::CoroHdl coro;
            {
                coutDebug() << "      ----> wait by" << std::this_thread::get_id() << std::endl;
                std::unique_lock lock(corosMx);
                if (!corosCV.wait(lock, st, [&] {
                        return !coros.empty();
                    })) {
                    coutDebug() << " ----> STOP for " << std::this_thread::get_id() << std::endl;
                    return;
                }
                coro = coros.back();
                coros.pop_back();
            }
            // resume this coro
            auto id = coro.promise().id;
            coutDebug() << "             ***" << "RESUME coro" << id << " by " << std::this_thread::get_id() << std::endl;
            coro.resume();
            // 如果已经结束，表示该协程已经执行完毕，经历了 final_suspend()
            if (coro.done()) {
                coutDebug() << "           ***" << "SUSPEND coro" << id << " DONE  by " << std::this_thread::get_id() << std::endl;
            }
            else {
                // 此是在 coro 的内部或者更深层调用了 co_await 挂起操作
                coutDebug() << "           ***" << "SUSPEND coro" << id << " by " << std::this_thread::get_id() << std::endl;
            }

            // 此时可以尝试将结束的协程清理掉
            std::function<void(CoroPoolTask::CoroHdl)> destroyDone;
            destroyDone = [&destroyDone, this](auto hdl) {
                if (hdl && hdl.done()) {
                    // 挂起此协程的父协程
                    auto nextHdl = hdl.promise().contHdl;
                    coutDebug() << "=DESTROY coro " << hdl.promise().id << "by pool" << std::endl;
                    hdl.destroy();
                    --numCoros;
                    destroyDone(nextHdl);
                }
            };
            destroyDone(coro);
            numCoros.notify_all();

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void waitUntilNoCoros() {
        int num = numCoros.load();
        coutDebug() << "\nwaitUntilNoCoros():" << num << "left" << std::endl;
        while (num > 0) {
            numCoros.wait(num);
            num = numCoros.load();
            coutDebug() << "\nwaitUtilNoCoros():" << num << "left" << std::endl;
        }
    }

    void syncWait(CoroPoolTask&& task) {
        std::binary_semaphore taskDone{ 0 };

        auto makeWaitTask = [&]() -> CoroPoolTask {
            coutDebug() << "|| waitingTask: co_await task" << std::endl;
            co_await task;
            coutDebug() << "|| waittingTask: co_awit SignalDone{}" << std::endl;

            struct SignalDone {
                std::binary_semaphore& taskDoneRef;

                bool await_ready() const noexcept {
                    return false;
                }

                bool await_suspend(std::coroutine_handle<>) const noexcept {
                    taskDoneRef.release();
                    return false;
                }

                void await_resume() const noexcept {}
            };
            co_await SignalDone{ taskDone };
            coutDebug() << "  waitingTask(): done" << std::endl;
        };

        coutDebug() << "\nsyncWait(): makeWaitingTask()" << std::endl;
        auto t = makeWaitTask();
        coutDebug() << "\nsyncWait(): runTask()" << std::endl;
        runTask(std::move(t));
        taskDone.acquire();
        coutDebug() << "\nsyncWait(): done" << std::endl;
        corosCV.notify_one();
    }
};

/**
 * @brief 协程挂起操作
 * @param awaitingHdl
 */
void CoroPoolTask::CoAwaitAwaiter::await_suspend(CoroPoolTask::CoroHdl awaitingHdl) const noexcept {
    // newHdl 必定存在
    assert(newHdl);
    coutDebug() << "= RUN CORO" << newHdl.promise().id << " via co_await" << "(suspend coro" << awaitingHdl.promise().id << "for it)" << std::endl;

    newHdl.promise().contHdl = awaitingHdl;
    awaitingHdl.promise().poolPtr->runCoro(newHdl);
}

CoroPoolTask print(std::string id, std::string msg) {
    syncOut() << "    > " << id << " print: " << msg << "   on thread: " << std::this_thread::get_id() << std::endl;
    co_return;
}

CoroPoolTask runAsync(std::string id) {
    syncOut() << "===== " << id << " start        on thread: " << std::this_thread::get_id() << std::endl;

    /*
      co_await print(id + "a", "start");
      syncOut() << "===== " << id << " resume       on thread " << std::this_thread::get_id() << std::endl;

      co_await print(id + "b", "end  ");
      syncOut() << "===== " << id << " resume       on thread " << std::this_thread::get_id() << std::endl;
    */
    syncOut() << "===== " << id << " done" << std::endl;
    co_return;
}

int main() {
    // init pool of coroutine threads:
    syncOut() << "**** main() on thread " << std::this_thread::get_id() << std::endl;
    CoroPool pool{ 4 };

    // start main coroutine and run it in coroutine pool:
    syncOut() << "runTask(runAsync(0))" << std::endl;
    CoroPoolTask t0 = runAsync("0");
    pool.runTask(std::move(t0));

    syncOut() << "\n**** t1 = runAsync(1)" << std::endl;
    auto t1 = runAsync("1");
    syncOut() << "\n**** syncWait(t1)" << std::endl;
    pool.syncWait(std::move(t1));

    // start other coroutines:
    for (int i = 0; i < 3; ++i) {
        syncOut() << "runTask(runAsync(" << i + 2 << "))" << std::endl;
        CoroPoolTask t = runAsync(std::to_string(i + 2));
        pool.runTask(std::move(t));
    }

    // wait until all coroutines are done:
    syncOut() << "\n**** waitUntilNoCoros()" << std::endl;
    pool.waitUntilNoCoros();

    syncOut() << "\n**** main() done" << std::endl;
}
