#include <concepts>

namespace demo1 {

template <std::integral T>
struct wrapper
{
    T value;
};

void TEST()
{
    wrapper<int> a{2};
    //    wrapper<double> b{33};
}

} // namespace demo1

namespace demo2 {

}

int main() { return 0; }