#include <iostream>
#include <type_traits>

template<typename T>
void f(int T::*) {
    std::cout << "T is a class" << std::endl;
}

template<typename T>
void f(...) {
    std::cout << "T is not a class" << std::endl;
}

struct A {
};


template<typename T>
class is_class {
    template<typename C>
    static char test(int C::*);

    template<typename C>
    static int test(...);

public:
    static constexpr bool value = sizeof(test<T>(0)) == 1;
};


namespace implementation {
template<typename C>
static char test(int C::*);
template<typename C>
static int test(...);
} // namespace implementation

template<class T>
struct is_class2: std::integral_constant<bool, sizeof(implementation::test<T>(0)) == sizeof(char)> {};


int main() {
    std::cout << is_class<int>::value << std::endl;
    std::cout << is_class<A>::value << std::endl;

    return 0;
}