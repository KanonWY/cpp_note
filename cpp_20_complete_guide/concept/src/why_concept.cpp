#include <concepts>

/**
 * @brief 加法的算法
 */
namespace demo1 {
template <typename T>
T add(T a, T b)
{
    return a + b;
}
} // namespace demo1

/**
 * @brief 使用 std::enable_if 来约束 T 的类型，SNAF
 * @note 缺陷：报错不友好，需要对 SNAF 有一定的了解
 *
 */
namespace demo2 {
template <typename T,
          typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
T add(T const a, T const b)
{
    return a + b;
}
} // namespace demo2

/**
 * @brief 使用静态断言约束
 *
 * @note 从函数签名无法了解该函数的用法
 */
namespace demo3 {
template <typename T>
T add(T const a, T const b)
{
    static_assert(std::is_arithmetic_v<T>, "Arithmetic type required!");
    return a + b;
}
} // namespace demo3

/**
 * @brief 混合静态断言和 SNAF
 */
namespace demo4 {
template <typename T,
          typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
T add(T const a, T const b)
{
    static_assert(std::is_arithmetic_v<T>, "Arithmetic type required!");
    return a + b;
}
} // namespace demo4

/**
 * @brief 使用 requires 子句来约束类型模版参数的类型为算数型
 */
namespace demo5 {
template <typename T>
    requires std::is_arithmetic_v<T>
T add(T const a, T const b)
{
    return a + b;
}

} // namespace demo5

/**
 * @brief requires 子句后置约束类型模版参数的类型
 */
namespace demo6 {

template <typename T>
T add(T const a, T const b)
    requires std::is_arithmetic_v<T>
{
    return a + b;
}

} // namespace demo6

/// 什么是约束？
///  编译期间求值为 true 或者 false 的谓词。
///  约束是在使用他们的地方定义的匿名谓词。

/// 大部分类型特征都可以表示为约束，是否是xxx类型，类型是否相同，是否满足什么条件等等
/// 命名的约束-> 概念

namespace demo7 {

/**
 * @brief 算法类型的概念
 * @tparam T
 */
template <typename T>
concept arithmetic = std::is_arithmetic_v<T>;

template <arithmetic T>
T add(T const a, T const b)
{
    return a + b;
}

template <arithmetic T>
T mul(T const a, T const b)
{
    return a - b;
}

} // namespace demo7

namespace demo8 {

/**
 * @brief 使用 requires 表达式定义概念（简单需求）
 * @tparam T
 */
template <typename T>
concept arithmetic = requires { std::is_arithmetic_v<T>; };

// requires 表达式可以表示类型的需求：
// - 简单需求
// - 类型需求
// - 复合需求
// - 嵌套需求


} // namespace demo8

int main() { return 0; }