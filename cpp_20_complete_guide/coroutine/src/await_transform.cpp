#include <iostream>
#include "await_transform.h"

using namespace eon;


CoroPrioTask coro(int max) {
    std::cout << "CORO STATRT { " << max << " }" << std::endl;

    for (int val = 1; val <= max; ++val) {
        std::cout << "CORO[" << max << "]: " << val << std::endl;
        co_await CoroPrioRequest::less;
    }
    std::cout << "CORO END    { " << max << " }" << std::endl;
}


int main() {
    std::cout << "<< MAIN >>" << std::endl;

    CoroPrioScheduler sched;

    sched.start(coro(5));
    sched.start(coro(1));
    sched.start(coro(4));

    std::cout << "LOOP ->" << std::endl;

    while (sched.resumeNext()) {
        std::cout << "BACK TO MAIN" << std::endl;
    }


    std::cout << "<< MAIN END >>" << std::endl;

    return 0;
}
