#include <vector>

/**
 *     在一个直线上，有n个房子，房中有数量不等的财宝，由于房屋中有报警器，
 *      相邻的房间中盗取财宝就会触发报警器，在不触发报警器的前提下，最多可以盗取多少财宝。     
 * 
 *          [1,3,5,7,8,9,10]
 *          dp[i]表示盗取i个房间后所获取的最大财宝
 *          dp[0] = nums[0];
 *          dp[1] = std::max(num[0],num[1]);
 *          dp[i] = 是否盗取第i个房间 
 *          如果盗取第i个房间，则一定不能盗取i-1个房间，dp[i-2] + arr[i]
 *          如果不盗取第i个房间，则dp[i-1]
 * **/

class Solution
{
public:
    int rob(std::vector<int> &nums)
    {
        if (nums.size() == 0)
        {
            return 0;
        }
        if (nums.size() == 1)
        {
            return nums[0];
        }
        if (nums.size() == 2)
        {
            return std::max(nums[0], nums[1]);
        }
        std::vector<int> dp(nums.size(), 0);
        dp[0] = nums[0];
        dp[1] = std::max(nums[0], nums[1]);
        for (int i = 2; i < nums.size(); ++i)
        {
            dp[i] = std::max(dp[i - 1], dp[i - 2] + nums[i]);
        }
        return dp[nums.size() - 1];
    }
};