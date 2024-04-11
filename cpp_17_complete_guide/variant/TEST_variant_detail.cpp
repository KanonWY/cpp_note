#include <cmath>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>


/**
 * @brief construct api
 */
void TEST_construct_api()
{
    std::variant<int, int, std::string> v1;  // set first to 0, index = 0; will call first element's construct function!

    std::variant<long, int> v2{ 42 };
    std::cout << v2.index() << std::endl;  // print 1, 42 match int type

    // care about ambiguous
    // std::variant<long, long> v3{ 42 };
    // std::variant<int, float> v4{ 42.3 };
    std::variant<int, double> v5{ 42.3 };  // ????????
    // std::variant<int, long double> v6{ 42.3 };
    // std::variant<std::string, std::string_view> v7{ "hello" };
    std::variant<std::string, std::string_view, const char*> v8{ "hello" };
    std::cout << v8.index() << std::endl;  // print 2
}

struct A
{
    A(int x, int y)
        : m_a(x),
          m_b(y)
    {
    }
    int m_a;
    int m_b;
};

/**
 * @brief deal with pass more than one value to init
 */
void TEST_multi_value_init()
{
    // std::variant<A> v1{ 3, 4 }; // error
    // std::variant<A> v2{ { 3, 4 } }; error
    std::variant<A> v3{ std::in_place_type<A>, 3, 4 };
    std::variant<A> v4{ std::in_place_index<0>, 3, 4 };

    std::variant<int, int>  v5{ std::in_place_index<1>, 77 };
    std::variant<int, long> v6{ std::in_place_index<1>, 100 };
    std::cout << v6.index() << std::endl;

    auto sc = [](int x, int y)
    {
        return std::abs(x) < std::abs(y);
    };

    std::variant<std::vector<int>, std::set<int, decltype(sc)>> v7{ std::in_place_index<1>, { 4, 8, 7, -9, 10 }, sc };
    std::cout << v7.index() << std::endl;
}

/**
 * @brief 获取值
 */
void TEST_acess_value()
{
    std::variant<int, int, std::string> var;
    var = "xxx";
    std::cout << var.index() << std::endl;

    if (auto ip = std::get_if<2>(&var); ip)
    {
        std::cout << *ip << std::endl;
    }

    std::cout << std::get<2>(var) << std::endl;
    std::cout << std::get<std::string>(var) << std::endl;
}

/**
 * @brief change value
 */

void TEST_change_value()
{
    std::variant<int, int, std::string> var;
    // way1
    var = "hello";
    // way2
    var.emplace<1>(433);
    // way3
    std::get<0>(var) = 100;
    std::cout << var.index() << std::endl;
    std::get<1>(var) = 200;
    std::cout << var.index() << std::endl;
    std::get<std::string>(var) = "????";
    std::cout << var.index() << std::endl;

    if (auto p = std::get_if<std::string>(&var); p)
    {
        *p = "xxxx";
        std::cout << var.index() << std::endl;
    }
}

/**
 * @brief variant 变量之间可比吗？
 *
 */

void TEST_compare_op()
{
    std::variant<std::monostate, int, std::string> v1, v2{ "hello" }, v3{ 42 };

    std::variant<std::monostate, std::string, int> v4;
    // 不同的 variant 之间无法比较
    // std::cout << (v1 == v4) << std::endl;
    std::cout << std::boolalpha << (v1 == v2) << std::endl;

    // 实际值越靠后值越大?
    std::cout << std::boolalpha << (v1 < v2) << std::endl;
    std::cout << std::boolalpha << (v1 < v3) << std::endl;
    std::cout << std::boolalpha << (v2 < v3) << std::endl;
    std::cout << std::boolalpha << (v2 > v3) << std::endl;

    // 类型相同的时候开始比较实际的值的大小
    v2 = 41;
    std::cout << std::boolalpha << (v2 > v3) << std::endl;
}


struct exampleA
{
    exampleA() = default;
    exampleA(const exampleA&)
    {
        std::cout << "COPY construct" << std::endl;
    }
    exampleA(exampleA&&)
    {
        std::cout << "MOVE construct" << std::endl;
    }

    exampleA& operator=(const exampleA&)
    {
        std::cout << "COPY ASIG" << std::endl;
        return *this;
    }

    exampleA& operator=(exampleA&&)
    {
        std::cout << "MOVE ASIG" << std::endl;
        return *this;
    }
};

/**
 * @brief 测试 variant 是否支持移动语义
 */
void TEST_variant_move_semantics()
{
    std::variant<std::monostate, exampleA> var, var2;

    auto c = exampleA();
    var    = std::move(c);
    var2   = exampleA();
    auto b = exampleA();
    var2   = b;
}


int main()
{
    TEST_variant_move_semantics();
    return 0;
}
