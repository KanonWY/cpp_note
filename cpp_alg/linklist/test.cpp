#include "LinkListbasic.h"
#include <iostream>

int main1() {

    int res = 0;
    LRUCache *lRUCache = new LRUCache(2);
    lRUCache->put(1, 1);    // 缓存是 {1=1}
    lRUCache->put(2, 2);    // 缓存是 {1=1, 2=2}
    res = lRUCache->get(1); // 返回 1
    std::cout << "res = " << res << std::endl;
    lRUCache->put(3, 3); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}
    res = lRUCache->get(2); // 返回 -1 (未找到)
    std::cout << "res = " << res << std::endl;
    lRUCache->put(4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}

    res = lRUCache->get(1); // 返回 -1 (未找到)
    std::cout << "res = " << res << std::endl;

    res = lRUCache->get(3); // 返回 3
    std::cout << "res = " << res << std::endl;

    res = lRUCache->get(4); // 返回 4
    std::cout << "res = " << res << std::endl;

    return 0;
}

int main() {

    int res = 0;
    LRUCache *lRUCache = new LRUCache(2);
    lRUCache->put(1, 0);
    // lRUCache->put(2, 2);
    // lRUCache->get(1);
    // lRUCache->put(3, 3);
    // lRUCache->get(2);
    // lRUCache->put(4, 4);
    // lRUCache->get(1);
    // lRUCache->get(3);
    // lRUCache->get(4);

    return 0;
}