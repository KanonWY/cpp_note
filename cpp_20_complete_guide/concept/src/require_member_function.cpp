#include <concepts>
#include <ranges>
#include <iostream>
#include <vector>

// require 子句可以是成员函数声明的一部分
//

template <typename T>
class ValOrColl
{
    T value;

public:
    ValOrColl(const T &val) : value(val) {}

    ValOrColl(T &&val) : value(std::move(val)) {}

    void print() const { std::cout << value << '\n'; }

    void print() const
        requires std::ranges::range<T>
    {
        for (const auto &elem : value)
        {
            std::cout << elem << ' ';
        }
        std::cout << '\n';
    }
};

int main()
{
    ValOrColl ol = 42;
    ol.print();

    ValOrColl o2 = std::vector{1, 2, 3, 5};
    o2.print();

    return 0;
}
