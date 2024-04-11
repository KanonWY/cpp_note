#include <iostream>
#include <string>
#include <variant>


/**
 * @brief 测试
 * 如果 std::variant 同时存在 std::string 和 bool，
 * 赋值一个 字符串字面量的时候，会优先匹配 bool 而不是 std::string ???
 * 解决方式：
 *    1、指定位置与类型赋值
 *    2、强制使用 std::string("字面量")
 */
void TEST_bool_string()
{
    std::variant<bool, std::string> var;
    var = "hi";
    std::cout << "index: " << var.index() << std::endl;
    auto printHelper = [](const auto& val)
    {
        std::cout << "value: " << val << std::endl;
    };
    std::visit(printHelper, var);

    var.emplace<std::string>("emplace type");
    std::visit(printHelper, var);
    var.emplace<1>("emplace index");
    std::visit(printHelper, var);
    var = std::string("hello");
    std::visit(printHelper, var);
    using namespace std::literals;
    var = "world"s;
    std::visit(printHelper, var);
}

int main()
{
    TEST_bool_string();

    return 0;
}
