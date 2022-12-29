#include <memory>
#include <tuple>
#include <type_traits>

// 获取可变参数的个数
template <typename... Args>
int count(const Args&... args)
{
    return sizeof...(args);
}

template <typename... Args>
auto make_tuple(const Args&... args)
{
    return std::make_tuple(args...);
}

int main() { return 0; }