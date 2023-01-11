#include <vector>
#include <iostream>
/**
 * 
 *      已知数组nums，求新数组count，count[i]代表了在nums[i]右侧且比nums[i]小的元素个数。
 * 
 *      nums[] = {1,3,2,19,4,5,7}
 *      count[]= {0,1,0,3,0,0,0}
 * 
 * **/

//方法1，暴力遍历，事件复杂度为n2
//  n + （n-1） + （n-2） ... +1    (1+n)n/2   -> O(n2)
class Solution1
{
public:
    std::vector<int> countSmaller(std::vector<int> &nums)
    {
        std::vector<int> res;
        for (int i = 0; i < nums.size(); ++i)
        {
            int target = 0;
            for (int j = i; j < nums.size(); ++j)
            {
                if (nums[j] < nums[i])
                {
                    target++;
                }
            }
            res.push_back(target);
        }
        return res;
    }
};

//如何优化？    归并的方法，------->   nlogn
class Solution2
{
public:
    std::vector<int> countSmaller(std::vector<int> &nums)
    {
        std::vector<std::pair<int, int>> vec;
        std::vector<int> count;
        for (int i = 0; i < nums.size(); ++i)
        {
            vec.push_back(std::make_pair(nums[i], i));
            count.push_back(0);
        }
        merge_sort(vec, count);
        return count;
    }

private:
    void merge_sort_two_vec(std::vector<std::pair<int, int>> &sub_vec1,
                            std::vector<std::pair<int, int>> &sub_vec2,
                            std::vector<std::pair<int, int>> &vec,
                            std::vector<int> &count)
    {
        int i = 0, j = 0;
        while (i < sub_vec1.size() && j < sub_vec2.size())
        {
            if (sub_vec1[i].first <= sub_vec2[j].first)
            {
                count[sub_vec1[i].second] += j; //核心
                vec.push_back(sub_vec1[i]);
                ++i;
            }
            else
            {
                vec.push_back(sub_vec2[j]);
                ++j;
            }
        }
        for (; i < sub_vec1.size(); i++)
        {
            count[sub_vec1[i].second] += j;
            vec.push_back(sub_vec1[i]);
        }
        for (; j < sub_vec2.size(); j++)
        {
            vec.push_back(sub_vec2[j]);
        }
    }
    void merge_sort(std::vector<std::pair<int, int>> &vec,
                    std::vector<int> &count)
    {
        if (vec.size() < 2)
        {
            return;
        }
        int mid = vec.size() / 2;
        std::vector<std::pair<int, int>> sub_vec1;
        std::vector<std::pair<int, int>> sub_vec2;
        for (int i = 0; i < mid; ++i)
        {
            sub_vec1.push_back(vec[i]);
        }
        for (int i = mid; i < vec.size(); ++i)
        {
            sub_vec2.push_back(vec[i]);
        }
        merge_sort(sub_vec1, count);
        merge_sort(sub_vec2, count);
        vec.clear();
        merge_sort_two_vec(sub_vec1, sub_vec2, vec, count);
    }
};

int main()
{
    std::vector<int> num{5, 2, 6, 1};
    Solution2 Slove;
    std::vector<int> res;
    res = Slove.countSmaller(num);
    for (auto it : res)
    {
        std::cout << it << " ";
    }
    std::cout << std::endl;

    return 0;
}