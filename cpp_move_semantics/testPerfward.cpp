#include <iostream>

struct Base {
    Base() = default;
    virtual ~Base() = default;

    void func() {}
    int a;
    int b;
};

int main() {

    std::cout << sizeof(Base) << std::endl;
    return 0;
}