#include <vector>

/**
 * 
 *      给定一个数组，求这个数组的连续子数组中，最大的那一段的和。
 *      关系词：连续子数组
 *      [-2,1,-1,4,3,4] 
 *          
 * 
 *       暴力枚举：
 *               (n+1)n/2
 *      动态规划；
 *              dp[0]  = num[0];
 *              dp[1] = dp[]
 *              dp[i] 代表以i结尾的最大字段和
 *              dp[i] = max(dp[i-1] + num[i] ,num[i])
 * **/

class Solution
{
public:
    int maxSubArray(std::vector<int> &nums)
    {
        std::vector<int> dp(nums.size(), 0);
        dp[0] = nums[0];
        int max = dp[0];
        for (int i = 1; i < nums.size(); ++i)
        {
            dp[i] = std::max(dp[i - 1] + nums[i], nums[i]);
            if (dp[i] > max)
            {
                max = dp[i];
            }
        }
        return max;
    }
};