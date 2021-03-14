#include <vector>
#include <algorithm>

/**
 *  已知每一个孩子都有需求因子g，每一个糖果都有满足度s，当s >= g的时候每一个孩子才可以。
 *  使用一些糖果满足一些孩子，请问最多可以满足多少孩子？
 *  返回满足的孩子数
 * 
 *  孩子的需求被最小的糖果满足
 * **/

class Solution
{
public:
    int findContentChildren(std::vector<int> &g /*孩子需求*/, std::vector<int> &s /*糖果满足*/)
    {
        std::sort(g.begin(), g.end()); //从小到达排序
        std::sort(s.begin(), s.end());
        int child = 0;  //已经满足了几个孩子
        int cookie = 0; //尝试了几个糖果
        while (child < g.size() && cookie < s.size())
        {
            if (g[child] <= s[cookie])
            {            //孩子的满足因子小于等于糖果的大小时
                child++; //孩子指针后移动
            }
            cookie++; //每一个糖果只尝试一次，如果连最小的都无法满足，就没啥用了
        }
        return child;
    }
};