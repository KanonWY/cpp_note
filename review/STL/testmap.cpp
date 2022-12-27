#include <iostream>
#include <map>

// map底层数据结构为红黑树
// 插入删除查找的效率为log(n)

// 1、使用for循环语句获取元素，默认从小到大排序
// 2、map只能是一个key对应一个value

// erase 方法总结
// （1）
// string& erase ( size_t pos = 0, size_t n = npos );
// 此为string特有，删除容器中从pos位置开始的n个元素。返回值是经过删除操作后的容器。

// （2）
// iterator erase ( iterator position );
// 功能是：删除容器中position所指位置的元素。返回值是指向被删元素之后的那个元素(即下一个元素)的迭代器。

// （3）
// iterator erase ( iterator first, iterator last );
// 删除容器中first到last之间的所有元素(左闭右开)，但不包括last所指的元素。

// erase导致的后续迭代器无法被当前迭代器推导（迭代器失效）

// 对于关联式的容器而言，如果删除了当前迭代器，那么当前迭代器不存在，那么再次使用该迭代器就会出现错误。
// 但是对于其他的节点并没有影响。因此在删除当前迭代之前，使用该迭代器获取到的下一个迭代器是有效的。

// 对于顺序容器而言，删除当前的迭代器会使得后续内存中元素的移动，从而使得当前迭代器失效

static int test_erase() {
    std::map<int, int> m_map;

    m_map.insert({1, 2});
    m_map.insert({10, 2});
    m_map.insert({3, 100});
    m_map.insert({4, 33});
    m_map.insert({2000, 33});

    for (auto it = m_map.begin(); it != m_map.end(); ++it) {
        std::cout << it->first << " " << it->second << std::endl;
    }
    std::cout << "m_map.size() = " << m_map.size() << std::endl;

    for (auto it = m_map.begin(); it != m_map.end();) {

        if (it->first == 4) {
            it = m_map.erase(it);
        }
        { it++; }
    }
    std::cout << "m_map.size() = " << m_map.size() << std::endl;

    for (auto it = m_map.begin(); it != m_map.end();) {
        if (it->first == 10) {
            m_map.erase(
                it++); // 感觉这个实现不是很好，如果底层是顺序容器，那么此法不行
        } else {
            it++;
        }
    }

    std::cout << "m_map.size() = " << m_map.size() << std::endl;
    return 0;
}

int test_map_insert() {
    std::map<int, int> m_map;
    m_map.insert({1,2});
    return 0;
}

int main() {
    std::cout << "main" << std::endl;
    return 0;
}