#include <concepts>
#include <string_view>
#include <memory>

// 约束非模板成员函数
namespace demo1 {

template <typename T>
struct wrapper
{
    T value;

    bool operator==(std::string_view str)
        requires std::is_convertible_v<T, std::string_view>
    {
        return value == str;
    }
};
} // namespace demo1

namespace demo2 {

template <typename T>

struct wrapper
{
    T value;
    wrapper(T const& v) : value(v) {}
};

void TEST()
{
    wrapper<int> a = 32;

    //    wrapper<std::unique_ptr<int>> p = std::make_unique<int>(42);
}

} // namespace demo2

namespace demo3 {

void handle(int v) {}


} // namespace demo3

int main() { return 0; }