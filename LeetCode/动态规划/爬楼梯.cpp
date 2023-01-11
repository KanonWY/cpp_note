#include <vector>
#include <iostream>
//在爬楼梯的时候，每次向上走一阶台阶或者2阶台阶
//请问n阶楼梯有多少种上楼的走法？

class Solution1{
public:
	int climbStairs(int n)
	{
		if(n == 1 || n == 2)
		{
			return n;
		}
		return climbStairs(n-1) + climbStairs(n-2);
	}
};

class Solution{
public:
	int climbStairs(int n)
	{
		std::vector<int> dp(n+3,0);		//这里为什么要+3?防止越界问题的产生。
		dp[1] = 1;
		dp[2] = 2;
		for(int i = 3; i <= n; ++i)
		{
			dp[i] = dp[i-1] + dp[i-2];
		}
		return dp[n];
	}
};

int main()
{
	Solution s;
	int n = s.climbStairs();
	std::cout << n << std::endl;
	return 0;
}