#include "KaAlloc.h"
#include <unistd.h>
#include <vector>

class Test {
  public:
    Test() = default;

    ~Test() = default;

    Test(int x) : a(x) { std::cout << "Test(int x) " << std::endl; }

    Test(const Test &rhs) {
        std::cout << "copy" << std::endl;
        this->a = rhs.a;
    }

    Test &operator=(const Test &rhs) {
        std::cout << "=copy" << std::endl;
        this->a = rhs.a;
        return *this;
    }

    Test(Test &&rhs) noexcept {
        std::cout << "move" << std::endl;
        this->a = rhs.a;
    }

    Test &operator=(Test &&rhs) noexcept {

        std::cout << "=move" << std::endl;
        this->a = rhs.a;
        return *this;
    }

  private:
    int a;
};

int main() {

    std::vector<Test, MyAllocator<Test>> m_alloc;
    m_alloc.reserve(50);
    for (int i = 0; i < 10; ++i) {
        m_alloc.push_back(i);
    }
    return 0;
}