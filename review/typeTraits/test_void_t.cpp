#include <iostream>
#include <type_traits>

namespace kanon {

template <class _Tp, _Tp __v>
struct integral_constant
{
    static constexpr const _Tp value = __v;
    typedef _Tp value_type;
    typedef integral_constant type;
    constexpr operator value_type() const noexcept { return value; }
#if _LIBCPP_STD_VER > 11
    constexpr value_type operator()() const noexcept { return value; }
#endif
};

template <bool __b>
using bool_constant = integral_constant<bool, __b>;

typedef bool_constant<true> true_type;
typedef bool_constant<false> false_type;

// void_t
template <class...>
using void_t = void;

// condition impliment
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

} // namespace kanon

struct NoInnerType
{
    int m_int;
};

struct HaveInnerType
{
    using type = unsigned int;
    void myFunc() {}
};

template <typename T, typename U = kanon::void_t<>>
struct HaveTypeMem : kanon::false_type
{
};

// 判断是否存在某个类型
//  template <typename T>
//  struct HaveTypeMem<T, kanon::void_t<typename T::type>> : kanon::true_type
//  {
//  };

// 判断是否存在某一个成员变量
// template <typename T>
// struct HaveTypeMem<T, kanon::void_t<decltype(T::m_int)>> : kanon::true_type
// {
// };

// 判断类中是否存在某个成员函数
template <typename T>
struct HaveTypeMem<T, kanon::void_t<decltype(std::declval<T>().myFunc())>>
    : kanon::true_type
{
};

int main()
{
    std::cout << HaveTypeMem<NoInnerType>::value << std::endl;
    std::cout << HaveTypeMem<HaveInnerType>::value << std::endl;
    return 0;
}