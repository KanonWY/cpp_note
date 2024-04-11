#include "coroTask.h"


using namespace eon;
/**
 * @brief 简单的协程函数
 * @param max
 * @return
 */
CoroTask coro(int max) {
    std::cout << "CORO start" << std::endl;

    for (int value = 1; value <= max; ++value) {
        std::cout << "coro IN [ " << value << " ]" << std::endl;
        co_await std::suspend_always{};
    }
    std::cout << "CORO end" << std::endl;
}


int main() {

    std::cout << "MAIN START" << std::endl;
    auto task = coro(10);

    while (task.resume()) {
        std::cout << "coro suspended" << std::endl;
    }

    std::cout << "MAIN END" << std::endl;
    return 0;
}