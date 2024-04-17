#include <coroutine>

template <typename T>
class LazyTask
{
public:
    struct promise_type;
    using CoroHdl = std::coroutine_handle<promise_type>;

private:
    /// 协程句柄
    CoroHdl m_hdl;

public:
    struct promise_type
    {
        T m_value;
        auto init_suspend() noexcept { return std::suspend_always{}; }

        auto get_return_object() noexcept
        {
            return LazyTask(CoroHdl::from_promise(*this));
        }
        auto final_suspend() noexcept
        {
            return std::suspend_always{};

            void return_void() {}
        };

    public:
        explicit LazyTask(auto h) : m_hdl(h) {}
        ~LazyTask()
        {
            if (m_hdl)
            {
                m_hdl.destroy();
            }
        }
        // no copy
        LazyTask(const LazyTask &) = delete;
        LazyTask &operator=(const LazyTask &) = delete;
    };

    int main() { return 0; }