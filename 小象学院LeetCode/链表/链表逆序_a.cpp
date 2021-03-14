#include "Base.h"

class Solution{
public:
    ListNode* reverseList(ListNode* head){
        ListNode* newhead = NULL;
        while(head != NULL)
        {
            ListNode* temp = head->next;
            head->next = newhead;
            newhead = head;
            head = temp;
        }
        return newhead;
    }
};


class Solution2{
public:
    ListNode* reverseList(ListNode* head){
        ListNode* newhead = NULL;
        while(head)
        {
            ListNode* temp = head->next;
            head->next = newhead;
            newhead = head;
            head = temp;
        }
        return newhead;
    }
};