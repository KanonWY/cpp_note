#include <iostream>
#include <memory>

/**
 * @brief 简单的双重分发的例子
 * @note 使用虚函数的分发机制最终调用实际的例子
 */
struct stuff
{
    virtual void call() = 0;
};
struct A;
struct B;

void foo(A* a) { std::cout << "call a" << std::endl; }
void foo(B* b) { std::cout << "call b" << std::endl; }

struct A : public stuff
{
    void call() override { foo(this); }
};

struct B : public stuff
{
    void call() override { foo(this); }
};

int main()
{
    std::shared_ptr<stuff> p1 = std::make_shared<A>();

    p1->call();

    std::shared_ptr<stuff> p2 = std::make_shared<B>();

    p2->call();

    return 0;
}
