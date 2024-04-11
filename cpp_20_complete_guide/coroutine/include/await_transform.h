#include <coroutine>
#include <exception>
#include <map>
#include <utility>

namespace eon {


// 前置声明

class CoroPrioScheduler;
struct CoroPrio;

inline int CoroPrioDefVal = 10;


/**
 * @brief 优先级请求范围枚举
 */
enum class CoroPrioRequest {
    same,
    less,
    more,
    def
};

class [[nodiscard]] CoroPrioTask {
public:
    struct promise_type;
    using CoroHdl = std::coroutine_handle<promise_type>;

private:
    CoroHdl hdl;
    friend CoroPrioScheduler;

public:
    explicit CoroPrioTask(auto f):
        hdl(f) {}

    ~CoroPrioTask() {
        if (hdl) {
            hdl.destroy();
        }
    }

    CoroPrioTask(CoroPrioTask&& tc) noexcept
        :
        hdl(std::exchange(tc.hdl, {})) {
    }

    CoroPrioTask& operator=(CoroPrioTask&& tc) {
        hdl = std::exchange(tc.hdl, {});
        return *this;
    }

public:
    struct promise_type {
        CoroPrioScheduler* schedPtr = nullptr;

        auto get_return_object() {
            return CoroPrioTask{ CoroHdl ::from_promise(*this) };
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

        /**
         * @brief promise_type 中的 await_transform 函数
         * @note co_await expr 出现时
         *       将会查找由于协程提供的成员函数，await_transform
         *       如果存在将会调用 await_transform*() 并且产生一个等待器
         *       然后利用该等待器挂起协程
         */
        auto await_transform(CoroPrioRequest);

        void return_void() {}
    };
};

class CoroPrioScheduler {
    std::multimap<int, CoroPrioTask> tasks;

public:
    void start(CoroPrioTask&& task) {
        task.hdl.promise().schedPtr = this;
        tasks.emplace(CoroPrioDefVal, std::move(task));
    }

    /**
     * @brief 协程管理恢复执行所有管理的协程
     */
    bool resumeNext() {
        if (tasks.empty()) {
            return false;
        }
        auto taskPos = tasks.begin();

        auto hdl = taskPos->second.hdl;
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

    /**
     * @brief 改变指定协程的优先级 by 协程句柄
     */
    bool changePrio(CoroPrioTask::CoroHdl hdl, CoroPrioRequest pr) {
        for (auto pos = tasks.begin(); pos != tasks.end(); ++pos) {
            if (hdl == pos->second.hdl) {
                int newPrio = pos->first;
                switch (pr) {
                    case eon::CoroPrioRequest::same:
                        break;
                    case eon::CoroPrioRequest::less:
                        ++newPrio;
                        break;
                    case eon::CoroPrioRequest::more:
                        --newPrio;
                        break;
                    case eon::CoroPrioRequest::def:
                        newPrio = CoroPrioDefVal;
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

// co_await CoroPrioRequest::less 将会调用这里
inline auto CoroPrioTask::promise_type::await_transform(CoroPrioRequest pr) {
    // 根据 promise_type 获取当前句柄
    auto hdl = CoroPrioTask::CoroHdl::from_promise(*this);
    // 切换当前协程的优先级
    schedPtr->changePrio(hdl, pr);
    // 返回一个等待器
    return std::suspend_always{};
}


} // namespace eon
