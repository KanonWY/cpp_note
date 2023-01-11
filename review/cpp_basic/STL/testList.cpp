#include <algorithm>
#include <iostream>
#include <list>

// 遍历测试
template <typename T> void travelList(std::list<T> &m_list) {
    for (auto it : m_list) {
        std::cout << it << std::endl;
    }
}

// 测试插入
static void testInsert() {
    std::list<int> m_list;

    // 在头部插入
    m_list.insert(m_list.begin(), 1);         // 头部插入
    m_list.insert(m_list.begin(), {2, 3, 4}); // 可以插入一个初始化表列
    m_list.insert(m_list.end(), {999, 222});
    m_list.push_front(1000); // 头部插入，仅允许插入一个元素
    m_list.push_back(20900); // 尾部插入，仅允许插入一个元素
    travelList(m_list);
}

class TestItem {
  public:
    TestItem(const TestItem &obj) { std::cout << "copy cs" << std::endl; }

    TestItem &operator=(const TestItem &obj) {
        std::cout << "operator=( copy )" << std::endl;
        return *this;
    }

    TestItem(TestItem &&rhs) noexcept { std::cout << "move cs" << std::endl; }

    TestItem &operator=(TestItem &&rhs) noexcept {
        std::cout << "operator=(move)" << std::endl;
        return *this;
    }

    TestItem() { std::cout << "default cs" << std::endl; }

    TestItem(int x, int y) { std::cout << "param cs" << std::endl; }

    virtual ~TestItem() {}
};

//  探究下push_back与emplace_back的区别？
// 底层实现有什么区别？
static void testEmplace() {
    std::list<TestItem> m_list;

    // 对于参数传递，至少可以节省一次move构造
    m_list.emplace_back(1, 2);
    m_list.push_back({1, 2});

    std::cout << "=======================" << std::endl;
    // 对于传入左值，则都是copy构造
    TestItem test1;
    m_list.push_back(test1);
    m_list.emplace_back(test1);

    // 对于右边传递，两者的效率基本是一致的
    std::cout << "=======================" << std::endl;
    m_list.push_back(TestItem());
    m_list.push_back(TestItem());
}

static void testsize() {
    std::list<int> m_list;
    m_list.insert(m_list.begin(), {1, 2, 3, 4});
    m_list.size(); // 在macos上似乎不是遍历实现的。
}

// 内置find与算法find的效率对比：
//  内置应该比通用的效率要高，具体还是要看下实现。对于unordered应该内置的效率高，因为底册是hashmap。
// 通用的话，是遍历迭代器找到一样的就返回该迭代器，如果找不到就返回end。
static void testFind() {
    std::list<int> m_list;
    m_list.insert(m_list.begin(), {1, 2, 3, 4, 5, 6, 7});

    auto res = std::find(m_list.begin(), m_list.end(), 1);
}

void testErase() {
    std::list<int> m_list;
    m_list.insert(m_list.begin(), {1, 2, 3, 4});
    m_list.erase(m_list.begin());
    m_list.erase(m_list.begin(), m_list.end());
}

int main() {
    testEmplace();
    return 0;
}