#include "eon_match.h"
#include <string_view>
#include <iostream>

struct Shape
{
    virtual ~Shape() = default;
};
struct Circle : Shape
{
};
struct Square : Shape
{
};

auto getClassName(Shape const &s)
{
    using namespace matchit;
    return match(s)(pattern | as<Circle>(_) = "Circle",
                    pattern | as<Square>(_) = "Square");
}


constexpr int32_t gcd(int32_t a, int32_t b)
{
    using namespace matchit;
    return match(a, b)(
        pattern | ds(_, 0) = [&] { return a >= 0 ? a : -a; },
        pattern | _        = [&] { return gcd(b, a%b); }
    );
}

int main()
{
    Circle c{};
    auto res = getClassName(c);
    std::cout << res << std::endl;

    Square q{};
    auto res2 = getClassName(q);
    std::cout << res2 << std::endl;


    using namespace matchit;

    int x = 20;
    auto m = match(x) (
        pattern | or_(1,2,3) = true,
        pattern | _ = false
        );
    std::cout << std::boolalpha << m << std::endl;


    int a = 2;
    int b = 3;

    auto k = gcd(a,b);

    std::cout << k << std::endl;

    return 0;
}