#include "coroYield.h"
#include <coroutine>
#include <iostream>

using namespace eon;


/**
 * @brief 协程函数
 * @param max 传入参数
 * @return
 */
CoroYield<int> coro(int max) {
    std::cout << "CORO START" << std::endl;

    for (int val = 1; val <= max; ++val) {
        std::cout << "IN CORO [ " << val << " ]" << std::endl;
        co_yield val;
    }
    std::cout << "CORO END" << std::endl;
}

int main() {

    auto task = coro(2);

    std::cout << "Main: start" << std::endl;

    while (task.resume()) {
        std::cout << "回到 Main 函数" << std::endl;
        std::cout << "从 协程中返回的值为: " << task.getYieldValue() << std::endl;
    }

    std::cout << "Main: end" << std::endl;

    return 0;
}