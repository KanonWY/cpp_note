#include <future>
#include <iostream>
#include <string>

// 捕获*this
class Work {
  public:
    Work() : value(42) {}

    std::future<int> spawn() {
        return std::async([=]() -> int { return value; });
    }

    std::future<int> spawn_2() {
        return std::async(
            [=, this]() -> int { return value; }); //[=,this]捕获this指针
    }

    std::future<int> spawn_safe() {
        return std::async([=, *this]() -> int {
            return value;
        }); // 捕获*this，本质是复制了一份类对象
    }

  private:
    int value;
};

class Test {

  public:
    Test(const std::string str) {
        m_str = str;
        std::cout << "string cs" << std::endl;
    }
    Test(const Test& rhs) {
        m_str = rhs.m_str;
        std::cout << "copy" << std::endl;
    }
    Test& operator=(const Test& rhs) {
        m_str = rhs.m_str;
        std::cout << "copy =" << std::endl;
        return *this;
    }

    Test(Test&& rhs) noexcept {
        m_str = std::move(rhs.m_str);
        std::cout << "move" << std::endl;
    }

    Test& operator=(Test&& rhs) noexcept {
        m_str = std::move(rhs.m_str);
        std::cout << "move = " << std::endl;
        return *this;
    }

    void echo_const() const {
        std::cout << "str = " << m_str.c_str() << std::endl;
    }

    void echo_no_const() {
        std::cout << "str = " << m_str.c_str() << std::endl;
    }

  private:
    std::string m_str;
};

int func_const() {
    int a = 100;
    int b = 200;
    return a + b;
}

int main() {

    auto foo = []() -> void { func_const(); };
    Test test("dlapdlp");
    auto fun1 = [&test]() -> void {
        test.echo_const();
        test.echo_no_const();
    };
}
