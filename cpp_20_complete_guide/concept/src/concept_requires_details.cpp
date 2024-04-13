#include <concepts>
#include <iostream>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////
// requires 表达式可能是复杂表达式
// 语法：
// 不允许默认参数
// requires (param-list) {
//   requirement - seq
// }
//
//  requirement - seq 以分号分割
//
//  - 简单需求
//  - 类型需求
//  - 复合需求
//  - 嵌套需求
//

//////////
/**
 * @brief 辅助函数，用于打印 bool 值
 * @tparam T
 * @param a
 */
template <typename T>
void boolOut(T a)
{
    std::cout << std::boolalpha << a << std::endl;
}

/////////
namespace demo1 {

// 简答需求：
// 不求值，只检查正确性的表达式。

template <typename T>
concept arithmetic = requires { std::is_arithmetic_v<T>; };

template <typename T>
concept addable = requires(T a, T b) { a + b; };

/**
 * @brief 日志器的概念
 * @note 确保 T 有三个成员函数，分别为 error ，warning ，info可以接受 const
 * char* 参数,或者可以从 const char* 构造出的其他类型参数
 * @tparam T
 */
template <typename T>
concept logger = requires(T t) {
    t.error("xxx");
    t.warning("yyy");
    t.info("zzz");
    t.fatal("zzz");
};

struct OK
{
    OK(const char* s) {}
};

struct OP
{
    OP(const char* s, bool = true) {}
};

struct log1

{
    void error(const std::string& s) { std::cout << s << std::endl; }

    void warning(const char* s) { std::cout << s << std::endl; }

    void info(OK l) { std::cout << "s" << std::endl; }

    void fatal(OP p) { std::cout << ">>>" << std::endl; }
};

struct loginterfacer
{
};

void TEST()
{
    std::cout << std::boolalpha << logger<log1> << std::endl;
    std::cout << std::boolalpha << logger<loginterfacer> << std::endl;
}

} // namespace demo1

namespace demo2 {

// 类型需求: 通过关键字 typename 后接类型名引入。
//           类型名必须有效，需求才为真

template <typename T>
concept KVP = requires {
    typename T::key_type;
    typename T::value_type;
};

template <typename T, typename V>
struct key_value_pair
{
    using key_type = T;
    using value_type = V;

    key_type key;
    value_type value;
};

void TEST()
{
    boolOut(KVP<key_value_pair<int, int>>);
    boolOut(KVP<std::pair<int, int>>);
}

} // namespace demo2

namespace demo3 {
// 复合需求
// 不仅可以验证表示式是否有效，还可以验证表达式的某些属性
//
// { expression } noexcept -> type_constraint;
//   表达式      禁止抛异常      类型线程
// 1、替换表达式中的模板参数
// 2、指定了 noexcept， 则表达式不能抛异常，如果抛出异常，需求为 false
// 3、存在类型约束模版参数会被替换为模板约束
//    decltype(expression) 必须满足 type_constraint 的条件，否则为fasle

/**
 * @brief 该函数不抛出异常
 * @tparam T
 */
template <typename T>
void f(T) noexcept
{
    std::cout << "f(T)" << std::endl;
}

/**
 * @brief 该函数可能抛出异常
 * @tparam T
 */
template <typename T>
void g(T)
{
    std::cout << "g(T)" << std::endl;
}

template <typename F, typename... T>
concept NOThrowing = requires(F&& func, T... t) {
    { func(t...) } noexcept;
};

template <typename F, typename... T>
    requires NOThrowing<F, T...>
void invoke(F&& func, T... t)
{
    func(t...);
}

void TEST1()
{
    demo3::invoke(demo3::f<int>, 42);
    //    demo3::invoke(demo3::g<int>, 42);
}

/**
 * @brief 定时器的概念，要求 timer 对象有 t.start 函数并且返回 void 类型
 * @tparam T
 */
template <typename T>
concept timer = requires(T t) {
    { t.start() } -> std::same_as<void>;
    { t.stop() } -> std::convertible_to<long long>;
};

struct TimerA
{
    void start() {}
    long long stop() { return 0; }
};

struct TimerB
{
    void start() {}
    int stop() { return 0; }
};

struct TimerC
{
    void start() {}
    void stop() {}
};

void TEST()
{
    boolOut(timer<TimerA>);
    boolOut(timer<TimerB>);
    boolOut(timer<TimerC>);
}

} // namespace demo3

