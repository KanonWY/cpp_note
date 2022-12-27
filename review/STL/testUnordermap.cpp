#include <iostream>
#include <unordered_map>

void testInsert() {
    std::unordered_map<int, int> m_umap;
    m_umap.insert({1, 2});
    m_umap.insert({1, 3});
    m_umap[1] = 29999;
    for (auto it : m_umap) {
        std::cout << it.first << " " << it.second << std::endl;
    }
}

int main() {
    testInsert();
    return 0;
}