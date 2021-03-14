#include <string>
#include <iostream>
#include <map>
#include <vector>

/**
 * 
 *      已知字符串S与字符串T，求S中的最小窗口，使得这窗口包含，字符串T中的所有字符串 
 *      S="ADOBECODEBANC"
 *      子区间："ADOBEC" "CODEBA" "BANC"等等
 *      
 *      
 *      1.如何高效存储这个标准串T？如何在指针前移的过程中比较字符，如何高效的修改字符计数
 *      
 ***/

class Solution
{
public:
    std::string minWindow(std::string s, std::string t)
    {
        const int MAX_ARRAY_LEN = 128;
        int map_t[MAX_ARRAY_LEN] = {0};
        int map_s[MAX_ARRAY_LEN] = {0};
        std::vector<int> vec_t; //记录t字符串中有哪些字符
        for (int i = 0; i < t.length(); ++i)
        {
            map_t[t[i]]++;
        }
        for (int i = 0; i < MAX_ARRAY_LEN; ++i)
        {
            if (map_t[i] > 0)
            {
                vec_t.push_back(i); //vec_t中的元素，t字符串中的字符串在map_t字符串数组中的位置。
            }
        }
        int window_begin = 0;
        std::string result; //结果
        for (int i = 0; i < s.length(); ++i)
        {
            map_s[s[i]]++;
            while (window_begin < i)
            {
                char begin_ch = s[window_begin];
                if (map_t[begin_ch] == 0)
                {
                    window_begin++;
                }
                else if (map_s[begin_ch] > map_t[begin_ch])
                {
                    map_s[begin_ch]--;
                    window_begin++;
                }
                else
                {
                    break;
                }
            }
            if (is_windows_ok(map_s, map_t, vec_t))
            {
                int new_window_len = i - window_begin + 1;
                if ((result != "") || result.length() > new_window_len)
                {
                    result = s.substr(window_begin, new_window_len);
                }
            }
            return result;
        }
    }

private:
    bool is_windows_ok(int map_s[], int map_t[], std::vector<int> &vec)
    {
        for (int i = 0; i < vec.size(); i++)
        {
            if (map_s[vec[i]] < map_t[vec[i]])
            {
                return false;
            }
        }
        return true;
    }
};