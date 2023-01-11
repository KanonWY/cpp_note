#include <string>
#include <vector>
#include <set>
#include <iostream>

/**
 *          已知一个字符串，求用该字符串的无重复字符的最长子串的长度。
 *              
 *          最长无重复子串？
 *              双指针？
 * 
 *          最长无重复子序列？
 *                 出现一次存储一次             
 *          
 *  for example:
 *              s="abcabcbb"   ->abc 3
 *              s="bbbbb"      ->bbbbb      1
 *              s="pwwkew"     ->wke      不是pwke
 *              注意子序列与子串的区别？
 *  
 **/
class Solution
{
public:
    int lengthOfLongestSubOrder(std::string s)
    {
        std::set<char> char_set;
        for (auto it : s)
        {
            char_set.insert(it);
        }
        return char_set.size();
    }
};

/**
 *              pwwkewaoool
 *              采用遍历的方式进行，查询
 *              时间复杂度为O(n2)
 *                  
 *      
 * **/

class Solution2
{
public:
    int lengthOfLongestSubString(std::string s)
    {
        int result = 0;
        if (s.size() == 0)
        {
            return result;
        }
        std::set<char> char_set;
        for (size_t i = 0; i < s.size(); ++i) //外指针增加1
        {
            int len = 0;
            for (size_t j = i; j < s.size(); ++j)
            {
                if (char_set.find(s[j]) != char_set.end())
                {
                    break;
                }
                else
                {
                    char_set.insert(s[j]);
                    len++;
                }
            }
            if (result < len)
            {
                result = len;
            }
            char_set.clear();
        }
        return result;
    }
};

/***
 * 
 *          pwwkew
 * 
 *          双指针p1 ，p2 
 *          
 *          处理子串，一般都可以优化O（n2）     ->   O(n)的最优方式？
 *          nlogn 问题一般分治，二叉树，二分的方法。
 * 
 * **/

class Solution3
{
public:
    int lengthOfLongestSubString(std::string s)
    {
        int begin = 0;
        int result = 0;
        std::string word = "";
        int char_map[128] = {0};
        for (int i = 0; i < s.size(); i++) //i指针后移
        {
            char_map[s[i]]++;
            if (char_map[s[i]] == 1)
            {
                word += s[i];
                if (result < word.size())
                {
                    result = word.size();
                }
            }
            else
            {
                while ((begin < i) && (char_map[s[i]] > 1)) //大于1很关键，找到第一个重复的字符
                {
                    char_map[s[begin]]--;
                    begin++;
                }
                //更新word，将word更新为begin与i之间的数据，
                word = "";
                for (int j = begin; j <= i; j++)
                {
                    word += s[j];
                }
            }
        }
        return result;
    }
};

class Solution4
{
public:
    int lengthOfLongestSunString(std::string s)
    {
        int begin = 0;
        std::string word = "";
        int len = 0;
        int char_map[128] = {0};
        for (size_t i = 0; i < s.size(); ++i)
        {
            char_map[s[i]]++;
            if (char_map[s[i]] == 1) //第一次出现
            {
                word += s[i];
                if (word.size() > len)
                {
                    len = word.size();
                }
            }
            else //遇到了重复的
            {
                while ((begin < i) && char_map[s[i]] > 1)
                {
                    begin++;
                    char_map[s[begin]]--;
                }
                word = "";
                for (auto j = begin; j <= i; ++j) //注意这里，<=
                {
                    word += s[j];
                }
            }
        }
        return len;
    }
};

int main_01()
{
    std::string up{"bbbbbbbbbbbbbbbb"};
    Solution s;
    int len = s.lengthOfLongestSubOrder(up);
    std::cout << len << std::endl;
    return 0;
}

int main()
{
    Solution4 s;
    int len = s.lengthOfLongestSunString("pwwkew");
    std::cout << "最长子串 len = " << len << std::endl;
    return 0;
}
