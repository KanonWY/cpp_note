#include <chrono>
#include <future>
#include <random>
#include <sstream>
#include <string>
#include <iostream>
#include <thread>

void async_function(std::promise<std::string>& pro) {
    std::string str("ldpaldla");
    pro.set_value(str);
}

int test_promise_future() {
    std::promise<std::string> prom;
    // std::future<std::string> future = p
    std::future<std::string> fu = prom.get_future();
    //
    auto res = std::async(std::launch::async, async_function, std::ref(prom));
    std::cout << fu.get() << std::endl;
    return 0;
}


int main() {
    auto              id = std::this_thread::get_id();
    std::stringstream os;
    os << id;
    auto strId = os.str();
    std::cout << strId.c_str() << std::endl;
    std::cout << id << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}