#include "LinkListbasic.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <map>
#include <set>
#include <sys/syslimits.h>
#include <unordered_map>
#include <unordered_set>
#include <utility>

/**
 * @brief   删除一个链表的倒数第n个元素
 *
 */

size_t LengthOfList(ListNode *root) {
    size_t res = 0;
    while (root) {
        res++;
        root = root->next;
    }
    return res;
}

// 1->2->3->4->5->6->7->8->9
// n = 3, length = 9  ->obj = 7
// n = 1,length = 9  ->obj = 9

ListNode *removeNthFromEnd(ListNode *head, int n) {
    int length = LengthOfList(head);
    ListNode *newhead = head;
    if (n < length) {
        return head;
    } else if (n == length) {
        return head->next;
    } else {
        int Order = length - n;
        while (Order--) {
            head = head->next;
        }
        head->next = head->next->next;
    }
    return newhead;
}

/**
 * @brief     合并两个有序链表
 *
 * @param list1     有序链表1
 * @param list2     有序链表2
 * @return ListNode*    新的头节点
 */
ListNode *mergeTwoLists(ListNode *list1, ListNode *list2) {
    ListNode root(0);
    ListNode *node = &root;
    while (list1 && list2) {
        if (list1->val <= list2->val) {
            node->next = list1;
            list1 = list1->next;
        } else {
            node->next = list2;
            list2 = list2->next;
        }
        node = node->next;
    }
    if (list1) {
        node->next = list1;
    }
    if (list2) {
        node->next = list2;
    }
    return root.next;
}

/**
 * @brief 合并K个有序链表
 *
 * @param lists 有序链表的头部
 * @return ListNode*  新的节点
 */

//
//      1
//      2
//      3
//      4
//      5
//      6
//      7
//      8
// 4 + 2 + 1 = 7;
//
// log2(length)

ListNode *mergeKLists(std::vector<ListNode *> &lists) {
    int length = lists.size();
    if (length <= 0) {
        return nullptr;
    }
    ListNode *first = lists[1];
    for (int i = 1; i < length; ++i) {
        first = mergeTwoLists(first, lists[i]);
    }
    return first;
}

/**
 * @brief   将二叉树展开为链表
 *
 * @param root  二叉树根节点
 */
//  展开后的单链表应该同样使用 TreeNode ，其中 right
//  子指针指向链表中下一个结点，而左子指针始终为 null 。
// 展开后的单链表应该与二叉树 先序遍历 顺序相同。

//
//          a               a               a               a
//        /  \            /                  \
//       b    c          b                    c
//
//      ListNode* help(node)
//

TreeNode *flattern_help(TreeNode *node) {
    if (!node) {
        return nullptr;
    }
    if (!node->left && !node->right) {
        return node;
    } else if (node->left && node->right) {
        TreeNode *left_last = flattern_help(node->left);
        TreeNode *right_last = flattern_help(node->right);
        TreeNode *tmpNode = node->right;
        node->right = node->left;
        node->left = nullptr;
        left_last->right = tmpNode;
        return right_last;
    } else {
        if (node->left) {
            TreeNode *left_last = flattern_help(node->left);
            node->right = node->left;
            node->left = nullptr;
            return left_last;
        } else {
            return flattern_help(node->right);
        }
    }
}

void flatten(TreeNode *root) { flatten(root); }

/**
 * @brief   给定一个链表判断链表是否有环
 *
 * @param head  给定的链表的头部
 * @return true
 * @return false
 */

bool hasCycle(ListNode *head) {
    std::unordered_set<ListNode *> m_store;
    while (!head) {
        if (m_store.count(head)) {
            return true;
        }
        m_store.insert(head);
        head = head->next;
    }
    return false;
}

bool hasCycle_byPointer(ListNode *head) {
    if (head == nullptr || head->next == nullptr) {
        return false;
    }
    ListNode *slow = head;
    ListNode *fast = head->next;
    while (slow != fast) {
        if (fast == nullptr || fast->next == nullptr) {
            return false;
        }
        slow = slow->next;
        fast = fast->next->next;
    }
    return true;
}

/**
 * @brief 给定一个链表的头节点  head ，返回链表开始入环的第一个节点。
 * 如果链表无环，则返回 null。
 *
 * @param head
 * @return ListNode*
 */

ListNode *detectCycle(ListNode *head) {
    ListNode *fast = head;
    ListNode *slow = head;
    while (fast != nullptr) {
        slow = slow->next;
        if (fast->next == nullptr) {
            return nullptr;
        }
        fast = fast->next->next;
        if (fast == slow) {
            ListNode *ptr = head;
            while (ptr != slow) {
                ptr = ptr->next;
                slow = slow->next;
            }
            return ptr;
        }
    }
    return nullptr;
}

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
    }

    //    head <-> 1 <-> 2 <-> 3 <-> key <-> 4 <-> tail
    //  head <-> key <-> 1 <-> 2 <-> 3 <->
    int get(int key) {
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
        } else { // 不存在
            // 插入
            if (m_len >= m_capacity) {
                // 删除一个

            } else {
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
    void delOldest() {
        assert(tail.pre != &head);
        LinkNode *oldest = tail.pre;
        oldest->pre->next = &tail;
        tail.pre = oldest->pre;
        inner_map.erase(oldest->value);
        delete oldest;
    }
    LinkNode head;
    LinkNode tail;
    int m_capacity = 0;
    int m_len = 0;
    std::unordered_map<int, LinkNode *> inner_map; // 确保O(1)的找
};
