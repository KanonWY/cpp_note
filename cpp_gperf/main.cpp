#include "gperftoolsWrap.h"
#include <atomic>
#include <list>
#include <memory>
#include <thread>

void func1()
{
    int i = 0;
    while (i < 100000) {
        ++i;
    }
}
void func2()
{
    int i = 0;
    while (i < 200000) {
        ++i;
    }
}
void func3()
{
    for (int i = 0; i < 1000; ++i) {
        func1();
        func2();
    }
}

struct Test
{
    int a[1024];  // 4*1024 = 4k
};

void f1()
{
    int i;
    for (i = 0; i < 10240; ++i) {
        auto sp     = std::make_shared<Test>();
        sp->a[1023] = 100;
    }
}

void f2()
{
    int i;
    for (i = 0; i < 10240; ++i) {
        auto sp     = std::make_shared<Test>();
        sp->a[1023] = 100;
    }
}

void example_test_cpu_profile()
{
    kanon::gperftoolsWrapper::instance().StartCpuProfiler();
    func3();
    kanon::gperftoolsWrapper::instance().EndCpuProfiler();
}

void example_test_heap_profile()
{
    std::vector<Test> vec;
    vec.reserve(100000);
    std::vector<Test> vec2;
    vec2.reserve(100000);
    kanon::gperftoolsWrapper::instance().HeapSample();
}

void example_test_heap_growth_profile()
{
    std::vector<Test> vec;
    vec.reserve(100000);
    std::vector<Test> vec2;
    vec2.reserve(100000);
    kanon::gperftoolsWrapper::instance().HeapGrowthStacks();
}

int main()
{
    example_test_heap_growth_profile();
    return 0;
}