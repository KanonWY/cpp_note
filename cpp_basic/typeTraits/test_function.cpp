#include <functional>
#include <iostream>

/**
 * @brief std::function：可调用对象包装器，是C++11新标准引入的类模板。
 *
 *  函数名（函数指针）、具有operator()成员函数的类对象、lambda表达式等等，都属于可调用对象。
 *
 */

static void myfunc(int tmp)
{
    std::cout << "exec " << __FUNCTION__ << "tmp = " << tmp << std::endl;
}

void callFunction(std::function<void(int)> function_object)
{
    function_object(999);
}

int basic_example()
{
    callFunction(myfunc);
    callFunction(
        [](int x) { std::cout << "lambda function x = " << x << std::endl; });
    return 0;
}

int main()
{
    basic_example();
    return 0;
}