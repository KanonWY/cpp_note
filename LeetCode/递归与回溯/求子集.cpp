#include <vector>
#include <set>
#include <algorithm>
/**
 *  已知一组数（其中无重复的元素），求这组数组可以组成的所有子集。结果中不可有无重复的子集。
 *          
 *      nums[] = [1,2,3]
 *  
 *      2的n次方。     
 *      
 *       循环构建代码？
 *      [[],[1],[2],[3],[1,2],[1,3],[2,3],[1,2,3]]
 * 
 *      什么是子集？
 *          子集合可以是无序的，不连续的
 *  
 * 
 * **/

void generate_not_full(int i, std::vector<int> &num,
                       std::vector<int> &item, std::vector<std::vector<int>> &result)
{
    if (i >= num.size())
    {
        return;
    }
    item.push_back(num[i]);
    result.push_back(item);
    generate_not_full(i + 1, num, item, result);
}

void generate_full(int i, std::vector<int> &num,
                   std::vector<int> &item, std::vector<std::vector<int>> &result)
{
    if (i >= num.size())
    {
        return;
    }
    item.push_back(num[i]);
    result.push_back(item);
    generate_full(i + 1, num, item, result);
    item.pop_back();
    generate_full(i + 1, num, item, result);
}

class Solution
{
public:
    std::vector<std::vector<int>> subsets(std::vector<int> &nums)
    {
        std::vector<std::vector<int>> result;
        std::vector<int> item;
        generate(0, nums, item, result);
        return result;
    }

private:
    void generate(int i, std::vector<int> &nums,
                  std::vector<int> &item,
                  std::vector<std::vector<int>> &result)
    {
        if (i >= nums.size())
        {
            return;
        }
        item.push_back(nums[i]);
        result.push_back(item);
        generate(i + 1, nums, item, result);
        item.pop_back();
        generate(i + 1, nums, item, result);
    }
};

/**
 *    位运算： 6种
 *      
 *      &           按位与         1 & 1 = 1,1&0 = 0, 0&0=0      
 *      |           按位或         1 |  1= 1，1 | 0 = 1， 0 | 0 = 0
 *      ^            异或          相同为0，不同为1  0^0=0,1^1=0,1^0=1;
 *      ~           每一位都不一样
 *      <<          乘几个2，末尾添0
 *      >>          除几个2，末尾去0
 *      
 * **/

/**
 * 
 *      求无重复的子集，
 *      已知一组数（其中有重复的元素），求这组数可以组成的所有子集。
 *      结果中无重复的子集。
 *          
 * 
 *      nums[2,1,2,2]
 *      [[],[1],[1,2],[1,2,2],[1,2,2,2],[2],[2,2],[2,2,2]]
 * 
 * 
 *      如何去重（核心）？
 *          
 *      常见去重的方式，排序在去重复。
 *      
 *      排序之后，只会重载次序相同的重复，然后使用set进行过滤。
 * 
 * 
 * **/

class Solution2
{
public:
    std::vector<std::vector<int>> subsetWithDup(std::vector<int> &nums)
    {
        std::vector<std::vector<int>> result;
        std::vector<int> item;
        std::set<std::vector<int>> res_set;
        std::sort(nums.begin(), nums.end());
        result.push_back(item);
        generate(0, nums, result, item, res_set);
        return result;
    }

private:
    void generate(int i, std::vector<int> &nums,
                  std::vector<std::vector<int>> &result,
                  std::vector<int> &item,
                  std::set<std::vector<int>> &res_set)
    {
        if (i > nums.size())
        {
            return;
        }
        item.push_back(nums[i]);
        if (res_set.find(item) == res_set.end())
        {
            result.push_back(item);
            res_set.insert(item);
        }
        generate(i + 1, nums, result, item, res_set);
        item.pop_back();
        generate(i + 1, nums, result, item, res_set);
    }
};