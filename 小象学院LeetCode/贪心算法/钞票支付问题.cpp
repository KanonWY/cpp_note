/**
 *  有1元、5元、10元、20元、100元、200元的钞票无穷多张，先用这些钞票支付x元
 *  最少需要多少张。
 * **/

#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;


//this way is wrong
class Solution
{
public:
    int coinChange(vector<int> &coins, int amount)
    {
        std::sort(coins.begin(), coins.end(),greater<int>()); //从大到小排序
        int count = 0;
        for (int i = 0; i < coins.size() && amount > 0; ++i)
        {
            int use = amount / coins[i];
            count += use;
            amount = amount % coins[i];
        }
        if (amount == 0)
        {
            return count;
        }
        return -1;
    }
};

int main_01()
{
    Solution s;
    std::vector<int> cion = {1,2,3,4};
    int sum = 11;
    int count = s.coinChange(cion, sum);
    std::cout << count << std::endl;
    return 0;
}

void test()
{
    std::vector<std::vector<int>> he;

}

int main()
{
    std::vector<int> vec = {1,2,3,4};
    for(auto it:vec)
    {
        std::cout << it << std::endl;
    }
    return 0;
}