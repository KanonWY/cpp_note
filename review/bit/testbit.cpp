#include "BitBasic.h"
#include <iostream>
#include <vector>

int main() {
    auto res = GetTwoNumber({1, 1, 2, 2, 10, 10,  12,  12, 88,   88,
                             5, 6, 5, 6, 7,  200, 200, 7,  3999, 875});
    int X = res[0];
    int Y = res[1];
    std::cout << "X = " << X << std::endl;
    std::cout << "Y = " << Y << std::endl;
}