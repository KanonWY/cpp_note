#include <iostream>
#include <vector>
/**
 * 
 *          必须有递归调用的终止条件，否则会一直执行，直到内存耗尽。
 *          
 *          （1）递归函数有传出值，可以作为返回值返回，也可以使用传出参数进行返回。
 * 
 * 
 * **/

//返回void，所以这个的传递值，是使用传出参数获取的。
void compute_sum(int i, int &sum)
{
    if (i > 3)
    {
        return;
    }
    sum += i;
    compute_sum(i + 1, sum);
}

struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int x)
        : val(x), next(NULL) {}
};

void add_to_vector(ListNode *head, std::vector<int> &vec)
{
    if (head == NULL)
    {
        return;
    }
    vec.push_back(head->val);
    add_to_vector(head->next, vec);
}

/**
 * 
 *      回溯法，当探索到某一步的时候，达不到目标，就退回重新选择。
 *      简单的说，回溯法就是走不通就退回重走的概念。
 * 
 * 
 * 
 * **/