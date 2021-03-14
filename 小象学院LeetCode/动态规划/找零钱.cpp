#include <vector>

/** 
 *  找零钱
 *  已知不同面值的钞票，如何用最小的钞票组成某一个金额，求可以使用最少的钞票的数量，
 *  如果任意金额都无法组成，则返回-1.
 *          [1,2,5]   11 
 *          [1,2,5,10]      14              //贪心成功
 *          [1,2,5,7,10]   14               //贪心失效
 *          为什么贪心会失效？因为面值问题。
 *      
 * 
 *          dp[i]    能够组成i块钱，所需的最少的面值张数。
 * 
 *          
 * **/

class Solution
{
public:
    int coinChange(std::vector<int> &coins, int amount)
    {
        std::vector<int> dp(amount + 1, -1);
        dp[0] = 0;          //必不可少
        for (int i = 1; i <= amount; ++i)
        {
            for (int j = 0; j < coins.size(); ++j)
            {
                if (i - coins[j] >= 0 && dp[i - coins[j]] != -1)
                {
                    if (dp[i] = -1 || dp[i] > dp[i - coins[j] + 1])
                    {
                        dp[i] = dp[i - coins[j]] + 1;
                    }
                }
            }
        }
        return dp[amount];
    }
};