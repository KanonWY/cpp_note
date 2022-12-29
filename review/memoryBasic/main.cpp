#include "KaSmartPointer.h"
#include <iostream>

struct test {
    int a = 10;
    int b = 20;
};

int main() {
    std::cout << "end" << std::endl;
    auto sp = make_kaUnique_ptr<test>();
    std::cout << "sp->a = " << sp->b << std::endl;
    return 0;
}