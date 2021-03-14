#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
/**
 *      已知一组数（其中有重复的数），求这组元素中所有子集中，子集中各个元素之和为整数target
 *      的子集，结果中无重复的子集。
 *      
 *      nums[] = [10,1,2,7,6,1,5], target = 8
 *      
 *      [[1,7],[2,6],[1,2,5],[1,1,6]];
 * 
 *          //核心，减枝操作，减小递归迭代次数
 * **/

class Solution
{
public:
    std::vector<std::vector<int>> combinationSum2(std::vector<int> &candidates,
                                                  int target)
    {
        std::vector<int> item;
        std::vector<std::vector<int>> result;
        std::set<std::vector<int>> res_set;
        std::sort(candidates.begin(), candidates.end());
        int sum = 0;
        generate(0, candidates, item, result, res_set, target, sum);
        return result;
    }

private:
    void generate(int i, std::vector<int> &nums,
                  std::vector<int> &item,
                  std::vector<std::vector<int>> &result,
                  std::set<std::vector<int>> &res_set,
                  int target, int &sum)
    {
        if (i >= nums.size() || sum > target)
        {
            return;
        }
        item.push_back(nums[i]);
        sum += nums[i];
        if ((res_set.find(item) == res_set.end()) && (sum  == target))
        {
            result.push_back(item);
            res_set.insert(item);
        }
        generate(i + 1, nums, item, result, res_set, target, sum);
        item.pop_back();
        sum -= nums[i];
        generate(i + 1, nums, item, result, res_set, target, sum);
    }
};

int main()
{
    std::vector<std::vector<int>> result;
    std::vector<int> nums{10, 1, 2, 7, 6, 5,1};

    Solution s;
    result = s.combinationSum2(nums, 8);
    for (auto it : result)
    {
        for (auto i : it)
        {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}