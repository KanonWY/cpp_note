#include <bitset>
#include <iostream>

int main() {
    std::bitset<32> m_bitset{0x01};

    std::cout << sizeof(m_bitset) << std::endl;

    std::cout << m_bitset.all() << std::endl;
    std::cout << m_bitset.any() << std::endl;
    std::cout << m_bitset.size() << std::endl;
}