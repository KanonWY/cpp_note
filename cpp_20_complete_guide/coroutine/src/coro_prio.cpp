#include "coroPrio.h"
#include <iostream>

using namespace eon;

CoroPrioTask coro(int max) {
    std::cout << "CORO START < " << max << " >" << std::endl;
    for (int val = 1; val <= max; ++val) {
        std::cout << "CORO < " <<max << " > "<<" IN [ " << val << " ]" << std::endl;
        co_await CoroPrio{ CoroPrioRequest::less };
    }
    std::cout << "CORO END" << std::endl;
}


int main() {

    std::cout << "Main Start" << std::endl;

    CoroPrioScheduler sched;

    std::cout << "schedule corotines" << std::endl;
    sched.start(coro(5));
    sched.start(coro(1));
    sched.start(coro(4));

    std::cout << "loop until all finished" << std::endl;
    while (sched.resumeNext()) {}

    std::cout << "Main end" << std::endl;

    return 0;
}