#include <algorithm>
#include <cstddef>
#include <memory>
#include <iostream>


class USP {
public:
    ~USP() {
        std::cout << "~USP()" << std::endl;
    }
    void echo() {
        std::cout << "USP echo() " << std::endl;
    }
};

std::unique_ptr<int> getSP(int x) {
    auto sp = std::unique_ptr<int>(new int(x));
    return sp;
}


void example_make_nullptr() {
    auto sp = std::make_unique<USP>();
    sp      = nullptr;
    std::cout << "end" << std::endl;
}


void example_release() {
    auto sp  = std::make_unique<USP>();
    auto sp2 = sp.release();
    if (sp) {
        std::cout << "yes" << std::endl;
    }
    else {
        std::cout << "removed" << std::endl;
    }
    sp2->echo();
}


void example_move() {
    auto sp  = std::make_unique<USP>();
    auto sp2 = std::move(sp);
    if (sp) {
        std::cout << "sp" << std::endl;
    }
    if (sp2) {
        std::cout << "sp2" << std::endl;
    }
}


std::unique_ptr<USP> getUniquePtr() {
    auto sp = std::make_unique<USP>();
    return sp;
}


void PassUniquePtr(const std::unique_ptr<USP>& rhs) {
    if (rhs) {
        rhs->echo();
    }
    else {
        std::cout << "const" << std::endl;
    }
    // rhs.reset();
}


void PassUniquePtr(std::unique_ptr<USP>&& rhs) {
    if (rhs) {
        rhs->echo();
        rhs.reset();
    }
}

// pass by value
void test(std::unique_ptr<USP> rhs) {
    rhs->echo();
}


int main() {
    auto sp = std::make_unique<USP>();
    PassUniquePtr(std::move(sp));
    PassUniquePtr(sp);
    if (sp) {
        std::cout << "yes" << std::endl;
    }
    else {
        std::cout << "no" << std::endl;
    }
    auto sp2 = std::make_unique<USP>();

    test(std::move(sp2));
    if (sp2) {
        std::cout << "sp2 yes" << std::endl;
    }
    else {
        std::cout << "sp2 no" << std::endl;
    }
    return 0;
}