#include <vector>
#include <iostream>

/**
 *      什么是子序列？
 *      例如：
 *          【1，3，4，5，6，7】
 *          【1，5，6】 就是其中一个子序列
 *      
 *      题目描述：
 *      已知一个未排序数组，求这个数组最长上升子序列的长度。
 *      【1，3，2，3，1，4】
 *      有很多上升的子序列，【1，3】  【1，3，4】  【1，2，3，4】    
 *      
 *      与找零钱有种类似的感觉，后面一个DP依赖于前面所有的DP。
 *      dp[i]   代表着以nums[i]结尾的上升子序列
 *      
 *      temp = 0;      
 *      for(int j = 0;j < i;++j)
 *      {
 *
 *          if(num[i] > num[j]){
 *              
 *             }     
 *      }
 * 
 * ***/

class Solution
{
public:
    int lengthOfLIS(std::vector<int> &nums)
    {
        std::vector<int> dp(nums.size(), 0);
        dp[0] = 1;
        int maxlen = 1;
        for (int i = 0; i < nums.size(); ++i)
        {
            dp[i] = 1;
            for (int j = 0; j < i; ++j)
            {
                if ((nums[i] > nums[j]) && (dp[j] + 1 > dp[i]))
                {
                    dp[i] = dp[j] + 1;
                }
            }
            if (dp[i] > maxlen)
            {
                maxlen = dp[i];
            }
        }
        return maxlen;
    }
};

class Solution1
{
public:
    int lengthOfLIS(std::vector<int> &nums)
    {
        if (nums.size() == 0)
        {
            return 0;
        }
        std::vector<int> dp(nums.size(), 0);
        dp[0] = 1;
        int LIS = 1;
        for (int i = 1; i < nums.size(); ++i)
        {
            dp[i] = 1; //key
            for (int j = 0; j < i; ++j)
            {
                if ((nums[i] > nums[j]) && (dp[j] + 1 > dp[i]))
                {
                    dp[i] = dp[j] + 1;
                }
            }
            if (LIS < dp[i])
            {
                LIS = dp[i];
            }
        }
        return LIS;
    }
};
/**
 * 
 *  使用栈实现
 * 
 * 
 * **/
class Solution2
{
public:
    int lengthOfLIS(std::vector<int> &nums)
    {

    }
};

int main()
{
    std::vector<int> nums{1, 3, 2, 3, 1, 4};
    Solution s;
    int num = s.lengthOfLIS(nums);
    std::cout << num << std::endl;
    return 0;
}