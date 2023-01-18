#include <memory>
#include "SourcePool.h"
#include <iostream>
#include <type_traits>
#include <functional>


// shared_ptr   共享式智能指针。
// lambda删除函数
void test_shared_lambda_function() {
    auto deletor = [](SourcePool* ptr) mutable -> void {
        std::cout << "deletor" << std::endl;
    };
    std::shared_ptr<SourcePool> sp(new SourcePool, deletor);
}

// 普通全局删除函数
void deletor_global_deletor(SourcePool* ptr) {
    std::cout << "deletor_global_deletor!" << std::endl;
}

void test_shared_global_function() {
    std::shared_ptr<SourcePool> sp(new SourcePool, deletor_global_deletor);
}

// 包含删除函数的类，SourcePool的删除函数是Su类的一个成员函数。
class Su {
public:
    void deletor_class(SourcePool* ptr) {
        std::cout << "deletor_class" << std::endl;
    }
    static void deletor_class_static(SourcePool* ptr) {
        std::cout << "deletor_class_static" << std::endl;
    }
};

// 无法直接绑定，但是可以使用std::bind,生成一个function对象。
/**
 * @brief 共享智能指针无法直接绑定一个类成员函数，但是可以使用std::bind生成一个仿函数对象。还需要传入是实力化的类成员对象。
 *      类成员函数无法直接被调用，需要使用一个
 */
void test_class_deletor_function() {
    Su                               su;
    std::function<void(SourcePool*)> deletor3 = std::bind(&Su::deletor_class, su, std::placeholders::_1);
    std::shared_ptr<SourcePool>      sp(new SourcePool, deletor3);
}

/**
 * @brief   shared_ptr的删除函数可以是类的静态成员函数。
 */
void test_class_static_deletor_function() {
    std::shared_ptr<SourcePool> sp(new SourcePool, Su::deletor_class_static);
}


int main() {
    test_class_deletor_function();
    return 0;
}