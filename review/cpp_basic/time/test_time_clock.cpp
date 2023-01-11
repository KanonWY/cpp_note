#include <iostream>
#include <chrono>


int main()
{
    std::chrono::minutes t1(30);  //1800 s 
    std::chrono::seconds t2(60);  //60 s
    auto dt = t1 - t2;            //1740 s
    std::cout << dt.count() << " secons" << std::endl;
    return 0;
}
