#include "KaSmartPointer.h"
#include <iostream>

int main() {
    std::cout << sizeof(KaShared_ptr<int>) << std::endl;
    std::cout << "end" << std::endl;
    return 0;
}