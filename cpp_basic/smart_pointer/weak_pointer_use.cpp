#include <functional>
#include <memory>
#include "SourcePool.h"
#include "boost/type_index.hpp"

/**
 * @brief class A和class B循环引用，然后没有释放。
 *
 */
class A;
class B;

class A {
public:
    A() = default;
    A(std::shared_ptr<B> m_sp):
        b_sp(m_sp) {}

    std::shared_ptr<B> b_sp;

    ~A() {
        std::cout << "~A()" << std::endl;
    }
};

class B {
public:
    B() = default;
    B(std::shared_ptr<A> m_sp):
        a_sp(m_sp) {}

    std::shared_ptr<A> a_sp;

    ~B() {
        std::cout << "~B()" << std::endl;
    }
};


class A_w;
class B_w;

class A_w {
public:
    A_w() = default;
    A_w(std::shared_ptr<B_w> m_sp):
        b_sp(m_sp) {}

    std::shared_ptr<B_w> b_sp;

    ~A_w() {
        std::cout << "~A_w()" << std::endl;
    }
};

class B_w {
public:
    B_w() = default;
    B_w(std::shared_ptr<A_w> m_sp):
        a_sp(m_sp) {}

    std::weak_ptr<A_w> a_sp;

    ~B_w() {
        std::cout << "~B_w()" << std::endl;
    }
};


/**
 * @brief 测试循环引用，在输出中可以看到，最终两个智能指针的引用计数为1.
 *
 * @return int
 */
int test_cycle_ref() {
    auto sp_a  = std::make_shared<A>();
    auto sp_b  = std::make_shared<B>();
    sp_a->b_sp = sp_b;
    sp_b->a_sp = sp_a;
    std::cout << "sp_a->use_count() = " << sp_a.use_count() << std::endl;
    std::cout << "sp_b->use_count() = " << sp_b.use_count() << std::endl;
    return 0;
}

int test_cycle_ref_2() {
    auto sp_a  = std::make_shared<A_w>();
    auto sp_b  = std::make_shared<B_w>();
    sp_a->b_sp = sp_b;
    sp_b->a_sp = sp_a;
    std::cout << "sp_a->use_count() = " << sp_a.use_count() << std::endl;
    std::cout << "sp_b->use_count() = " << sp_b.use_count() << std::endl;
    return 0;
}


void test_weak_ptr_function() {
    auto                      sp = std::make_shared<SourcePool>();
    std::weak_ptr<SourcePool> sp2(sp);
}


class TestA {
public:
    std::string m_data;

    void echo() {
        std::cout << "data = " << m_data.c_str() << std::endl;
    }
};

void testFoo(TestA* rhs) {
    std::cout << "testFoo End......" << std::endl;
}


auto NewCallFunction = std::bind(testFoo, std::placeholders::_1);

int main() {

    auto foo = [](TestA* rhs) mutable -> void {
        std::cout << "foo End......" << std::endl;
    };

    std::unique_ptr<TestA, std::function<void(TestA*)>> sp(new TestA, testFoo);
    sp->m_data = "kokdoakd";
    sp->echo();

    auto type = boost::typeindex::type_id_with_cvr<decltype(foo)>().pretty_name();

    std::cout << "name = " << type.c_str() << std::endl;

    type = boost::typeindex::type_id_with_cvr<decltype(testFoo)>().pretty_name();
    std::cout << "name = " << type.c_str() << std::endl;

    return 0;
}