#include "Base.h"

//已知链表的头指针和数值x，将小于x的节点放在大于等于x的节点之前，且保持这些链表相等。

/**
 *  核心：临时头节点
 * 
 * 
 **/

class Solution
{
public:
    ListNode *partition(ListNode *head, int x)
    {
        ListNode less_head(0);
        ListNode more_head(0);
        ListNode *less_ptr = &less_head;
        ListNode *more_ptr = &more_head;
        while (head)
        {
            if (head->val <= x)
            {
                less_ptr->next = head;
                less_ptr = less_ptr->next;
            }
            else
            {
                more_ptr->next = head;
                more_ptr = more_ptr->next;
            }
            head = head->next;
        }
        less_ptr->next = more_head.next;
        more_ptr->next = NULL;      //为什么置空
        return less_head.next;
    }
};