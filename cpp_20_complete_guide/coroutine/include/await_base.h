#pragma once

#include <iostream>

namespace eon {
class Awaiter {
public:
    bool await_ready() const noexcept {
        std::cout << "await_ready()" << std::endl;
        // 返回 false 运行未准备，因此挂起
        return false;
    }

    /**
     * @brief 在协程挂起之后立即为协程调用此函数
     * @param hdl 被挂起协程的句柄
     * @note 此函数中可以指定下一个步需要做什么，立即恢复协程或者等待协程。
     * @note 这里返回的是 void 因此需要挂起当前协程，然后将控制权返回给调用者
     */
    void await_suspend(auto hdl) const noexcept {
        std::cout << "await_suspend()" << std::endl;
    }

    /**
     * @brief 当协程成功挂起后恢复协程时调用此函数，即成功挂起后再次调用 resume() 之前调用此函数
     * hdl->resume() 内部前面调用！！！
     */
    void await_resume() const noexcept {
        std::cout << "await_resume() " << std::endl;
    }
};
} // namespace eon
