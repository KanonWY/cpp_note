#include "Tree_basic.h"
#include <__tree>
#include <algorithm>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <queue>
#include <stack>
#include <vector>

// 非递归前序遍历
std::vector<int> PreTravel(TreeNode *root) {
    std::vector<int> res;
    if (!root) {
        return res;
    }
    std::stack<TreeNode *> m_stack;
    m_stack.push(root);
    while (!m_stack.empty()) {
        auto tmp = m_stack.top();
        res.push_back(tmp->val);
        m_stack.pop();
        // right
        if (tmp->right != nullptr) {
            m_stack.push(tmp->right);
        }
        if (tmp->left != nullptr) {
            m_stack.push(tmp->left);
        }
    }
    return res;
}

// mid travel
//
//       a
//       /   \
//   /     \
//  b       c
//

// 非递归中序遍历
std::vector<int> midtravel(TreeNode *root) {
    std::vector<int> res;
    if (!root) {
        return res;
    }
    std::stack<TreeNode *> m_stack;
    TreeNode *curr = root;

    while (curr != nullptr || !m_stack.empty()) {
        while (curr != nullptr) {
            m_stack.push(curr);
            curr = curr->left;
        }
        auto top_element = m_stack.top();
        res.push_back(top_element->val);
        m_stack.pop();
        curr = top_element->right;
    }
    return res;
}

// 非递归后续遍历
std::vector<int> aftertravel(TreeNode *root) {
    std::vector<int> res;
    if (!root) {
        return res;
    }
    std::stack<TreeNode *> s1;
    std::stack<TreeNode *> s2;
    s1.push(root);
    while (!s1.empty()) {
        auto tmp = s1.top();
        s1.pop();
        s2.push(tmp);
        if (tmp->left)
            s1.push(tmp->left);
        if (tmp->right)
            s1.push(tmp->right);
    }
    while (!s2.empty()) {
        res.push_back(s2.top()->val);
        s2.pop();
    }
    return res;
}

// 层序遍历
std::vector<std::vector<int>> levelOrder(TreeNode *root) {
    std::vector<std::vector<int>> res;
    if (!root) {
        return res;
    }
    std::queue<TreeNode *> m_queue;
    m_queue.push(root);
    while (!m_queue.empty()) {
        std::vector<int> tmp(m_queue.size(), 0);
        for (int i = 0; i < tmp.size(); ++i) {
            auto node = m_queue.front();
            m_queue.pop();
            if (node->left) {
                m_queue.push(node->left);
            }
            if (node->right) {
                m_queue.push(node->right);
            }
            tmp[i] = node->val;
        }
        res.push_back(tmp);
    }
    return res;
}

// 层次遍历，返回一维数组
std::vector<int> levelOderToV1(TreeNode *root) {
    std::vector<int> res;
    if (!root) {
        return res;
    }
    std::queue<TreeNode *> m_queue;
    m_queue.push(root);
    while (!m_queue.empty()) {
        auto tmpNode = m_queue.front();
        m_queue.pop();
        res.push_back(tmpNode->val);
        if (tmpNode->left) {
            m_queue.push(tmpNode->left);
        }
        if (tmpNode->right) {
            m_queue.push(tmpNode->right);
        }
    }
    return res;
}

#include <numeric>

// 计算每一层的平均值
std::vector<double> averageOfLevels(TreeNode *root) {
    std::vector<double> res;
    if (!root) {
        return res;
    }
    std::queue<TreeNode *> m_queue;
    m_queue.push(root);
    while (!m_queue.empty()) {
        std::vector<int> tmp(m_queue.size(), 0);
        for (int i = 0; i < tmp.size(); ++i) {
            auto node = m_queue.front();
            m_queue.pop();
            tmp[i] = node->val;
            if (node->left) {
                m_queue.push(node->left);
            }
            if (node->right) {
                m_queue.push(node->right);
            }
        }
        double sum = std::accumulate(std::begin(tmp), std::end(tmp), 0.0);
        double mean = sum / tmp.size();
        res.push_back(mean);
    }
    return res;
}

