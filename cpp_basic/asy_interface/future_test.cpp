#include <future>
#include <thread>
#include <chrono>
#include <iostream>

int testThread(std::promise<int>& res) {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    res.set_value(100);
    return 0;
}

int main() {
    std::promise<int> res;
    std::future<int>  f = res.get_future();

    std::thread t1(&testThread, std::ref(res));
    std::cout << f.get() << std::endl;

    t1.join();
    return 0;
}