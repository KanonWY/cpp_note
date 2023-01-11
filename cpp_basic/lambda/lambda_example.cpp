#include "sw/redis++/redis++.h"

#include <exception>
#include <future>
#include <iostream>
#include <memory>
#include <string>

using namespace sw::redis;

void example_redis_connect() {

    try {
        // auto rdsp =
        //     std::make_shared<Redis>("tcp://127.0.0.1:6379", [](Redis* rd) {
        //         std::cout << "析构" << std::endl;
        //         rd->~Redis();
        //     });

        auto rdsp = new Redis("tcp://127.0.0.1:6379");
        rdsp->set("kanon", "hello");

        auto ret = rdsp->get("kanon");

        if (ret) {
            std::cout << "ret = " << ret->c_str() << std::endl;
        }

        free(rdsp);
    } catch (const Error& ec) {
        std::cout << "err = " << ec.what() << std::endl;
    }
}

void example_redis_sp() {

    auto sp = std::make_shared<Redis>("tcp://127.0.0.1:6379");
    auto value = sp->get("kanon");

    std::cout << "value = " << value->c_str() << std::endl;
}

void example_redis_sp_deletor() {
    auto de = [](Redis* redis) {
        std::cout << "endDelete" << std::endl;
        redis->~Redis();
    };

    std::shared_ptr<Redis> sp(new Redis("tcp://127.0.0.1:6379"), de);

    auto val = sp->get("kanon");
    if (val) {
        std::cout << "val = " << val->c_str() << std::endl;
    }

    std::cout << "size = " << sizeof(sp) << std::endl;
}

void example_redis_usp_deletor() {
    int a = 200;
    // std::string str("lllll");
    long b = 2900;
    auto de = [](Redis* redis) {
        std::cout << "endDelete, a = " << std::endl;
        redis->~Redis();
    };
    std::unique_ptr<Redis, decltype(de)> sp(new Redis("tcp://127.0.0.1:6379"),
                                            de);
    auto val = sp->get("kanon");
    if (val) {
        std::cout << "val = " << val->c_str() << std::endl;
    }
    std::cout << "size = " << sizeof(sp) << std::endl;
}

class Test {
  public:
    void echo() { std::cout << "inner string = " << m.c_str() << std::endl; }

  private:
    std::string m = "lplpl";
};

int sp_deletor() {

    auto dele = [](Test* test) { std::cout << "end" << std::endl; };

    std::shared_ptr<Test> sp(new Test, dele);
    sp->echo();
    return 0;
}
