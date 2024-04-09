#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::vector<std::string> coll;

    for (int i = 0; i < 1000; ++i)
    {
        coll.emplace_back("JUST NO SSO string");
    }
    return 0;
}
