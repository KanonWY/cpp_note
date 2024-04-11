#include "coroGenerator.h"
#include <coroutine>
#include <iostream>


using namespace eon;

Generator<int> coro(int max) {
    std::cout << "GEN_CORO run" << std::endl;

    for (int i = 1; i <= max; ++i) {
        std::cout << "CORO INNE [ " << i << " ]" << std::endl;
        co_yield i;
    }
    std::cout << "GEN_CORO end" << std::endl;
}


int main() {
    auto task = coro(2);
    std::cout << "Main RUN" << std::endl;

    for (const auto& val: task) {
        std::cout << "value = " << val << std::endl;
    }

    std::cout << "Main END" << std::endl;
    return 0;
}