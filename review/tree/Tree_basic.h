#ifndef __TREE_BASIC_H__
#define __TREE_BASIC_H__

#include <cstddef>
#include <iostream>
#ifdef __APPLE__
#include <malloc/_malloc.h>
#endif
#include <queue>
#include <stack>
#include <vector>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {
        std::cout << "+" << std::endl;
    }
    ~TreeNode() { std::cout << "-" << std::endl; }
};

/////////////////////////////////////////////////////////////////
#include <cmath>
// for test
// create a tree for test
// 2^n - 1 = length
// length + 1 = 2^n
// log(length+1) = n
//
inline int getlevel(int size) {
    double level = log(size + 1) / log(2);
    return floor(level);
}

void freeTree(TreeNode *root);
TreeNode *GetTree(const std::vector<int> &arr);

// 遍历基础
std::vector<int> aftertravel(TreeNode *root);
std::vector<int> midtravel(TreeNode *root);
std::vector<int> PreTravel(TreeNode *root);
std::vector<std::vector<int>> levelOrder(TreeNode *root);
std::vector<int> levelOderToV1(TreeNode *root);

// 给定一个二叉树的前序遍历和中序遍历结果，尝试复原这个树。已知树里不存在重复值的节点。
TreeNode *buildTree(std::vector<int> &preorder, std::vector<int> &inorder);

// 给定一个二叉树，求每一层的节点值的平均数。使用层次遍历的方式
std::vector<double> averageOfLevels(TreeNode *root);

///////////////////////////////////////////////////////////////////////////
// 简单递归

int maxDepth(TreeNode *node);

bool isBalanced(TreeNode *node);

bool isSameTree(TreeNode *p, TreeNode *q);

bool isSymmetric(TreeNode *root);

TreeNode *invertTree(TreeNode *root);

// 求一个二叉树的最长直径。直径的定义是二叉树上任意两节点之间的无向距离。
int diameterOfBinaryTree(TreeNode *root);

///////////////////////////////////////////////////////////////////////////
// 路径

// 给定一个整数二叉树，求有多少条路径节点值的和等于给定值。
// 不需要从根节点开始，也不需要在叶子节点结束。
int pathSum(TreeNode *root, int targetSum);

// 路径和 是路径中各节点值的总和，求最大路径和。
// 沿父节点-子节点连接，达到任意节点的序列。
// 同一个节点在一条路径序列中至多出现一次
// 该路径至少包含一个节点，且不一定经过根节点。
int maxPathSum(TreeNode *root);

///////////////////////////////////////////////////////////////////////////

// 给定一个整数二叉树和一些整数，求删掉这些整数对应的节点后，剩余的子树。
std::vector<TreeNode *> delNodes(TreeNode *root, std::vector<int> &to_delete);

///////////////////////////////////////////////////////////////////////////
//二叉查找树
template <typename T> class BST {
    struct Node {
        T data;
        Node *left;
        Node *right;
    };

    Node *root;
    Node *makeEmpty(Node *t) {
        if (t == nullptr) {
            return nullptr;
        }
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        return nullptr;
    }

    Node *insert(Node *t, T x) {
        if (!t) {
            t = new Node;
            t->data = x;
            t->left = nullptr;
            t->right = nullptr;
        } else if (x < t->data) {
            t->left = insert(t->left, x);
        } else if (x > t->data) {
            t->right = insert(t->right, x);
        }
        return t;
    }

    Node *find(Node *t, T x) {
        if (!t) {
            return nullptr;
        }
        if (x < t->data)
            return find(t->left, x);
        if (x > t->data)
            return find(t->right, x);
        return t;
    }

    Node *findMin(Node *t) {
        if (t == nullptr || t->left == nullptr) {
            return t;
        }
        return findMin(t->left);
    }

    Node *findMax(Node *t) {
        if (t == nullptr || t->right == nullptr) {
            return t;
        }
        return findMax(t->right);
    }

    Node *remove(Node *t, T x) {
        Node *temp;
        if (t == nullptr) {
            return nullptr;
        } else if (x < t->data) {
            t->left = remove(t->left, x);
        } else if (x > t->data) {
            t->right = remove(t->right, x);
        } else if (t->left && t->right) { //找到目标数据了
            temp = findMin(t->right);
            t->data = temp->data;
            t->right = remove(t->right, t->data);
        } else {
            temp = t;
            if (t->left == nullptr) {
                t = t->right;
            } else if (t->right == nullptr) {
                t = t->left;
            }
            delete temp;
        }
        return t;
    }

  public:
    BST() : root(nullptr) {}
    ~BST() { root = makeEmpty(root); }

    void insert(T x) { insert(root, x); }

    void remove(T x) { remove(root, x); }
};

#endif