#include <coroutine>
#include <iostream>
#include <vector>
#include <ranges>

#include "coroReturn.h"

using namespace eon;


/**
 * @brief 该协程接收一个 int 数组，使用 co_yield 返回挂起时的值，使用 co_return 返回最终值
 * @param coll
 * @return
 */
ResultTask<int> average(auto coll) {
    std::cout << "协程 average 开始" << std::endl;
    double sum = 0;
    for (const auto& item: coll) {
        std::cout << "process " << item << std::endl;
        sum += item;
        co_yield item;
    }
    std::cout << "协程 average 结束" << std::endl;
    co_return sum / std::ranges::ssize(coll);
}


int main() {
    std::vector<int> coll{ 1, 2, 3, 4, 5 };
    auto             task = average(std::views::all(coll));

    std::cout << "MAIN BEGIN" << std::endl;

    while (task.resume()) {
        std::cout << "BACK TO MAIN" << std::endl;
        std::cout << "co_yield value = " << task.getYieldValue() << std::endl;
    }

    std::cout << "result: " << task.getReturnValue() << std::endl;
    std::cout << "MAIN END" << std::endl;
    return 0;
}