#ifndef __LINKLIST_BASIC_H__
#define __LINKLIST_BASIC_H__

#include <cstddef>
#include <iostream>
#include <vector>

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Crete List by vector
ListNode *CreateListByVector(const std::vector<int> &vec) {
    if (vec.empty()) {
        return nullptr;
    }
    ListNode *root = new ListNode(vec[0]);
    ListNode *node = root;
    for (size_t i = 1; i < vec.size(); ++i) {
        ListNode *tmp = new ListNode(vec[i]);
        node->next = tmp;
        node = node->next;
    }
    return root;
}

// free a linklist
void FreeList(ListNode *root) {
    ListNode *node = root;
    while (root) {
        node = root->next;
        delete root;
        root = node;
    }
}

// 删除链表的倒数n个节点
ListNode *removeNthFromEnd(ListNode *head, int n);

// 合并两个有序链表
ListNode *mergeTwoLists(ListNode *list1, ListNode *list2);

// 合并K个有序链表
ListNode *mergeKLists(std::vector<ListNode *> &lists);

// 二叉树展开为链表
void flatten(TreeNode *root);

// 给定一个链表的头部判断该链表是否有环
bool hasCycle(ListNode *head);

// 给定一个链表的头部判断该链表是否有环, 如果有返回该相交节点。
ListNode *detectCycle(ListNode *head);

// LRU缓存
class LRUCache;

#endif
