#include <future>
#include <iostream>
#include <string>

// 常量表达式

// 捕获*this
class Work {
  public:
    Work() : value(42) {}

    std::future<int> spawn() {
        return std::async([=]() -> int { return value; });
    }

    std::future<int> spawn_safe() {
        return std::async([=, *this]() { return value; }); // 捕获*this
    }

  private:
    int value;
};

std::future<int> foo() {
    Work tmp;
    return tmp.spawn();
}

std::future<int> foo_safe() {
    Work tmp;
    return tmp.spawn_safe();
}

void example_this_unsafe() {
    auto f = foo();
    f.wait();
    std::cout << "f.get() = " << f.get() << std::endl;
}

void example_this_safe() {
    auto f = foo_safe();
    f.wait();
    std::cout << "f.get() = " << f.get() << std::endl;
}

int main() {
    example_this_unsafe();
    example_this_safe();
    return 0;
}