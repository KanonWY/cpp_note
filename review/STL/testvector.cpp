#include <iostream>
#include <vector>

// 关于容量与size的关系
//   resize(size，T())函数的作用是修改size的大小，
//           如果原本的cap > size, 则会将cap-size的元素初始化为T(),
//           如果原本的cap = size，不会做任何操作
//           如果原本的cap < size,
//           会进行扩容操作，然后先将原本的数据copy，然后size-cap初始化。
//
//   reserve(size) 函数的作用是改变vector的容量
//           如果size > cap,
//           则会对数组进行扩容操作，扩容为size，然后将原本的数组copy。 如果size
//           < cap, 不会有任何操作。
//
//
//
//  shrink_to_fit() 函数的作用是自动调整cap = size
//           如果size == 0，则cap就会变为0
//
//  clear()函数的作用是清空元素的内容，改变的size的大小，但是不改变cap的大小

int test_size_and_cap() {
    std::vector<int> m_vec;
    m_vec.reserve(1000);
    std::cout << "size = " << m_vec.size() << std::endl;
    std::cout << "cap = " << m_vec.capacity() << std::endl;

    m_vec.resize(2000, 2000);
    // resize会影响cap
    std::cout << "size = " << m_vec.size() << std::endl;
    std::cout << "cap = " << m_vec.capacity() << std::endl;

    std::cout << "m_vec[1000] = " << m_vec[1000] << std::endl;
    // 使用[]越界有问题
    // std::cout << "m_vec[2000] = " << m_vec[2000] << std::endl;
    // std::cout << "m_vec.at(2000) = " << m_vec.at(2000) << std::endl;

    // resize缩小之后，cap还是不变的
    m_vec.resize(10);
    std::cout << "size = " << m_vec.size() << std::endl;
    std::cout << "cap = " << m_vec.capacity() << std::endl;

    // reserve如果收缩一个比原本cap小的单位，不会有任何改变的
    m_vec.reserve(1000);
    std::cout << "size = " << m_vec.size() << std::endl;
    std::cout << "cap = " << m_vec.capacity() << std::endl;

    // clear()之后cap是否会改变？
    // clear之后改变的是size为0，不变的是cap值
    m_vec.clear();
    std::cout << "size = " << m_vec.size() << std::endl;
    std::cout << "cap = " << m_vec.capacity() << std::endl;

    // 使用shrink_to_fit之的变化呢
    // 会收缩到原本的size大小吗？
    m_vec.resize(100);
    m_vec.shrink_to_fit();
    std::cout << "size = " << m_vec.size() << std::endl;
    std::cout << "cap = " << m_vec.capacity() << std::endl;

    // resize修改,如果大于原本的cap，就会升级cap到新的resize的大小
    m_vec.resize(200, 99);
    std::cout << "size = " << m_vec.size() << std::endl;
    std::cout << "cap = " << m_vec.capacity() << std::endl;

    // 如果reserver函数，如果指定的size小于cap的大小，则不会做任何操作
    m_vec.reserve(100);
    std::cout << "size = " << m_vec.size() << std::endl;
    std::cout << "cap = " << m_vec.capacity() << std::endl;

    m_vec.reserve(1000);
    std::cout << "size = " << m_vec.size() << std::endl;
    std::cout << "cap = " << m_vec.capacity() << std::endl;

    // size < cap, 使用shrink_to_fit函数
    //  会将size与cap收缩到一个大小
    m_vec.shrink_to_fit();
    std::cout << "size = " << m_vec.size() << std::endl;
    std::cout << "cap = " << m_vec.capacity() << std::endl;
    m_vec.reserve(1000);
    std::cout << "size = " << m_vec.size() << std::endl;
    std::cout << "cap = " << m_vec.capacity() << std::endl;

    // 使用swap进行收缩
    std::vector<int> m_vec2;
    std::swap(m_vec2, m_vec);
    std::cout << "size = " << m_vec.size() << std::endl;
    std::cout << "cap = " << m_vec.capacity() << std::endl;

    std::cout << "size = " << m_vec2.size() << std::endl;
    std::cout << "cap = " << m_vec2.capacity() << std::endl;

    return 0;
}

int testErase() {
    std::vector<int> m_vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto it = m_vec.begin();
    for (; it != m_vec.end();) {
        if (*it == 5) {
            m_vec.erase(it++);
            break;
        } else {
            it++;
        }
    }
    std::cout << "*it = " << *it << std::endl; // 7
    return 0;
}

// 测试表明，vector insert会在扩容与不扩容有不同的表现。
// 当insert的时候，一旦发生了扩容现象，那么所有的迭代器都会失效
// 但是insert会返回，插入首元素的有效迭代器。

// inert如果没有导致扩容，那么当前迭代会变为指向新插入的第一个元素。

void test_insert() {
    std::vector<int> m_vec;
    m_vec.reserve(100);
    for (int i = 1; i < 10; ++i) {
        m_vec.push_back(i);
    }
    std::cout << "size = " << m_vec.size() << std::endl;
    std::cout << "cap = " << m_vec.capacity() << std::endl;

    auto it = m_vec.begin();

    for (; it != m_vec.end();) {
        if (*it == 5) {
            m_vec.insert(it, {100, 200});
            break;
        } else {
            it++;
        }
    }
    it++;
    std::cout << "*it = " << *it << std::endl;

    for (auto const &it : m_vec) {
        std::cout << it << std::endl;
    }
}

int main() {
    test_insert();
    return 0;
}
