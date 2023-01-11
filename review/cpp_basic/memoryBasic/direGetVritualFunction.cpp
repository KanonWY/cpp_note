#include <cstring>
#include <iostream>

class Base {
  public:
    virtual void foo() { std::cout << "Base::foo()" << std::endl; };
    virtual ~Base() { std::cout << "Base::~Base()" << std::endl; }
};

class Test : public Base {
  public:
    void foo() override { std::cout << "Test::foo()" << std::endl; }
    virtual void moo() { std::cout << "Test::moo()" << std::endl; }
    virtual ~Test() { std::cout << "Test::~Test()" << std::endl; }
};

typedef void (*pFunc)();

int dfir() {

    Test test;

    uint64_t *p;

    memcpy(&p, &test, 8);
    pFunc p1 = (pFunc) * (p);

    pFunc p2 = (pFunc) * (p + 1);
    pFunc p3 = (pFunc) * (p + 2);
    pFunc p4 = (pFunc) * (p + 3);

    std::cout << "------------------- p1();----------------------" << std::endl;
    p1();
    std::cout << "--------------------- p2();--------------------" << std::endl;
    p2();
    std::cout << "-------------------  p3()----------------------" << std::endl;
    // p3();
    std::cout << "---------------------p4()--------------------" << std::endl;
    p4();
    std::cout << "-----------------------------------------" << std::endl;
    return 0;
}

int main() { return 0; }