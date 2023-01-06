#include <type_traits>
#include <iostream>
#include "boost/type_index.hpp"

/**
 * @brief std::declval的作用?
 *
 *  <1> 从类型转换的角度来讲，将任意一个类型转换成右值引用类型。
 *
 *
 *  <2> 从假想创建出某类型对象的角度来说，配合decltype，
 *      令在decltype表达式中，不必经过该类型的构造函数就能使用该类型的成员函数。
 *
 *  std::declval不能被调用，也不能创建任何对象。
 *      但std::declval能在不创建对象的情况下，
 *      达到创建了一个该类型对象的效果或者说可以假定创建出了一个该类型对象。
 *
 */

class A
{
public:
    A(int i) { std::cout << "A:A(int)" << std::endl; }

    double myfunc()
    {
        std::cout << "A::myfunc()" << std::endl;
        return 12.3;
    }
};

using YT = decltype(std::declval<A>());

using YF = decltype(std::declval<A>().myfunc());

template <typename T>
T& mydeclval() noexcept;

int main()
{
    std::cout << "YT name = "
              << boost::typeindex::type_id_with_cvr<YT>().pretty_name()
              << std::endl;

    std::cout << "YF name = "
              << boost::typeindex::type_id_with_cvr<YF>().pretty_name()
              << std::endl;

    std::cout << boost::typeindex::type_id_with_cvr<decltype(mydeclval<A>())>()
                     .pretty_name()
              << std::endl;
    std::cout << boost::typeindex::type_id_with_cvr<decltype(mydeclval<A&>())>()
                     .pretty_name()
              << std::endl;

    std::cout
        << boost::typeindex::type_id_with_cvr<decltype(mydeclval<A&&>())>()
               .pretty_name()
        << std::endl;
    return 0;
}
