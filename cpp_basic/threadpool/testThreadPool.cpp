#include "ThreadPool.h"
#include <chrono>
#include <iostream>
#include <thread>

void Hello_world(const std::string& str) {
    std::cout << str.c_str() << std::endl;
}


int main() {
    std::string       str("hello world");
    kanon::ThreadPool pool(10);

    pool.enqueue(Hello_world, std::ref(str));

    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}