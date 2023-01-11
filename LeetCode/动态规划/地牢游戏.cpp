#include <vector>
#include <iostream>
/**
 *      已知一个二维数组，左上角代表骑士的位置，右下角代表公主的位置
 *      二位数组中存储整数，正数可以给其实加血，负数代表毒药，可以给
 *      骑士扣除血量，
 *      那么骑士的生命值是多少的时候，才可以保证骑士在行走过程中至少保持
 *      生命值为1.（骑士只能向下或者向右走）
 *      
 *      -2    -3      3
 *      -5    -10     1
 *      10     30    -5
 *                 
 * 
 *      dp[i][j]代表达到右下角至少要多少血量。
 * 
 * **/

class Solution
{
public:
    int calculateMinimumHP(std::vector<std::vector<int>> &dungeon)
    {
        std::vector<std::vector<int>> dp(dungeon.size(), std::vector<int>(dungeon[0].size(), 0));
        int row = dungeon.size();    //行数
        int col = dungeon[0].size(); //列数
        dp[row - 1][col - 1] = std::max(1, 1 - dungeon[row - 1][col - 1]);
        for (int i = col - 2; i >= 0; --i) //
        {
            dp[row - 1][i] = std::max(1, dp[row - 1][i + 1] - dungeon[row - 1][i]);
        }
        for (int j = row - 2; j >= 0; --j)
        {
            for (int i = dp[row - 2].size() - 2; i >= 0; --i)
            {
                int temp = std::min(dp[i][j + 1], dp[i + 1][j]);
                dp[j][i] = std::max(1, temp - dungeon[i][j]);
            }
        }
        return dp[0][0];
    }
};

int main()
{
    Solution s;
    std::vector<std::vector<int>> re{{-2, -3, 3}, {-5, -10, 1}, {10, 30, -5}};
    int sum = s.calculateMinimumHP(re);
    std::cout << sum << std::endl;
    return 0;
}
