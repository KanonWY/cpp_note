#include <iostream>
#include <string>
#include "boost/type_index.hpp"

/**
 * @brief   简单实现condtional
 *
 */

namespace kanon {

template <bool b, typename T, typename F>
struct conditional
{
    using type = T;
};

template <typename T, typename F>
struct conditional<false, T, F>
{
    using type = F;
};

} // namespace kanon

template <typename T>
void dumpPrettyType(T t)
{
    std::cout << "type T = "
              << boost::typeindex::type_id_with_cvr<decltype(t)>().pretty_name()
              << std::endl;
}

int main()
{
    kanon::conditional<true, std::string, double>::type a;
    kanon::conditional<false, std::string, double>::type b;
    dumpPrettyType(a);
    dumpPrettyType(b);

    return 0;
}