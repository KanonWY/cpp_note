#include <type_traits>
#include <iostream>

int main()
{
    std::cout << "sizeof(meta) = " << sizeof(std::_MetaBase<true>) << std::endl;
    return 0;
}