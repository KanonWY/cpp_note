#include <iostream>

template <typename T>
auto m_add(const T& a, const T& b) -> decltype(a + b)
{
    return a + b;
}

template <typename T>
auto add(const T& a, const T& b)
{
    return a + b;
}

int main()
{
    add("x", "y");
    m_add("a", "b");

    return 0;
}
