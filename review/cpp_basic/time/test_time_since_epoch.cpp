#include <iostream>
#include <chrono>
#include <ctime>


//1s = 1000 ms = 1000000 us = 1000000000 ns
using secClock_type = std::chrono::time_point<std::chrono::system_clock,std::chrono::seconds>;  //
using msClock_type = std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds>;  //ms
using usClock_type = std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>; // us
using nsClock_type = std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>;  //ns 

int testSecond()
{
    auto base = std::chrono::system_clock::now();
    secClock_type sp = std::chrono::time_point_cast<std::chrono::seconds>(base);
    std::time_t tt = std::chrono::system_clock::to_time_t(sp);
    std::cout << "tt = " << tt << std::endl;
    std::cout << "tt2 = " << sp.time_since_epoch().count() << std::endl;
    return 0;
}


void testMs()
{
    auto base = std::chrono::system_clock::now();
    msClock_type sp = std::chrono::time_point_cast<std::chrono::milliseconds>(base);
    std::time_t tt = std::chrono::system_clock::to_time_t(sp);
    std::cout << "tt = " << tt << std::endl;
    std::cout << "tt2 = " << sp.time_since_epoch().count() << std::endl;
}

void testUs()
{
    auto base = std::chrono::system_clock::now();
    usClock_type sp = std::chrono::time_point_cast<std::chrono::microseconds>(base);
    std::time_t tt = std::chrono::system_clock::to_time_t(sp);      //返回s
    std::cout << "tt = " << tt << std::endl;
    std::cout << "tt2 = " << sp.time_since_epoch().count() << std::endl;
}

void testNs()
{
    auto base = std::chrono::system_clock::now();
    nsClock_type sp = std::chrono::time_point_cast<std::chrono::nanoseconds>(base);
    std::cout << "tt2 = " << sp.time_since_epoch().count() << std::endl;        //始终返回ns
}


int main()
{
    testNs();
    return 0;
}
