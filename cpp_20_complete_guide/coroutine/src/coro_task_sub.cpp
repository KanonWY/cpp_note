#include "coroTaskSub.h"

#include <coroutine>
#include <iostream>

using namespace eon;

CoroTaskSub coro() {
    std::cout << "CORO 前半部分" << std::endl;
    co_await std::suspend_always{};
    std::cout << "CORO 后半部分" << std::endl;
}

CoroTaskSub callCoro() {
    std::cout << "callCoro 协程启动" << std::endl;
    std::cout << "即将调用 coro" << std::endl;
    co_await coro();

    std::cout << "coro 调用完毕" << std::endl;

    co_await std::suspend_always{};
    std::cout << "callCoro 协程结束" << std::endl;
}


int main() {
    auto task = callCoro();
    std::cout << "Main: 启动" << std::endl;

    while (task.resume()) {
        std::cout << "BACK MAIN function" << std::endl;
    }

    std::cout << "Main: 结束" << std::endl;
    return 0;
}
