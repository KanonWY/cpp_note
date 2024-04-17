#include <coroutine>
#include <iostream>

class Awaiter
{
public:
    bool await_ready() const noexcept
    {
        std::cout << "await_ready" << std::endl;
        return false;
    }

    void await_suspend(auto hdl) const noexcept
    {
        std::cout << "await_suspend()" << std::endl;
    }

    void await_resume() const noexcept
    {
        std::cout << "await_resume()" << std::endl;
    }
};

