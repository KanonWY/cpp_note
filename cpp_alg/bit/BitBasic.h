#ifndef __BITBASIC_H__
#define __BITBASIC_H__

#include <iostream>
#include <vector>

//给定两个十进制数字，求它们二进制表示的汉明距离（Hammingdistance，即不同位的个数）
int hammingDistance(int x, int y) {
    int diff = x ^ y;
    int res = 0;
    while (diff) {
        res += diff & 1;
        diff >>= 1;
    }
    return res;
}

//给定一个十进制整数，输出它在二进制下的翻转结果。
uint32_t reverseBits(uint32_t n);

//给定一个整数数组，这个数组里只有一个数次出现了一次，其余数字出现了两次，求这个只出现一次的数字。
int singleNumber(std::vector<int> &nums) {
    int res = 0;
    for (int i = 0; i < nums.size(); ++i) {
        res ^= nums[i];
    }
    return res;
}

//给定一个整数，判断它是否是 4 的次方。
bool isPowerOfFour(int n);

int first_bit(int n) {
    int count = 0;
    while (n ^ 1) //相同取0
    {
        n >>= 1;
        count++;
    }
    count--;
    return 1 << count;
}

//给定一个整数数组，这个数组里只有两个数次出现了一次，其余数字出现了两次，求这两个只出现一次的数字。
std::vector<int> GetTwoNumber(const std::vector<int> &nums) {
    std::vector<int> res;

    int M = 0;
    for (int i = 0; i < nums.size(); ++i) {
        M ^= nums[i];
    }
    int B = first_bit(M);

    int X = 0;
    int Y = 0;

    for (int i = 0; i < nums.size(); ++i) {
        if (B & nums[i]) {
            X ^= nums[i];
        } else {
            Y ^= nums[i];
        }
    }
    // std::cout << "X = " << X << " Y = " << Y << std::endl;
    res.push_back(X);
    res.push_back(Y);
    return res;
}

#endif