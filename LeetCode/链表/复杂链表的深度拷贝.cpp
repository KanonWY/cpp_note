#include "Base.h"

//复杂链表节点
struct RandomListNode
{
    int label;
    RandomListNode *next, *random;
    RandomListNode(int x)
        : label(x), next(NULL), random(NULL)
    {
    }
};

/**
 *  问题描述：已知一个复杂链表，节点中有一个节点指向链表的任意一个随机节点，
 *  求这个链表的深度拷贝。
 * 
 * **/

#include <set>
#include <vector>
#include <map>

class Solution
{
public:
    RandomListNode *copyRandomList(RandomListNode *head)
    {
        std::map<RandomListNode *, int> node_map;
        std::vector<RandomListNode *> node_vec;
        RandomListNode *ptr = head;
        int i = 0;
        while (ptr)
        {
            node_vec.push_back(new RandomListNode(ptr->label));
            node_map[ptr] = 0;
            ptr = ptr->next;
            i++;
        }
        node_vec.push_back(0);
        ptr = head;
        i = 0;
        while (ptr)
        {
            node_vec[i]->next = node_vec[i + 1];
            if (ptr->random)
            {
                int id = node_map[ptr->random];
                node_vec[i]->random = node_vec[id];
            }
            ptr = ptr->next;
            i++;
        }
        return node_vec[0];
    }
};
