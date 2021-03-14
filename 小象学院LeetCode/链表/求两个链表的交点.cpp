#include "Base.h"

class Solution1{
public:
    ListNode* getIntersectionNode(ListNode* headA,ListNode* headB)
    {
        size_t lenA = len(headA);
        size_t lenB = len(headB);
        if(lenA >= lenB)
        {
            int delt = lenA - lenB;
            while(delt--)
            {
                headA = headA->next;
            }
        }
        else
        {
            int delt = lenB - lenA;
            while(delt--)
            {
                headB = headB->next;
            }
        }
        while(headA && headB)
        {
            if(headA == headB)
            {
                return headA;
            }
            headA = headA->next;
            headB = headB->next;
        }
        return NULL;
    }
private:
    size_t len(ListNode* Node)
    {
        size_t len = 0;
        while(Node)
        {
            Node = Node->next;
            len++;
        }
        return len;
    }
};


/*使用set双遍历的方式*/
#include <set>

//时间复杂度nlogn,空间复杂度n

class Solution2{
public:
    ListNode* getIntersectionNode(ListNode* headA,ListNode* headB)
    {
        std::set<ListNode*> node_set;
        while(headA)
        {
            node_set.insert(headA);
            headA = headA->next;
        }
        while(headB)
        {
            if(node_set.find(headB) != node_set.end())
            {
                return headB;
            }
            headB = headB->next;
        }
        return NULL;
    }

};