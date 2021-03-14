#include "Base.h"
#include <vector>

/**
 *      多个排序链表的合并，合并后仍然为有序的链表，染回合并后的头节点
 *      K个链表
 *      l1
 *      l2
 *      l3
 *      l4
 * ..........
 *      lk
 * **/

//way1,k个链表合并k-1次

class Solution1
{
public:
    ListNode *mergeKLists(std::vector<ListNode *> &lists)
    {
        ListNode *ret;
        for (auto it : lists)
        {
        }
    }

private:
    //合并两个有序链表
    ListNode *mergeTwoList(ListNode *l1, ListNode *l2)
    {
        ListNode temp(0);
        ListNode *pre = &temp;
        while (l1 && l2)
        {
            if (l1->val < l2->val)
            {
                pre->next = l1;
                l1 = l1->next;
            }
            else
            {
                pre->next = l2;
                l2 = l2->next;
            }
            pre = pre->next;
        }
        if (l1)
        {
            pre->next = l1;
        }
        if (l2)
        {
            pre->next = l2;
        }
        return temp.next;
    }
};

//合并k个节点

//将所有的List放到一个List中，然后排序
#include <algorithm>

class Solution
{
public:
    ListNode *mergeKLists(std::vector<ListNode *> &lists)
    {
        std::vector<ListNode *> node_vec;
        for (int i = 0; i < lists.size(); ++i)
        {
            ListNode *head = lists[i];
            while (head)
            {
                node_vec.push_back(head);
                head = head->next;
            }
        }
        if (node_vec.size() == 0)
        {
            return NULL;
        }
        std::sort(node_vec.begin(), node_vec.end(), cmp);
        for (int i = 1; i < node_vec.size(); ++i)
        {
            node_vec[i - 1]->next = node_vec[i];
        }
        node_vec[node_vec.size() - 1]->next = NULL;
        return node_vec[0];
    }

private:
    bool cmp(const ListNode *a, const ListNode *b)
    {
        return a->val > b->val;
    }
};

/**
 *  归并的方法,递归的方式实现
 * 
 * **/
class Solution3
{
public:
    ListNode *mergeKLists(std::vector<ListNode *> &lists)
    {
        if (lists.size() == 0)
        {
            return NULL;
        }
        if (lists.size() == 1)
        {
            return lists[0];
        }
        if (lists.size() == 2)
        {
            return mergerTwoLists(lists[0], lists[1]);
        }
        int mid = lists.size() / 2;
        std::vector<ListNode *> sub1_lists;
        std::vector<ListNode *> sub2_lists;
        for (int i = 0; i < mid; ++i)
        {
            sub1_lists.push_back(lists[i]);
        }
        for (int i = mid; i < lists.size(); i++)
        {
            sub2_lists.push_back(lists[i]);
        }
        ListNode *l1 = mergeKLists(sub1_lists);
        ListNode *l2 = mergeKLists(sub2_lists);
        return mergerTwoLists(l1, l2);
    }
private:
    ListNode *mergerTwoLists(ListNode *l1, ListNode *l2)
    {
        ListNode temp(0);
        ListNode *pre = &temp;
        while (l1 && l2)
        {
            if (l1->val < l2->val)
            {
                pre->next = l1;
                l1 = l1->next;
            }
            else
            {
                pre->next = l2;
                l2 = l2->next;
            }
            pre = pre->next;
        }
        if (l1)
        {
            pre->next = l1;
        }
        if (l2)
        {
            pre->next = l2;
        }
        return temp.next;
    }
};