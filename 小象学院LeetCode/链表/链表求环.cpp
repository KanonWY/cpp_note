#include "Base.h"

//已知链表中存在环，求出环的起始节点

//使用set保存数据
#include <set>

class Solution1
{
public:
    ListNode *detectCycle(ListNode *head)
    {
        std::set<ListNode *> node_set;
        while (head)
        {
            if (node_set.find(head) != node_set.end())
            {
                return head;
            }
            node_set.insert(head);
            head = head->next;
        }
        return NULL;
    }
};

/*快慢指针
*           sp1 ->  2v            
*           sp2 ->  v
*           若有环，快慢指针必定相遇。
*           核心：相遇点到环起始点的距离 = 起始点到环起点的距离
*           1：找到相遇点
*           2：从起始点、相遇点以相同的速度出发，相遇点就是环起始点
*/
class Solution2
{
public:
    ListNode *detectCycle(ListNode *head)
    {
        ListNode *fast = head;
        ListNode *slow = head;
        ListNode *meet = NULL;
        while (fast)                //通过快速指针遍历
        {
            slow = slow->next;      //先各自走一步
            fast = fast->next;
            if (!fast)              //快指针为NULL，说明没有链
            {
                return NULL;
            }
            fast = fast->next;      //快指针走一步
            if (fast == slow)
            {
                meet = fast;
                break;
            }
        }
        if (meet == NULL)       //不相遇，说明到达结尾了
        {
            return NULL;
        }
        while (head && meet)
        {
            if (head == meet)
            {
                return head;
            }
            head = head->next;
            meet = meet->next;
        }
        return NULL;
    }
};