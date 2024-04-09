#include <exception>
#include <iostream>
#include <variant>

struct S
{
    operator int()
    {
        throw "EXCEPTION";
    }
};

void TEST_value_exception()
{
    std::variant<std::string, int> var{ "12.2" };
    try
    {
        var.emplace<1>(S{});
    }
    catch (...)
    {
        if (!var.valueless_by_exception())
        {
            std::cout << "ok" << std::endl;
        }
        else
        {
            std::cout << "NO" << std::endl;
        }

        if (var.index() == std::variant_npos)
        {
            std::cout << "exception OK" << std::endl;
        }
    }
}

int main()
{
    TEST_value_exception();
    return 0;
}
