#include <concepts>
#include <iostream>
#include <vector>

/**
 * @note 可以使用 requires 来定义概念
 *
 * - 简单需求
 * - 类型需求
 * - 复合需求
 * - 嵌套需求
 */

namespace demo1 {

/**
 * @note 需要定一个模板，只接收容器类型作为参数
 *
 * 在 C++20 之前使用 SFINAE 或者静态断言实现。
 *
 * - 容器需要有成员，value_type, size_type, allocator_type iterator
 * const_iterator
 * - 成员函数 size
 * - 成员函数begin/end，cbegin/cend
 */

////////////////////////////////////       SFINAE 的模版类型限定
///////////////////////////////
template <typename T, typename U = void>
struct is_container : std::false_type
{
};

template <typename T>
struct is_container<
    T,
    std::void_t<typename T::value_type, typename T::size_type,
                typename T::allocator_type, typename T::iterator,
                typename T::const_iterator, decltype(std::declval<T>().size()),
                decltype(std::declval<T>().begin()),
                decltype(std::declval<T>().end()),
                decltype(std::declval<T>().cbegin()),
                decltype(std::declval<T>().cend())>> : std::true_type
{
};

/**
 * @brief 类型 T 是否是容器类型
 * @tparam T
 * @tparam U
 */
template <typename T, typename U = void>
constexpr inline bool is_container_v = is_container<T, U>::value;

struct foo
{
    int a;
    int b;
};

void TEST()
{
    std::cout << std::boolalpha
              << demo1::is_container_v<demo1::foo> << std::endl;

    std::cout << std::boolalpha
              << demo1::is_container_v<std::vector<demo1::foo>> << std::endl;
}

} // namespace demo1

namespace demo2 {

template <typename T>
concept container = requires(T t) {
    typename T::value_type;
    typename T::size_type;
    typename T::allocator_type;
    typename T::iterator;
    typename T::const_iterator;
    t.size();
    t.begin();
    t.end();
    t.cbegin();
    t.cend();
};

struct foo
{
};

void TEST()
{
    std::cout << std::boolalpha << container<foo> << std::endl;
    std::cout << std::boolalpha << container<std::vector<foo>> << std::endl;
}

} // namespace demo2

int main()
{
    demo1::TEST();
    std::cout << "----------------------" << std::endl;
    demo2::TEST();
    return 0;
}