#include <iostream>
#include <cstddef>
#include <type_traits>

namespace demo {

/**
 * @brief test for compare
 */
void TEST()
{
    std::byte a{0x11};

    if (a > std::byte{0x10})
    {
        std::cout << "a big than tmp" << std::endl;
    }
    else
    {
        std::cout << "a smaller than tmp" << std::endl;
    }
}

/**
 * @brief how to construct a std::byte
 */
void TEST2()
{
    std::byte a{0b1111'1111};
    std::byte b{0x22};
    /* std::byte c(0x22); */
    /* std::byte d = 0x22; */
    /* std::byte e = {0x22}; */
    /* std::byte f[] = {1, 2}; */
    std::byte g[] = {std::byte{0x02}, std::byte{0x01}};
    std::byte h;
    std::byte i{};
}

void TEST3()
{
    std::byte a{0x22};
    std::cout << std::to_integer<int>(a) << std::endl;

    std::cout << sizeof(a) << std::endl; // awlays1
}

} // namespace demo

int main()
{
    std::byte a{0x11};

    std::cout << std::boolalpha << std::is_integral_v<decltype(a)> << std::endl;
    std::cout << "sizeof(std::byte) = " << sizeof(std::byte) << std::endl;
    std::cout << std::to_integer<int>(a) << std::endl;

    return 0;
}
