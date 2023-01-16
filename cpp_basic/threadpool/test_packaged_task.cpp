#include <future>
#include <iostream>


void test_lambda() {
    auto foo = [](int x, int y) mutable -> int {
        return x + y;
    };
    std::packaged_task<int(int, int)> task(foo);
    task(2, 200);
    std::future<int> res = task.get_future();
    std::cout << "lambda res = " << res.get() << std::endl;
}


int main() {
    test_lambda();
    return 0;
}