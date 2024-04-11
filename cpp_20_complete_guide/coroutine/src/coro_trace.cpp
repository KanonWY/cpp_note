#include "coroTrace.h"


using namespace trace;


CoroTraceTask coro(int main) {
    std::cout << "追踪协程开始运行" << std::endl;

    for (int i = 1; i < main; ++i) {
        std::cout << "IN CORO" << i << std::endl;
        // 调用 co_yield 基本等于 co_await task.promise().yield_value(i)
        co_yield i;
    }

    auto co_await_str = co_await TraceAwaiter{};
    std::cout << "co_await_str = " << co_await_str << std::endl;

    std::cout << "追踪协程即将结束" << std::endl;

    co_return std::string{ "XXX" };
}


int main() {

    auto task = coro(2);

    std::cout << "MAIN ==>" << std::endl;

    while (task.resume()) {
        std::cout << "BACK MAIN" << std::endl;

        std::cout << task.getYieldValue() << std::endl;
    }

    std::cout << task.getReturnValue() << std::endl;

    std::cout << "MAIN <==" << std::endl;


    return 0;
}
