#include <type_traits>
#include <iostream>

/**
 * @brief   实现简单的conditional
 *
 */
namespace cpp {

template <class T, class U>
struct is_same
{
    static constexpr bool value = false;
};

template <class T>
struct is_same<T, T>
{
    static constexpr bool value = true;
};

template <class T>
struct integral_const
{
    using type = T;
};

template <class T>
struct remove_reference : integral_const<T>
{
};

template <class T>
struct remove_reference<T &> : integral_const<T>
{
};

template <class T>
struct remove_const : integral_const<T>
{
};

template <class T>
struct remove_const<const T> : integral_const<T>
{
};

// 实现condition
template <bool b, class T, class F>
struct conditional
{
    using type = T;
};

template <class T, class F>
struct conditional<false, T, F>
{
    using type = F;
};

template <bool b, class T, class F>
using conditional_t = typename conditional<b, T, F>::type;

struct B
{
    void print() { std::cout << "B\n"; }
};

struct C
{
    void print() { std::cout << "C\n"; }
};

template <bool b>
struct A : conditional_t<b, B, C>
{
};

template <bool b>
struct D : conditional<b, B, C>::type
{
};

} // namespace cpp
