#ifndef __LINKLIST_BASIC_H__
#define __LINKLIST_BASIC_H__

#include <cassert>
#include <cstddef>
#include <iostream>
#include <unordered_map>
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
static ListNode *CreateListByVector(const std::vector<int> &vec) {
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
static void FreeList(ListNode *root) {
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
// LRU缓存
// 实现 LRUCache 类：
// LRUCache(int capacity) 以 正整数 作为容量 capacity 初始化 LRU 缓存
// int get(int key) 如果关键字 key 存在于缓存中，则返回关键字的值，否则返回 -1
// 。 void put(int key, int value) 如果关键字 key 已经存在，则变更其数据值 value
// ；如果不存在，则向缓存中插入该组 key-value
// 。如果插入操作导致关键字数量超过 capacity ，则应该 逐出 最久未使用的关键字。

class LRUCache {
  public:
    struct LinkNode {
        int value;
        LinkNode *pre, *next;
        LinkNode() : value(0), pre(nullptr), next(nullptr) {}
        LinkNode(int x) : value(x), pre(nullptr), next(nullptr) {}
    };
    LRUCache(int capacity) {
        m_capacity = capacity;
        m_len = 0;
        head.next = &tail;
        tail.pre = &head;
    }

    //    head <-> 1 <-> 2 <-> 3 <-> key <-> 4 <-> tail
    //  head <-> key <-> 1 <-> 2 <-> 3 <->
    int get(int key) {
        // Dump();
        if (inner_map.count(key)) {
            LinkNode *node = inner_map[key];
            // 更新信息
            updateNodeToHead(node);
            return node->value;

        } else {
            return -1;
        }
    }

    void put(int key, int value) {
        if (inner_map.count(key)) { // 存在
            LinkNode *node = inner_map[key];
            if (node->value != value) {
                node->value = value;
            }
            updateNodeToHead(node);
        } else {                       // 不存在
            if (m_len >= m_capacity) { //删除一个，插入一个
                //删除最近不常用的
                _del_oldest();
                //插入新的节点到链表头部
                LinkNode *newNode = new LinkNode(value);
                _insert_to_head(newNode);
                //插入哈希表中
                inner_map.insert({key, newNode});
            } else {
                LinkNode *newNode = new LinkNode(value);
                _insert_to_head(newNode);
                inner_map.insert({key, newNode});
                m_len++;
            }
        }
    }
    ~LRUCache() {
        for (auto it = inner_map.begin(); it != inner_map.end();) {
            if (it->second != nullptr) {
                delete it->second;
                it->second = nullptr;
            }
        }
    }

  private:
    void updateNodeToHead(LinkNode *key) {

        // 断
        key->next->pre = key->pre;
        key->pre->next = key->next;

        // 接
        key->next = head.next;
        key->pre = &head;
        head.next = key;
        key->next->pre = key;
    }

    //   -----    n <-> m <->oldest <-> tail
    void _del_oldest() {
        assert(tail.pre != &head);
        LinkNode *oldest = tail.pre;
        oldest->pre->next = &tail;
        tail.pre = oldest->pre;
        inner_map.erase(
            oldest->value); // erase(key) return the number of key-value
        delete oldest;
    }

    void _insert_to_head(LinkNode *newNode) {
        if (m_len == 0) {
            head.next = newNode;
            newNode->pre = &head;
            newNode->next = &tail;
            tail.pre = newNode;
        } else { //必有一个oldhead节点
            LinkNode *oldhead = head.next;
            assert(oldhead != nullptr);
            head.next = newNode;
            newNode->pre = &head;
            newNode->next = oldhead;
            oldhead->pre = newNode;
        }
    }
    LinkNode head;
    LinkNode tail;
    int m_capacity = 0;
    int m_len = 0;
    std::unordered_map<int, LinkNode *> inner_map; // 确保O(1)的时间复杂度
};

//给你链表的头结点 head ，请将其按 升序 排列并返回 排序后的链表 。
ListNode *sortList(ListNode *head);

//给你两个单链表的头节点 headA 和 headB
//，请你找出并返回两个单链表相交的起始节点。如果两个链表不存在相交节点，返回
// null 。
ListNode *getIntersectionNode(ListNode *headA, ListNode *headB);

//给你单链表的头节点 head ，请你反转链表，并返回反转后的链表。
ListNode *reverseList(ListNode *head);

//给你一个单链表的头节点 head ，请你判断该链表是否为回文链表。如果是,返回
// true,否则，返回 false 。
bool isPalindrome(ListNode *head);

#endif
