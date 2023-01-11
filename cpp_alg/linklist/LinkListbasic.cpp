#include "LinkListbasic.h"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <map>
#include <set>
#ifdef __APPLE__
#include <sys/syslimits.h>
#endif
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

/**
 *  给你链表的头结点 head ，请将其按 升序 排列并返回 排序后的链表 。
 *
 *
 */

ListNode *sortList(ListNode *head) {
    if (!head) {
        return nullptr;
    }
    int length = LengthOfList(head);
    ListNode *dummyHead = new ListNode(0);
    dummyHead->next = head;
    for (int subLength = 1; subLength < length; subLength <<= 1) {
        ListNode *pre = dummyHead, *curr = dummyHead->next;
        while (curr) {
            ListNode *head1 = curr;
            for (int i = 1; i < subLength && curr->next != nullptr; i++) {
                curr = curr->next;
            }
            ListNode *head2 = curr->next;
            curr->next = nullptr;
            curr = head2;
            for (int i = 0;
                 i < subLength && curr != nullptr && curr->next != nullptr;
                 i++) {
                curr = curr->next;
            }
            ListNode *next = nullptr;
            if (curr != nullptr) {
                next = curr->next;
                curr->next = nullptr;
            }
            ListNode *merged = mergeTwoLists(head1, head2);
            pre->next = merged;
            while (pre->next != nullptr) {
                pre = pre->next;
            }
            curr = next;
        }
    }
    return dummyHead->next;
}

// 可能存在重复的信息，因此使用multimap·
ListNode *sortList_2(ListNode *head) {
    if (!head) {
        return nullptr;
    }
    std::multimap<int, ListNode *> inner_map;
    while (head) {
        inner_map.insert({head->val, head});
        head = head->next;
    }
    auto m = inner_map.begin();
    for (auto it = ++(inner_map.begin()); it != inner_map.end(); ++it) {
        m->second->next = it->second;
        m = it;
    }
    return inner_map.begin()->second;
}

ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    int length_1 = LengthOfList(headA);
    int length_2 = LengthOfList(headB);
    if (length_1 > length_2) {
        int delta = length_1 - length_2;
        while (delta-- && headA != nullptr) {
            headA = headA->next;
        }
    } else {
        int delta = length_2 - length_1;
        while (delta-- && headB != nullptr) {
            headB = headB->next;
        }
    }
    while (headA && headB) {
        if (headA == headB) {
            return headA;
        }
        headA = headA->next;
        headB = headB->next;
    }
    return nullptr;
}

/**
 *     @brief 给你单链表的头节点 head ，请你反转链表，并返回反转后的链表。
 *
 *     head: 输入的链表头
 */
ListNode *reverseList(ListNode *head) {
    if (!head) {
        return nullptr;
    }
    ListNode *pre = nullptr;
    while (head) {
        ListNode *temp = head->next;
        head->next = pre;
        pre = head;
        head = temp;
    }
    return pre;
}

/**
 *  @brief 给你一个单链表的头节点
 *  head,请你判断该链表是否为回文链表。如果是，返回 true
 * ；否则，返回 false
 *
 *
 *  //1.栈处理
 *  //2.反转链表
 *
 */

//  1->2->3->4->5->6->7

// 1 -> 2 -> 3->4->5->6

ListNode *TailOfFirst(ListNode *head) {
    ListNode *fast = head;
    ListNode *slow = head;
    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    return slow;
}
bool isPalindrome(ListNode *head) {
    if (!head) {
        return true;
    }
    ListNode *firstPartEnd = TailOfFirst(head);
    ListNode *SecondPartStart = reverseList(firstPartEnd->next);
    ListNode *p1 = head;
    ListNode *p2 = SecondPartStart;
    bool result = true;
    while (result && p2 != nullptr) {
        if (p1->val != p2->val) {
            result = false;
        }
        p1 = p1->next;
        p2 = p2->next;
    }
    firstPartEnd->next = reverseList(SecondPartStart);
    return result;
}
