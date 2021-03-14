#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>

class Solution
{
public:
    std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string> &strs)
    {
        std::vector<std::vector<std::string>> result;
        std::map<std::string, std::vector<std::string>> str_map;
        std::string temp_str;
        for (int i = 0; i < strs.size(); ++i)
        {
            temp_str = strs[i];
            std::sort(temp_str.begin(), temp_str.end());
            if (str_map.find(temp_str) == str_map.end())
            {
                str_map[temp_str] = {strs[i]};
            }
            else //find in map
            {
                str_map[temp_str].push_back(strs[i]);
            }
        }
        for (auto it : str_map)
        {
            result.push_back(it.second);
        }
        return result;
    }
};

class Solution2
{
public:
    std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string> &strs)
    {
        std::map<std::vector<int>, std::vector<std::string>> int_map;
        std::vector<std::vector<std::string>> result;
        for (int i = 0; i < strs.size(); ++i)
        {
            std::vector<int> vec;
            change_str_to_vector(strs[i], vec);
            if (int_map.find(vec) == int_map.end())
            {
                std::vector<std::string> item;
                int_map[vec] = item;
            }
            int_map[vec].push_back(strs[i]);
        }
        for (auto it : int_map)
        {
            result.push_back(it.second);
        }
        return result;
    }

private:
    void change_str_to_vector(std::string &str, std::vector<int> &vec)
    {
        for (int i = 0; i < 26; ++i)
        {
            vec.push_back(0);
        }
        for (int i = 0; i < str.size(); ++i)
        {
            vec[str[i] - 'a']++;
        }
    }
};

int main()
{
    std::vector<std::vector<std::string>> res;
    std::vector<std::string> ub{"eat", "tea", "tan", "ate", "nat", "bat"};
    Solution2 s;
    res = s.groupAnagrams(ub);
    for (auto it : res)
    {
        for (auto item : it)
        {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}