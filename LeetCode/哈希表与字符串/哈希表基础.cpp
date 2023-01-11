#include <vector>
#include <string>
#include <iostream>

/**
 * 
 *      key 代入到哈希函数中，可以得到一个哈希值，
 *      哈希值值对应到数组中的一个位置
 * 
 * **/

/**
 *  字符哈希
 * 
 *     assii码0-127,因此可以使用数组作为映射
 *       
 * **/

int main_01()
{
    int char_map[128] = {0};
    std::string str = "abcdefgaaxxy";
    for (int i = 0; i < str.length(); i++)
    {
        char_map[str[i]]++;
    }
    for (int i = 0; i < 128; i++)
    {
        if (char_map[i] > 0)
        {
            printf("[%c][%d]:%d\n", i, i, char_map[i]);
        }
    }
    return 0;
}

int main_02()
{
    int random[10] = {999, 1, 444, 7, 20, 9, 1, 3, 7, 7};
    int hash_map[1000] = {0};

    for (int i = 0; i < 10; i++)
    {
        hash_map[random[i]]++;
    }
    for (int i = 0; i < 1000; ++i)
    {
        for (int j = 0; j < hash_map[i]; ++j)
        {
            printf("%d\n", i);
        }
    }
    return 0;
}

struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int x)
        : val(x), next(NULL) {}
};

int hash_func(int key, int table_len)
{
    return key % table_len;
}

void insert(ListNode *hashtable[], ListNode *node, int table_len)
{
    int hash_key = hash_func(node->val, table_len);
    node->next = hashtable[hash_key];
    hashtable[hash_key] = node;
}


//在开链法中搜寻
bool search(ListNode *hash_table[], int value, int table_len)
{
    int hash_key = hash_func(value, table_len);
    ListNode *head = hash_table[hash_key];
    while (head)
    {
        if (head->val == value)
        {
            return true;
        }
        head = head->next;
    }
    return false;
}

/**
 *  hashtable取质数，冲突比其他的要少
 * 
 * 
 * **/

/**
 *      stl中的map,set是使用红黑树实现的，
 *      
 *      unorder是通过hash实现的。
 * 
 * **/

int main()
{
    main_02();
    return 0;
}