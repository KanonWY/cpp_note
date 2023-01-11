#include "Base.h"

//将一个链表从m到n逆序，m指示的是从链表的个数

//  a->b->c->d->e->f->g
//  a->f->e->d->c->b->g
//      1.首先找到m，如果m是NULL就不需要逆序了
//      2.然后向后逆序n-m次就可以了

class Solution{
    ListNode* reverBetween(ListNode* head,int m,int n)
    {
        ListNode* ret = head;
        int len = n-m+1;
        ListNode* head_before = NULL;       //核心
        while(--m && head)
        {   
            head_before = head;
            head = head->next;
        }
        ListNode* newhead = NULL;
        ListNode* tail = head;
        while(len-- && head)
        {
            ListNode* temp = head->next;
            head->next = newhead;
            newhead = head;
            head = temp;
        }
        //for here newhead = the new head of reverse list
        // head = end of revers list ->next;
        tail->next = head;
        if(head_before)
        {
            head_before->next = newhead;
        }
        else{
            ret = newhead;
        }
        return ret;
    }
};