// 根据vector构建一颗树
TreeNode *GetTree(const std::vector<int> &arr) {
    if (arr.size() == 0) {
        return nullptr;
    }
    std::queue<TreeNode *> Inner_queue;
    int index = 0;
    TreeNode *node = new TreeNode(arr[index]);
    TreeNode *ret = node;
    index++;
    Inner_queue.push(node);
    while (index < arr.size() && !Inner_queue.empty()) {
        auto tmp = Inner_queue.front();
        Inner_queue.pop();
        auto leftNode = new TreeNode(arr[index++]);
        tmp->left = leftNode;
        Inner_queue.push(leftNode);
        if (index >= arr.size()) {
            break;
        }
        auto rightNode = new TreeNode(arr[index++]);
        tmp->right = rightNode;
        Inner_queue.push(rightNode);
    }
    return ret;
}
// 释放堆中的树
void freeTree(TreeNode *root) {
    if (root == nullptr) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

///////////////////////////////////////////////////////////////////////////
// 树的深度
int maxDepth(TreeNode *node) {
    if (!node) {
        return 0;
    } else {
        return std::max(maxDepth(node->left), maxDepth(node->right)) + 1;
    }
}

int isBalanced_help(TreeNode *node) {
    if (!node) {
        return 0;
    }
    int left = isBalanced_help(node->left);
    int right = isBalanced_help(node->right);
    if (left == -1 || right == -1 || std::abs(left - right) > 1) {
        return -1;
    }
    return 1 + std::max(left, right);
}

// 是否平衡
// 平衡判断：对于树上的任意节点，其两侧节点的最大深度 的差值不得大于 1。
bool isBalanced(TreeNode *node) {
    if (isBalanced_help(node) == -1) {
        return false;
    } else {
        return true;
    }
}

// 两个树的值是否相等
bool isSameTree(TreeNode *p, TreeNode *q) {
    if (p == nullptr && q == nullptr) {
        return true;
    } else if (p == nullptr || q == nullptr) {
        return false;
    } else if (p->val != q->val) {
        return false;
    } else {
        return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
    }
}

bool isSymmetric_help(TreeNode *node1, TreeNode *node2) {
    if (!node1 && !node1) {
        return true;
    } else if (!node1 || !node2) {
        return false;
    } else if (node1->val != node2->val) {
        return false;
    } else {
        return isSymmetric_help(node1->left, node2->right) &&
               isSymmetric_help(node1->right, node2->left);
    }
}

// 判断一个二叉树是否对称。
bool isSymmetric(TreeNode *root) {
    if (!root) {
        return true;
    } else {
        return isSymmetric_help(root->left, root->right);
    }
}

void invertTree_help(TreeNode *left, TreeNode *right, TreeNode *parent) {
    if (!left && !right) {
        return;
    } else {
        parent->left = right;
        parent->right = left;
    }
    if (left) {
        invertTree_help(left->left, left->right, left);
    }
    if (right) {
        invertTree_help(right->left, right->right, right);
    }
}

// 翻转二叉树
TreeNode *invertTree(TreeNode *root) {
    if (!root) {
        return nullptr;
    }
    invertTree_help(root->left, root->right, root);
    return root;
}

/**
 * @brief: 求二叉树的直径。
 *  一棵二叉树的直径长度是任意两个结点路径长度中的最大值。这条路径可能穿过也可能不穿过根结点。
 *
 *          1
 *        / \
 *       2   3
 *      / \
 *     4   5
 */

int diameterOfBinaryTree_help(TreeNode *node, int &diameter) {
    if (!node) {
        return 0;
    }
    int left = diameterOfBinaryTree_help(node->left, diameter);
    int right = diameterOfBinaryTree_help(node->right, diameter);
    diameter = std::max(left + right, diameter);
    return std::max(left, right) + 1;
}

int diameterOfBinaryTree(TreeNode *root) {
    if (!root)
        return 0;
    int diameter = 0;
    diameterOfBinaryTree_help(root, diameter);
    return diameter;
}

/**
 * @brief :二叉树里节点值之和等于 targetSum 的路径的数目
 *
 */
// 关键在于辅助函数如何构造？
int pathSum_help(TreeNode *node, int sum) {
    if (!node) {
        return 0;
    }
    int count = node->val == sum ? 1 : 0;
    count += pathSum_help(node->left, sum - node->val);
    count += pathSum_help(node->right, sum - node->val);
    return count;
}

int pathSum(TreeNode *root, int targetSum) {
    if (!root) {
        return 0;
    }
    return pathSum_help(root, targetSum) + pathSum_help(root->left, targetSum) +
           pathSum_help(root->right, targetSum);
    return 0;
}

/**
 * @brief 求一个树的最大路径值
 *
 */

int maxPathSum_help(TreeNode *node, int &maxValue) {
    if (!node) {
        return 0;
    }
    int left = maxPathSum_help(node->left, maxValue);
    int right = maxPathSum_help(node->right, maxValue);
    maxValue = std::max(maxValue, left + right + node->val);
    return std::max(left, right) + node->val;
}

int maxPathSum(TreeNode *root) {
    if (!root) {
        return 0;
    }
    int maxValue = 0;
    maxPathSum_help(root, maxValue);
    return maxValue;
}

/**
 * @brief 给你一个整数 n ，求恰由 n 个节点组成且节点值从 1 到 n 互不相同的
 *        二叉搜索树 有多少种？ 返回满足题意的二叉搜索树的种数。
 *
 */
int numTrees(int n) {
    std::vector<int> G(n + 1, 0);
    G[0] = 0;
    G[1] = 1;
    for (int i = 2; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            G[i] = G[j - 1] * G[i - j];
        }
    }
    return G[n];
}

