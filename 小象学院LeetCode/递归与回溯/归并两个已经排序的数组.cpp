#include <vector>

/**
 * 
 *      已知两个已排序的数组，将这两个合并为一个。
 *      arr1[] = {2 ,5, 8, 20};
 *      arr2[] = {1, 3, 5, 7, 30, 50};
 * 
 *       
 *      
 * 
 * 
 * **/

void merge_sort_two_vec(std::vector<int> &sub1,
                        std::vector<int> &sub2,
                        std::vector<int> &res)
{
    int i = 0, j = 0;
    while (i < sub1.size() && j < sub2.size())
    {
        if (sub1[i] <= sub2[j])
        {
            res.push_back(sub1[i]);
            ++i;
        }
        else
        {
            res.push_back(sub2[j]);
            ++j;
        }
    }
    for (; i < sub1.size(); i++)
    {
        res.push_back(sub1[i]);
    }
    for (; j < sub2.size(); j++)
    {
        res.push_back(sub2[j]);
    }
}

//归并排序

/**
 *      [X1,X2,X3,X4,X5,X6,X7,X8,X9]
 *      
 *      必须有一个临时的数组来存放中间数据？
 *      因为将前后排序好的数组再做归并，因此必须要有一个temp。
 * 
 * 
 * **/

void merge_sort(std::vector<int> &nums, int l, int r, std::vector<int> &temp)
{
    if (l + 1 >= r)
    {
        return;
    }
    int mid = l + (r - 1) / 2;
    merge_sort(nums, l, mid, temp);
    merge_sort(nums, mid, r, temp);
    int p = l, q = mid, i = l;
    while (p < mid || q < r) //pq表示左右数组的开头，这个||有意思，可以完全把数据转到新的temp中。
    {
        if (q >= r || (p < mid && nums[p] <= nums[q])) //这个q >= r有意思，表明右边数组已经取完。
        {
            temp[i++] = nums[p++]; //左边的循环
        }
        else
        {
            temp[i++] = nums[q++]; //右边循环
        }
    }
    for (i = l; i < r; ++i)
    {
        nums[i] = temp[i];
    }
}

void merge_sort(std::vector<int> &vec)
{
    if (vec.size() == 1)
    {
        return;
    }
    int mid = vec.size() / 2;
    std::vector<int> sub_vec1;
    std::vector<int> sub_vec2;
    for (int i = 0; i < mid; ++i)
    {
        sub_vec1.push_back(vec[i]);
    }
    for (int i = mid; i < vec.size(); ++i)
    {
        sub_vec2.push_back(vec[i]);
    }
    merge_sort(sub_vec1);
    merge_sort(sub_vec2);
    vec.clear();
    merge_sort_two_vec(sub_vec1, sub_vec2, vec); //子问题合并
}