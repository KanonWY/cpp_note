#include <concepts>

// 约束变量模板与模板别名
namespace demo1 {
template <typename T>
inline constexpr T PI = T(3.14);

template <std::floating_point T>
inline constexpr T PI2 = T(3.333);

} // namespace demo1

int main() { return 0; }