#include <vector>
#include <iostream>
/**
 *          给定一个二维数组，保存了一个数字三角形，求三角形，顶端到底端的数字和最小的路径之和。
 *          限制，每次可以向下走两个相邻的位置
 *          【2】                              2
 *          【3】【4】                          9 10
 *          【6】【5】【1】                     7  6  10        
 *          【4】【10】【8】【1】            
 *            
 *           从上到下思考，还是从下到上思考。
 * **/

class Solution
{
public:
    int minimumTotal(std::vector<std::vector<int>> &triangle)
    {
        std::vector<int> dp{triangle[triangle.size() - 1]};
        std::vector<int> cp = dp;
        for (int i = triangle.size() - 2; i >= 0; --i)
        {
            for (int j = triangle[i].size() - 1; j >= 0; --j) //使用空间压缩了将二维压缩为1维。
            {
                cp[j] = std::min(dp[j], dp[j + 1]) + triangle[i][j];
            }
            dp = cp;
        }
        return dp[0];
    }
};

class Solution2
{
public:
    int minimumTotal(std::vector<std::vector<int>> &triangle)
    {
        if (triangle.size() == 0)
        {
            return 0;
        }
        std::vector<std::vector<int>> dp;
        for (int i = 0; i < triangle.size(); ++i)
        {
            dp.push_back(std::vector<int>());
            for(int j = 0;j < triangle.size();++j)
            {
                dp[i].push_back(0);
            }
        }
        for(int i = 0; i < dp.size(); ++i)
        {
            dp[dp.size() - 1][i] = triangle[dp.size() - 1][i];
        }
        for(int i = dp.size() - 2;i >=0;--i)
        {
            for(int j = 0;j<dp[i].size();++j)
            {
                dp[i][j] = std::min(dp[i+1][j],dp[i+1][j+1])+triangle[i][j];
            }
        }
        return dp[0][0];
    }
};

int main()
{
    std::vector<std::vector<int>> num{{2}, {3, 4}, {6, 5, 1}, {4, 10, 8, 1}};
    Solution s;
    int ret = s.minimumTotal(num);
    std::cout << ret << std::endl;
    return 0;
}