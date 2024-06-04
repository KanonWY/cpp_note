#include "foo.h"
#include <iostream>

void inc(const int& i) { std::cout << "in inc [ " << i << " ]" << std::endl; }

int main()
{
    std::cout << D::n << std::endl;
    inc(D::n); // error: 此处或报错 */
    return 0;
}
