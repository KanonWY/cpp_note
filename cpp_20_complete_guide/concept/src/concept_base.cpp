#include <concepts>
#include <iostream>

namespace eon {

/**
 * @brief 使用自定义的概念, T 对象是一个指针
 */
template <typename T>
concept IsPointer = requires(T p) {
    *p;           ///<<< operator* 操作符对 T 类型有效
    p == nullptr; ///<<< 能够与 nullptr 进行比较

    { p < p } -> std::convertible_to<bool>;
};

auto maxValue(auto a, auto b) { return b < a ? a : b; }

auto maxValue(IsPointer auto a, IsPointer auto b)
    requires std::totally_ordered_with<decltype(*a), decltype(*b)>
{
    std::cout << "requires" << std::endl;
    return maxValue(*a, *b);
}
} // namespace eon

void TEST_max()
{
    int x = 43;
    int y = 33;

    std::cout << eon::maxValue(x, y) << std::endl;
    std::cout << "------------------------------------------\n";
    std::cout << eon::maxValue(&x, &y) << std::endl;
}

int main()
{
    TEST_max();

    return 0;
}
