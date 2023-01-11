#include <future>
#include <iostream>
#include <string>

// 广义捕获
int example_move_lambda() {

    int a = 200;
    int c = 3000;
    std::string str("lllll");
    std::cout << "str.size() = " << str.size() << std::endl;
    auto foo = [a, st = std::move(str)]() mutable {
        std::cout << "st = " << st.c_str() << std::endl;
        return a;
    };
    std::cout << "str.size() = " << str.size() << std::endl;
    foo();
    return 0;
}

// 异步调用时复制this对象
class Work {
  public:
    Work() : value(42) {}

    std::future<int> spawn() {
        return std::async([=]() -> int { return value; });
    }

    std::future<int> spawn_safe() {
        return std::async([=, tmp = *this]() { return tmp.value; });
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

// 捕获将亡值
int example_move_capture() {

    auto foo = [st = std::string("lpdlapdlpal")]() mutable -> std::string {
        st.append("===========>");
        return st;
    };
    std::cout << foo().c_str() << std::endl;
    return 0;
}

// 泛型lambda表达式

int main() {
    example_this_unsafe();
    example_this_safe();
    return 0;
}