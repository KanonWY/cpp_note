#pragma once

#include <map>
#include <coroutine>
#include <exception>
#include <utility>

namespace eon {

/**
 * @brief 协程运行优先级
 */
enum class CoroPrioRequest {
    same,
    less,
    more,
    def
};


class CoroPrioScheduler;
struct CoroPrio;
inline int CoroPrioDefVal = 10;

/**
 * @brief 带有优先级的协程任务
 */
class [[nodiscard]] CoroPrioTask {
    friend class CoroPrioScheduler;

public:
    struct promise_type {
        // give access to the handle
        CoroPrioScheduler* schedPtr = nullptr;

        auto get_return_object() {
            return CoroPrioTask{ CoroHdl::from_promise(*this) };
        }

        auto initial_suspend() {
            return std::suspend_always{};
        }

        auto final_suspend() noexcept {
            return std::suspend_always{};
        }

        void unhandled_exception() {
            std::terminate();
        }

        void return_void() {}
    };
    using CoroHdl = std::coroutine_handle<promise_type>;

public:
    explicit CoroPrioTask(CoroHdl h):
        hdl(h) {}

    ~CoroPrioTask() {
        if (hdl) {
            hdl.destroy();
        }
    }

    // move only
    CoroPrioTask(const CoroPrioTask&)            = delete;
    CoroPrioTask& operator=(const CoroPrioTask&) = delete;

    CoroPrioTask(CoroPrioTask&& ct) noexcept:
        hdl(std::exchange(ct.hdl, {})) {}

    CoroPrioTask& operator=(CoroPrioTask&& ct) noexcept {
        hdl = std::exchange(ct.hdl, {});
        return *this;
    }

private:
    CoroHdl hdl;
};


/**
 * @brief 简答协程调度器
 */
class CoroPrioScheduler {
    // 存储所有的任务根据优先级
    std::multimap<int, CoroPrioTask> tasks;

public:
    void start(CoroPrioTask&& task) {
        task.hdl.promise().schedPtr = this;
        tasks.emplace(CoroPrioDefVal, std::move(task));
    }
    bool resumeNext() {
        if (tasks.empty()) {
            return false;
        }
        auto taskPos = tasks.begin();
        auto hdl     = taskPos->second.hdl;
        while (!hdl || hdl.done()) {
            ++taskPos;
            if (taskPos == tasks.end()) {
                return false;
            }
            hdl = taskPos->second.hdl;
        }
        hdl.resume();
        if (hdl.done()) {
            tasks.erase(taskPos);
        }
        return !tasks.empty();
    }

    bool changePrio(CoroPrioTask::CoroHdl hdl, CoroPrioRequest pr) {
        for (auto pos = tasks.begin(); pos != tasks.end(); ++pos) {
            if (hdl == pos->second.hdl) {
                int newPrio = pos->first;
                switch (pr) {
                    case CoroPrioRequest::same:
                        break;
                    case CoroPrioRequest::less:
                        ++newPrio;
                        break;
                    case CoroPrioRequest::def:
                        newPrio = CoroPrioDefVal;
                        break;
                    case CoroPrioRequest::more:
                        break;
                }
                if (pos->first != newPrio) {
                    auto nh  = tasks.extract(pos);
                    nh.key() = newPrio;
                    tasks.insert(std::move(nh));
                }
                return true;
            }
        }
        return false;
    }
};

/**
 * @brief CoroPrio 是一个 Awaiter 对象
 */
struct CoroPrio {
private:
    CoroPrioRequest prioRequest;

public:
    explicit CoroPrio(CoroPrioRequest pr):
        prioRequest(pr) {}


    /**
     * @brief 是否先挂起当前程序
     */
    bool await_ready() const noexcept {
        return false;
    }

    /**
     * @brief 由于返回的void，因此在执行完 await 内部的函数之后将会挂起
     * @param h 挂起的协程的句柄
     */
    void await_suspend(CoroPrioTask::CoroHdl h) noexcept {
        h.promise().schedPtr->changePrio(h, prioRequest);
    }

    /**
     * @brief co_await expr 返回的值
     */
    auto await_resume() const noexcept {}
};

} // namespace eon
