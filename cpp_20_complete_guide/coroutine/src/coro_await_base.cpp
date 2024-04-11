#include "await_base.h"
#include "coroTask.h"


using namespace eon;

/**
 * @brief 协程函数
 * @param max
 * @return
 */
CoroTask coro(int max) {
    std::cout << "CORO START" << std::endl;
    for (int i = 1; i <= max; ++i) {
        std::cout << "CORO inner [ " << i << " ]" << std::endl;
        co_await eon::Awaiter{};
    }
}


int main() {
    auto task = coro(2);
    std::cout << "MAIN: START" << std::endl;

    while (task.resume()) {
        std::cout << "BACK MAIN coro" << std::endl;
    }

    std::cout << "MAIN: EDN" << std::endl;

    return 0;
}