#include <iostream>
#include <functional>
#include <string>

/**
 * @brief bind函数可以绑定一个函数，然后生成一个函数对象。 std::function
 * 绑定的对象可以是全局函数，lambda函数，或者成员函数。
 */


class TestFunction {
public:
    void echo() {
        std::cout << "TestFunction echo ......" << std::endl;
    }

    static void echo_static() {
        std::cout << "TestFunctuon echo_static ......" << std::endl;
    }
};


/**
 * @brief   绑定一个成员函数
 *
 */

int main() {
    std::cout << "hello world" << std::endl;
    return 0;
}