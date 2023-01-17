#include <iostream>
#include "boost/type_index.hpp"
#include <string>


void func(int x) {
    std::cout << "func(int x)" << std::endl;
}

void func(double x) {
    std::cout << "func(double x)" << std::endl;
}

template<typename T>
void func(T t) {
    std::cout << "template func" << std::endl;
}

void func(...) {
    std::cout << "func(...)" << std::endl;
}


int testBoost_type() {
    auto a    = 2000;
    auto type = boost::typeindex::type_id_with_cvr<decltype(a)>().pretty_name();
    std::cout << type.c_str() << std::endl;
    return 0;
}

struct B {
    operator int() const { return 0; }
};


int main() {
    func(1000);
    func(199.01);
    std::string Hello("hello world");
    B           b;
    func(b);
    func(Hello);
    return 0;
}
