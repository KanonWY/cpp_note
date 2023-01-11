#ifndef __KARBTREE_H__
#define __KARBTREE_H__

#include <utility>
#include <vector>

// 1、每个节点只有红和黑两种颜色
// 2、根节点是黑色的
// 3、每个红色节点的两个子节点一定是黑色节点
// 4、任意节点到每个叶子节点的路径包含的黑色节点的数量相同，俗称：黑高。
// 5、每个叶子节点都为黑色的（NIL)

//

enum class Colour {
    RED,
    BLACK,
};
template <typename K, typename V> class RBTree {
    //内嵌结构体
    struct RBTreeNode {
        std::pair<K, V> _kv;
        RBTreeNode *_left;
        RBTreeNode *_right;
        RBTreeNode *_parent;

        Colour _col;

        RBTreeNode(const std::pair<K, V> &kv)
            : _kv(kv), _left(nullptr), _right(nullptr), _parent(nullptr),
              _col(Colour::RED) {}
    };

  public:
    RBTree();
    ~RBTree();
    std::vector<std::pair<K, V>> preOrder();
    std::vector<std::pair<K, V>> inOrder();
    std::vector<std::pair<K, V>> postOrder();

    RBTreeNode *search(K key);
    RBTreeNode *iterativeSearch(K key);

    std::pair<K, V> minimum();

    std::pair<K, V> maximum();

    RBTreeNode *successor(RBTreeNode *node);

    RBTreeNode *predecessor(RBTreeNode *node);

  private:
    RBTreeNode *_root;
    RBTreeNode leaf;
};

#endif