/**
 * @brief  给你一个二叉树的根节点 root ，判断其是否是一个有效的二叉搜索树
 *          二叉搜索树的定义：
 *          （1）节点的左子树只包含 小于 当前节点的数。
 *          （2）节点的右子树只包含 大于 当前节点的数。
 *          （3）所有左子树和右子树自身必须也是二叉搜索树。
 * @param root
 * @return true
 * @return false
 */

//
//                a
//            /      \
//           b        e
//         /            \
//        c              m
//
//

bool isValidBST(TreeNode *root) {
    if (!root) {
        return true;
    }
    std::stack<TreeNode *> m_stack;
    TreeNode *curr = root;
    long long preOrder = (long long)INT_MIN - 1;
    while (!m_stack.empty() || curr != nullptr) {
        while (curr) {
            m_stack.push(curr->left);
            curr = curr->left;
        }
        TreeNode *tmp = m_stack.top();
        m_stack.pop();
        if (tmp->val <= preOrder) {
            return false;
        }
        preOrder = tmp->val;
        curr = tmp->right;
    }
    return true;
}

/**
 * @brief 给出二叉搜索树的根节点，
 * 该树的节点值各不相同，请你将其转换为累加树（Greater SumTree），使每个节点
 * node 的新值等于原树中大于或等于 node.val 的值之和。
 *
 * @param root
 * @return TreeNode*
 */

TreeNode *convertBST(TreeNode *root) {
    if (!root) {
        return nullptr;
    }
    int sum = 0;
    std::stack<TreeNode *> m_stack;
    TreeNode *curr = root;
    while (!m_stack.empty() || curr) {
        while (curr) {
            m_stack.push(curr);
            curr = curr->right;
        }
        TreeNode *tmp = m_stack.top();
        m_stack.pop();
        tmp->val = sum + tmp->val;
        sum = tmp->val;
        curr = tmp->left;
    }
    return root;
}

int help_convertBST_2 = 0;
TreeNode *convertBST_2(TreeNode *root) {
    if (root != nullptr) {
        convertBST_2(root->right);
        help_convertBST_2 += root->val;
        root->val = help_convertBST_2;
        convertBST_2(root->left);
    }
    return root;
}

/**
 * @brief 二叉树的最近公共祖先
 *      对于有根树 T 的两个节点 p、q，最近公共祖先表示为一个节点 x，满足 x 是
 *      p、q 的祖先且 x 的深度尽可能大 （一个节点也可以是它自己的祖先）
 * @param root
 * @param p
 * @param q
 * @return TreeNode*
 */

TreeNode *lowestCommonAncestor_help_node;
bool lowestCommonAncestor_help(TreeNode *root, TreeNode *p, TreeNode *q) {
    if (!root) {
        return false;
    }
    bool inCurrentNode = (root->val == p->val || root->val == q->val);
    bool inLeft = lowestCommonAncestor_help(root->left, p, q);
    bool inRight = lowestCommonAncestor_help(root->right, p, q);
    if ((inLeft && inRight) || ((inCurrentNode) && (inLeft || inRight))) {
        lowestCommonAncestor_help_node = root;
    }
    return inCurrentNode || inLeft || inRight;
}

TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q) {
    lowestCommonAncestor_help(root, p, q);
    return lowestCommonAncestor_help_node;
}
