#include <iostream>
#include <string>

// cosnt lvalue ref
// everything
// (1) named object which can modify
// (2) cosnt named object
// (3) no_named temp object
// (4) std::move(object)
void foo(const std::string& rhs)
{
    std::cout << "void foo(const std::string& rhs)" << std::endl;
}

// lvalue ref
// named object which can modify
void foo(std::string& rhs)
{
    std::cout << "void foo(std::string& rhs)" << std::endl;
}

// rvalue ref
//(1)  no_name temp obj
//(2)  std::move(obj)
void foo(std::string&& rhs)
{
    std::cout << "void foo(std::string&& rhs)" << std::endl;
}

std::string GetRandomString() { return std::string("klpplp"); }

int main()
{
    std::string str1("hello move Semantics!");
    std::cout << str1.c_str() << std::endl;
    foo(str1);
    foo(GetRandomString());
    return 0;
}
