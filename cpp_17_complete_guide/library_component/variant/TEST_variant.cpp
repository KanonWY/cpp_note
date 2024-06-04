#include <atomic>
#include <iostream>
#include <string>
#include <variant>

// std::variant


// 定义一个未初始化的 variant 的行为是什么？
// 使用第一个类型的默认构造函数和默认值

void TEST_define_uninit_variant()
{
    try
    {
        std::variant<std::string, int> a;
        std::cout << a.index() << std::endl;
        std::cout << std::get<std::string>(a) << std::endl;
        std::cout << std::get<0>(a) << std::endl;
        // std::cout << std::get<int>(a) << '\n';
        // std::cout << std::get<1>(a) << '\n';
    }
    catch (const std::bad_variant_access& e)
    {
        std::cout << "EXCEPTION: " << e.what() << '\n';
    }
}

// 定义一个variant 会调用构造函数吗

struct A
{
    A()
    {
        std::cout << "A()" << std::endl;
    }

    ~A()
    {
        std::cout << "~A()" << std::endl;
    }
};

struct B
{
    B()
    {
        std::cout << "B()" << std::endl;
    }

    ~B()
    {
        std::cout << "~B()" << std::endl;
    }
};

struct C
{
    C() = delete;

    C(int a)
    {
        std::cout << "C(int a)" << std::endl;
    }

    ~C()
    {
        std::cout << "~C()" << std::endl;
    }
};


/**
 * 如果 element 没有默认构造函数会如何？
 */
void TEST_call_construct_variant()
{
    // 会调用第一个元素的构造函数
    std::variant<B, A> var1;
    std::variant<A, B> var2;

    // 可以通过编译，即使 C 没有构造函数
    std::variant<A, B, C> var3;
    // 无法通过编译，因为创建 var4 的时候需要调用 C 的构造函数
    // std::variant<C, A, B> var4;
}

/**
 * 如果 element 都没有默认构造函数会如何？将会无法构建
 *
 * 库提供了 std::monostate
 */

void TEST_no_default_construct()
{
    std::variant<std::monostate, C> var;
    std::cout << "index = " << var.index() << std::endl;

    if (std::holds_alternative<std::monostate>(var))
    {
        std::cout << "has monostate" << std::endl;
    }

    if (std::get_if<0>(&var))
    {
        std::cout << "get_if<index> monostate" << std::endl;
    }
    if (std::get_if<std::monostate>(&var))
    {
        std::cout << "get_if<Type> monostate" << std::endl;
    }
}

// variant 是否可继承?

class Derived : public std::variant<int, std::string>
{
};

/**
 * @brief 测试 variant 的可继承性
 */
void TEST_variant_dervived()
{
    Derived d{ "hellowero" };
    std::cout << d.index() << std::endl;
    std::cout << std::get<1>(d) << std::endl;
    std::cout << std::get<std::string>(d) << std::endl;
    d.emplace<0>(100);
    std::cout << std::get<0>(d) << std::endl;
}


int main()
{
    TEST_variant_dervived();
    return 0;
}
