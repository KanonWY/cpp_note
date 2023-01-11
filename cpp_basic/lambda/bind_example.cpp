#include <functional>
#include <iostream>
#include <string>

double callFunc(double x, double y) { return x + y; }

int bind_normal_function() {
    auto NewCallable = std::bind(callFunc, std::placeholders::_1, 100);
    std::cout << NewCallable(200) << std::endl;
    return 0;
}

class Base {
  public:
    void display_sum(int a1, int a2) {
        std::cout << "display_sum = " << a1 + a2 << std::endl;
    }
};

void bind_class_function() {
    Base base;
    auto newFunction =
        std::bind(&Base::display_sum, &base, std::placeholders::_1, 200);
    newFunction(2000);
}

// 需要被绑定的函数
void TestBindFunction(std::string& str, double b) {
    str.append(std::to_string(b));
    std::cout << "Bind string = " << str.c_str() << std::endl;
}

// 传递引用
void bind_ref_function() {
    std::string str("BBBBBBBBBBBBBBBB");
    std::cout << "str = " << str.c_str() << std::endl;
    auto foo =
        std::bind(TestBindFunction, std::ref(str), std::placeholders::_1);
    foo(2999);
    std::cout << "str = " << str.c_str() << std::endl;
}

// 传递值
void bind_value_function() {
    std::string str("BBBBBBBBBBBBBBBB");
    std::cout << "str = " << str.c_str() << std::endl;
    auto foo = std::bind(TestBindFunction, str, std::placeholders::_1);
    foo(2999);
    std::cout << "str = " << str.c_str() << std::endl;
}

int main() {
    bind_value_function();
    return 0;
}