#include <iostream>
#include <unordered_map>

void testInsert() {
    std::unordered_map<int, int> m_umap;
    m_umap.insert({1, 2});
    m_umap.insert({1, 3});
    m_umap[1] = 2000;
    for (auto it : m_umap) {
        std::cout << it.first << " " << it.second << std::endl;
    }
}

void testFind() {
    std::unordered_map<int, int> m_umap;
    for (int i = 0; i < 10; ++i) {
        m_umap.insert({i, i + 10});
    }

    auto it = m_umap.find(100);
}

int main() {
    testInsert();
    return 0;
}