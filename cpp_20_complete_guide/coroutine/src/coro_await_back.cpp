#include "awaitBack.h"
#include <iostream>

using namespace eon;

CoroGenBack coro(int max) {
    std::cout << "GENBACK begin" << std::endl;

    for (int val = 1; val <= max; ++val) {
        std::cout << "IN COROI [ " << val << " ]" << std::endl;
        auto back = co_yield val;
        std::cout << "back Value = " << back << std::endl;
    }
    std::cout << "GENBACK end" << std::endl;
}

int main() {

    auto task = coro(20);
    std::cout << "MAIN" << std::endl;
    std::string in{ "" };
    while (task.resume()) {
        std::cout << "回到主程序" << std::endl;
        std::cout << "获取值: " << task.getValue() << std::endl;
        in.append("X");
        std::cout << "传入值 ==> " << in << std::endl;
        task.setBackValue(in);
    }
    std::cout << "MAIN" << std::endl;
}
