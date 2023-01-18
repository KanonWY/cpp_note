#include "SourcePool.h"
#include <__functional/bind.h>
#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <iostream>


class USP {
public:
    ~USP() {
        std::cout << "~USP()" << std::endl;
    }
    void echo() {
        std::cout << "USP echo() " << std::endl;
    }
};


/**
 * @brief   struct for test unique_ptr
 *
 * @param x
 * @return std::unique_ptr<int>
 */

class Unique_test {
public:
    void echo() const {
        std::cout << m_str.c_str() << std::endl;
    }

private:
    std::string m_str = "Unqiue_test";
};


std::unique_ptr<int> getSP(int x) {
    auto sp = std::unique_ptr<int>(new int(x));
    return sp;
}


void example_make_nullptr() {
    auto sp = std::make_unique<USP>();
    sp      = nullptr;
    std::cout << "end" << std::endl;
}


void example_release() {
    auto sp  = std::make_unique<USP>();
    auto sp2 = sp.release();
    if (sp) {
        std::cout << "yes" << std::endl;
    }
    else {
        std::cout << "removed" << std::endl;
    }
    sp2->echo();
}


void example_move() {
    auto sp  = std::make_unique<USP>();
    auto sp2 = std::move(sp);
    if (sp) {
        std::cout << "sp" << std::endl;
    }
    if (sp2) {
        std::cout << "sp2" << std::endl;
    }
}

/**
 * @brief 测试lambda删除函数
 *
 */
void test_deletor_unique_ptr_lambda() {
    auto unique_deletor = [](Unique_test* rhs) mutable -> void {
        std::cout << "unique_deletor" << std::endl;
    };
    std::unique_ptr<Unique_test, decltype(unique_deletor)> sp(new Unique_test, unique_deletor);
}

// 测试全局删除函数

/**
 * @brief   全局删除函数
 * @param rhs
 */
void deletor_function_global_2(Unique_test* rhs) {
    std::cout << "deletor_function_global_2" << std::endl;
}

void test_deletor_unique_ptr_global_function() {
    std::unique_ptr<Unique_test, std::function<void(Unique_test*)>> sp(new Unique_test, deletor_function_global_2);
    sp->echo();

    std::unique_ptr<Unique_test> sp2(new Unique_test);
}


class Uess {
public:
    static void deletor_class_static(SourcePool* ptr) {
        std::cout << "deletor_class_static" << std::endl;
    }

    void deletor_class(SourcePool* ptr) {
        std::cout << "use Uess deletor_class" << std::endl;
    }
};

void test_deletor_unique_ptr_class_static_function() {
    std::unique_ptr<SourcePool, std::function<void(SourcePool*)>> sp(new SourcePool, Uess::deletor_class_static);
}

void test_deletor_unique_ptr_class_function() {
    Uess                                                          uess;
    auto                                                          deletor_fun2 = std::bind(&Uess::deletor_class, uess, std::placeholders::_1);
    std::unique_ptr<SourcePool, std::function<void(SourcePool*)>> sp(new SourcePool, deletor_fun2);

    std::unique_ptr<SourcePool, std::function<void(SourcePool*)>> sp2(new SourcePool, std::bind(&Uess::deletor_class, uess, std::placeholders::_1));
}

int main() {
    test_deletor_unique_ptr_class_function();
    return 0;
}