#include <memory>
#include <string>
#include <type_traits>
#include "SourcePool.h"


void test_shared_ptr() {
    std::shared_ptr<SourcePool> sp;
    sp.reset();
    sp.use_count();
    sp.unique();
}

int test2() {

    auto sp = std::make_shared<SourcePool>();
    sp->echo();
    std::shared_ptr<SourcePool> p;
    std::cout << p.use_count() << std::endl;
    SourcePool* l = new SourcePool;

    std::shared_ptr<SourcePool> spp(l);

    auto del1 = [](SourcePool* rhs) mutable -> void {
        std::cout << "end" << std::endl;
    };

    std::shared_ptr<SourcePool> sp2(new SourcePool, del1);


    return 0;
}

int test3() {
    bool a = std::is_convertible<char*, std::string>();
    std::cout << a << std::endl;
    int p[10] = { 0 };
    int k     = 20;
    int b     = std::is_array<decltype(p)>();
    return 0;
}

template<typename T>
T foo(T& t) {
    std::cout << "foo(T&)" << std::endl;
    T tt(t);
    return tt;
}

void foo(...) {
    std::cout << "foo(...)" << std::endl;
}


template<typename _Tp, class... Args, std::enable_if_t<!std::is_array<_Tp>::value>>
int make_test(Args&&... args) {
    std::cout << "aaa" << std::endl;
    return 0;
}

template<typename _Tp, class... Args, std::enable_if_t<std::is_array<_Tp>::value>>
void make_test(Args&&... args) {
    std::cout << "bbb" << std::endl;
}

class Test {};

int main() {
    // 注意,函数参数必须为对象指针或者const对象指针
    auto del1 = [](const SourcePool* p) mutable -> void {
        std::cout << "end" << std::endl;
    };


    std::shared_ptr<SourcePool> sp2(new SourcePool, del1);
    sp2->echo();

    std::shared_ptr<SourcePool> sp3(new SourcePool);

    auto sp4 = std::make_unique<SourcePool>();

    std::shared_ptr<SourcePool> sp5(std::move(sp4));

    return 0;
}