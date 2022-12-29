#include <type_traits>
#include "basic_trailts.h"

void test_condtional()
{
    using T1 = cpp::conditional<true, int, double>::type;
    using T2 = cpp::conditional<false, int, double>::type;
    cpp::A<false> a;
    a.print();

    cpp::A<true> b;
    b.print();

    cpp::D<false> d;
    d.print();
}

int main()
{
    test_condtional();
    return 0;
}