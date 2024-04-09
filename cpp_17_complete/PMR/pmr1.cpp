#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstdlib>
#include <memory_resource>

int main()
{
    std::array<std::byte, 200000> buf;
    std::pmr::monotonic_buffer_resource pool(buf.data(), buf.size());
    std::pmr::vector<std::string> coll{&pool};

    for (int i = 0; i < 1000; ++i)
    {
        coll.emplace_back("JUST a NON-SSO STRING");
    }

    return 0;
}
