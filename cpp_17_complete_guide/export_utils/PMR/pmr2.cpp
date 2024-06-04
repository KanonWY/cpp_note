#include <cstddef>
#include <iostream>
#include <memory_resource>
#include <array>
#include <string>
#include <vector>

int main()
{
    std::array<std::byte, 200'000> buf;

    std::pmr::monotonic_buffer_resource pool(buf.data(), buf.size());
    std::pmr::vector<std::pmr::string> coll{&pool};

    for (int i = 0; i < 1000; ++i)
    {
        coll.emplace_back("JUST a NON-SSO STRING");
    }

    return 0;
}
