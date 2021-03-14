#include <vector>
#include <map>
#include <string>

/**
 *  已知一个只包括大小写字符的字符串，求该字符串中的字符可以生成的最长回文子串。
 * 
 *           偶数个的字符必定可以组成回文串
 *          奇数需要找到数量最大的最为中间的
 * 
 * **/

class Solution1
{
public:
    int longestPalinedrome(std::string s)
    {
        int char_map[128] = {0};
        int res = 0;
        int flag = 0;
        for (int i = 0; i < s.size(); ++i)
        {
            char_map[s[i]]++;
        }
        for (int i = 0; i < 128; ++i)
        {
            res += char_map[i];
            if (char_map[i] % 2)
            {
                res--;
                flag = 1;
            }
        }
        return res + flag;
    }
};

/**
 *      a     97
 *      A     65
 *      相差  97-65=32
 * **/
void test_assicc()
{
    char c = 'a'; //97
    printf("c = %c\n", c - 26 - 6);
    printf("c = %d\n", c);
}

// class Solution
// {
// public:
//     int longesrPalinedrome(std::string s)
//     {
        
//     }
// };

int main()
{
    Solution1 s;
    int len = s.longestPalinedrome("aaabbbbbccddeeffgg");
    printf("len = %d\n", len);
    return 0;
}