#include <vector>

/**
 * 
 *  已知一个二维数组，其中存储了非负整数，找到从左上角到右下角的一条路径，使得路径上的和最小。
 *  移动过程只能向下或者向右
 *      
 *      1 3 1
 *      1 5 1
 *      4 2 1
 * 
 * **/

class Solution
{
public:
    int minPathSum(std::vector<std::vector<int>> &grid)
    {
        std::vector<std::vector<int>> dp(grid.size(), std::vector<int>(grid[0].size(), 0));
        dp[0][0] = grid[0][0];
        for (int i = 1; i < grid[0].size(); ++i)
        {
            dp[0][i] = dp[0][i - 1] + grid[0][i];
        }
        for (int i = 1; i < grid.size(); ++i)
        {
            dp[i][0] = dp[i - 1][0] + grid[i][0];
            for (int j = 1; j < grid.size(); ++j)
            {
                dp[i][j] = std::min(dp[i][j - 1], dp[i - 1][j]) + grid[i][j];
            }
        }
        return dp[grid.size() - 1][grid[0].size() - 1];
    }
};

/**
 *          下面压缩为一维
 * 
 * 
 * **/
class Solution1
{
public:
    int minPathSum(std::vector<std::vector<int>> &grid)
    {
    }
};