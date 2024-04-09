#include <iostream>
#include <string>
#include <variant>

// 测试 variant 的访问器

struct MyVisitor
{
    void operator()(int i) const
    {
        std::cout << "int: " << i << std::endl;
    }

    void operator()(std::string s) const
    {
        std::cout << "string: " << s << std::endl;
    }

    void operator()(double d) const
    {
        std::cout << "double: " << d << std::endl;
    }
};

void TEST_visitor_easy()
{
    std::variant<int, std::string, double> var(42);
    std::visit(MyVisitor(), var);
    var = "hello";
    std::visit(MyVisitor(), var);
    var = 32.3;
    std::visit(MyVisitor(), var);
}

// use visitor to modify value of variant
struct Twice
{
    void operator()(double& d) const
    {
        d *= 2;
    }

    void operator()(int& i) const
    {
        i *= 2;
    }

    void operator()(std::string& s) const
    {
        s += s;
    }
};

void TEST_visitor_twice()
{
    std::variant<int, std::string, double> var("xxx");
    std::visit(Twice(), var);
    if (auto p = std::get_if<std::string>(&var); p)
    {
        std::cout << *p << std::endl;
    }
}

/**
 * @brief USE Generic as a visitor
 */
void TEST_use_generic_as_visitor()
{
    auto printVariant = [](const auto& value)
    {
        std::cout << "value = " << value << std::endl;
    };
    std::variant<int, std::string, double> var("hello ??");
    std::visit(printVariant, var);
    var = 22;
    std::visit(printVariant, var);
    var = 34.55;
    std::visit(printVariant, var);
}


/**
 * @brief 自定义类
 */
class MyCluster
{
public:
    template <typename T>
    auto operator()(const T& val) const
    {
        std::cout << val << std::endl;
    }
};


/**
 * @brief USE Generic as visitor_2
 */
void TEST_use_generic_as_visitor2()
{
    std::variant<int, std::string, double> var("xxx");
    std::visit(MyVisitor(), var);
    var = 22;
    std::visit(MyVisitor(), var);
    var = 34.55;
    std::visit(MyVisitor(), var);
}

/**
 * @brief 使用 lambda 作为 访问器
 */
void TEST_lambda_as_visitor()
{
    std::variant<int, std::string, double> var("yyyy");
    std::visit(
        [](auto& val)
        {
            val = val + val;
        },
        var);
}

/**
 * @brief  Use Overloaded lambda as Visitors
 *
 */
template <typename... Ts>
struct overload : Ts...
{
    using Ts::operator()...;
};

template <typename... Ts>
overload(Ts...) -> overload<Ts...>;

void TEST_overload()
{
    std::variant<int, std::string> var(43);
    std::visit(
        overload{
            [](int i)
            {
                std::cout << "int: " << i << std::endl;
            },
            [](const std::string& s)
            {
                std::cout << "string: " << s << std::endl;
            },
        },
        var);
    var = "KKKKOOOO";
    std::visit(
        overload{
            [](int i)
            {
                std::cout << "int: " << i << std::endl;
            },
            [](const std::string& s)
            {
                std::cout << "string: " << s << std::endl;
            },
        },
        var);
}

void TEST_overload_2()
{
    auto twice = overload{
        [](std::string& s)
        {
            s += s;
        },
        [](int& x)
        {
            x *= 2;
        },
    };

    std::variant<int, std::string> var("xxx");
    std::visit(twice, var);
    std::visit(twice, var);
    if (auto p = std::get_if<std::string>(&var); p)
    {
        std::cout << *p << std::endl;
    }
}

int main()
{
    TEST_overload_2();
    return 0;
}
