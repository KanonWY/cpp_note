#include "BitBasic.h"
#include <iostream>
#include <vector>

int testTwoNumber() {
    auto res = GetTwoNumber({1, 1, 2, 2, 10, 10,  12,  12, 88,   88,
                             5, 6, 5, 6, 7,  200, 200, 7,  3999, 875});
    int X = res[0];
    int Y = res[1];
    std::cout << "X = " << X << std::endl;
    std::cout << "Y = " << Y << std::endl;
    return 0;
}

int testBinaryBit() {
    short a = 0;
    a = 0b1;
    std::cout << "a = " << a << std::endl;
    a = 0B11;
    std::cout << "a = " << a << std::endl;
    a = 0B111;
    std::cout << "a = " << a << std::endl;
    a = 0B1111;
    std::cout << "a = " << a << std::endl;
    a = 0B11111;
    std::cout << "a = " << a << std::endl;
    a = 0B111111;
    std::cout << "a = " << a << std::endl;
    a = 0B1111111;
    std::cout << "a = " << a << std::endl;
    a = 0B11111111;
    std::cout << "a = " << a << std::endl;

    return 0;
}

void testHexBit() {
    unsigned int d = 0xfff;
    std::cout << "d = " << d << std::endl;
    d = 0xffff;
    std::cout << "d = " << d << std::endl;
    d = 0xfffff;
    std::cout << "d = " << d << std::endl;
    d = 0xffffff;
    std::cout << "d = " << d << std::endl;
    d = 0xffffffff; // for int it's -1
    std::cout << "d = " << d << std::endl;
}

int main() { return 0; }
