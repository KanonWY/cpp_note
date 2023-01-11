#ifndef __BASIC_DP_H__
#define __BASIC_DP_H__

#include <iostream>
#include <vector>

// 给定 n 节台阶，每次可以走一步或走两步，求一共有多少种方式可以走完这些台阶。
int climbStairs(int n) {}

// 打家劫社
// 假如你是一个劫匪，并且决定抢劫一条街上的房子，每个房子内的钱财数量各不相同。如果
// 你抢了两栋相邻的房子，则会触发警报机关。求在不触发机关的情况下最多可以抢劫多少钱。
int rob(std::vector<int> &nums);

// 给定一个数组，求这个数组中连续且等差的子数组一共有多少个。
int numberOfArithmeticSlices(std::vector<int> &nums);

// 最小路径和
//  给定一个 m × n大小的非负整数矩阵，
// 求从左上角开始到右下角结束的、经过的数字的和最小的路径。每次只能向右或者向下移动。
int minPathSum(std::vector<std::vector<int>> &grid) { return 0; }

// 每一个位置到0的最近距离
// 给定一个由 0 和 1 组成的二维矩阵，求每个位置到最近的 0 的距离。
// std::vector<std::vector<int>>
// updateMatrix(const std::vector<std::vector<int>> &matrix) {}

#endif