namespace demo4 {
// 嵌套需求，需要 requires 引入
// requires contraint-express

// demo: 定义一个函数
// 1. 有多个参数
// 2. 所有参数类型相同
// 3. 表达式 arg1+arg2...

template <typename T, typename... Ts>
inline constexpr bool are_same_v = std::conjunction_v<std::is_same<T, Ts>...>;

template <typename... T>
concept HomegenousRange = requires(T... t) {
    (... + t);
    requires are_same_v<T...>;
    requires sizeof...(T) > 1;
};

// 组合约束

// &&

template <typename T>
concept Integral = std::is_integral_v<T>;

template <typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

template <typename T>
concept Signed = std::is_signed_v<T>;

// 合取
template <typename T>
concept SigendInt = Integral<T> && Signed<T>;

// 析取 ||
template <typename T>
concept Number = Integral<T> || FloatingPoint<T>;

////////////// 合取合析取无法约束模板参数包

// template <typename... T>
//     requires (std::is_integral_v<T> && ...
// auto add(T... args)
//{
//     return (args + ...);
// }

template <typename... T>
    requires(std::is_integral_v<T> && ...)
auto add(T... args)
{
    return (args + ...);
}

} // namespace demo4

///////////////////////////////////////////////////////////////////////////////
// 约束规则：编译器会选择约束更强的重载作为最佳匹配
// 加入了约束之后的函数重载规则是否发生变化
// 函数重载规则：

namespace demo5 {
/**
 * @brief 普通函数
 * @param a
 * @param b
 * @return
 */
int add(int a, int b) { return a + b; }

/**
 * @brief 模板函数
 * @tparam T
 * @param a
 * @param b
 * @return
 */
template <typename T>
T add(T a, T b)
{
    return a + b;
}

void TEST_a()
{
    add(1.0, 2.0); // 函数模板
    add(1, 2);     // 普通函数
}

} // namespace demo5

namespace demo6 {

// 两个重载都是模板，一个收约束？

template <typename T>
T add(T a, T b)
{
    std::cout << "no requires" << std::endl;
    return a + b;
}

template <typename T>
    requires std::is_integral_v<T>
T add(T a, T b)
{
    std::cout << "requires" << std::endl;
    return a + b;
};

void TEST()
{
    add(1.0, 2.0); // 函数模板
    add(1, 2);     // 普通函数
}

} // namespace demo6

namespace demo7 {
template <typename T>
    requires(sizeof(T) == 4)
T add(T a, T b)
{
    std::cout << "required sizeof()" << std::endl;
    return a + b;
}

template <typename T>
    requires std::is_integral_v<T>
T add(T a, T b)
{
    return a + b;
}

void TEST()
{
    add(short(1), short(2));
    //    add(1, 2);
}

} // namespace demo7

namespace demo8 {

// 使用约束之后，这个两个还是区分不开。因为
template <typename T>
    requires std::is_integral_v<T>
T add(T a, T b)
{
    return a + b;
}

template <typename T>
    requires std::is_integral_v<T> && (sizeof(T) == 4)
T add(T a, T b)
{
    return a + b;
}

void TEST()
{
    add(short(1), short(2));
    //        add(1, 2);
}

} // namespace demo8

namespace demo9 {

template <typename T>
concept Integral = std::is_integral_v<T>;

template <typename T>
    requires Integral<T>
T add(T a, T b)
{
    return a + b;
}

template <typename T>
    requires Integral<T> && (sizeof(T) == 4)
T add(T a, T b)
{
    return a + b;
}

void TEST()
{
    add(short(1), short(2));
    add(1, 2);
}

} // namespace demo9

namespace demo10 {

/**
 * @brief 整型概念
 * @tparam T
 */
template <typename T>
concept Integral = std::is_integral_v<T>;

template <Integral T>
T add(T a, T b)
{
    return a + b;
}

template <Integral T>
    requires(sizeof(T) == 4)
T add(T a, T b)
{
    return a + b;
}

void TEST()
{
    add(short(1), short(2));
    add(1, 2);
}

} // namespace demo10

template <typename T>
    requires std::is_pointer_v<T>
void printAddress(T ptr)
{
    printf("%p\n", ptr);
}


int main()
{
    std::string_view a;
    demo9::TEST();
    printAddress(a.data());

    return 0;
}