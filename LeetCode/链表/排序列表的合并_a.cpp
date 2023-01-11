#include "Base.h"

/**
 *  已知两个已经排序的链表头节点指针l1,l2;将这两个链表合并，
 *  合并后仍为有序的，返回合并后的链表头。
 *      l1  a->b->c->d->e->f->nil
 *      l2   h->i->j->k->l->m->n->nil
 * **/

class Solution
{
public:
    ListNode *mergeTwoLists(ListNode *l1, ListNode *l2)
    {
        ListNode temp_head(0);      //临时头节点
        ListNode *pre = &temp_head; //pre指针指向临时头节点
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
        if (l1) //l1仍然有剩余
        {
            pre->next = l1;
        }
        if (l2) //l2仍然有剩余
        {
            pre->next = l2;
        }
        return temp_head.next;
    }
};