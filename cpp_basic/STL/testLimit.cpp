#include <iostream>
#include <limits>

template <typename T> void showLimit() {
    std::cout << "min = " << std::numeric_limits<T>::min() << std::endl;
    std::cout << "max = " << std::numeric_limits<T>::max() << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "short: " << std::endl;
    showLimit<short>();
    std::cout << "unsigned short: " << std::endl;
    showLimit<unsigned short>();

    std::cout << "int: " << std::endl;
    showLimit<int>();
    std::cout << "unsigned int: " << std::endl;
    showLimit<unsigned int>();

    std::cout << "long: " << std::endl;
    showLimit<long>();

    std::cout << "unsigned long: " << std::endl;
    showLimit<unsigned long>();

    std::cout << "long int: " << std::endl;
    showLimit<long int>();
    std::cout << "long long:" << std::endl;
    showLimit<long long>();
    std::cout << "long long int:" << std::endl;
    showLimit<long long int>();

    std::cout << "float: " << std::endl;
    showLimit<float>();

    std::cout << "doble: " << std::endl;
    showLimit<double>();

    std::cout << "long double: " << std::endl;
    showLimit<long double>();

    return 0;
}