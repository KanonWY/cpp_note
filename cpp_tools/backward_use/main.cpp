#include <thread>
#include <backward.h>
#define BACKWARD_HAS_DW 1
namespace backward {
backward::SignalHandling sh;
}

#include <sstream>

void test()
{
    backward::StackTrace st;
    st.load_here();

    backward::Printer p;
    p.color_mode = backward::ColorMode::always;
    // 存储下来
    std::ostringstream oss;
    p.print(st, oss);
    std::cout << "----------------" << std::endl;
    std::cout << oss.str() << std::endl;
    std::cout << "----------------" << std::endl;
}

struct Test
{
    static void he() { test(); }
};

template <typename T>
void foo(T t)
{
    Test::he();
}

int main()
{
    foo(2);
    std::thread t1([]() {
        backward::StackTrace st;
        st.load_here();

        backward::Printer p;
        p.color_mode = backward::ColorMode::always;
        p.print(st, std::cout);
        std::cout << std::this_thread::get_id() << std::endl;
    });

    t1.join();
    return 0;
}
