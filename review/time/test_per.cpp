#include <iostream>
#include <chrono>

int main()
{
    auto s = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(s);

    std::cout << "time = " << t << std::endl;
    return 0;
}