#include <iostream>
#include <string>
#include <utility>

class Customer
{
private:
    std::string name;
    std::string address;
    long long value;

public:
    Customer(std::string f, std::string g)
        : name(std::move(f)), address(std::move(g))
    {
    }

public:
    std::string getName() const { return name; }

    std::string getAddress() const { return address; }

    long long getValue() const { return value; }
};

int main() { return 0; }
