#include <string>
#include <map>
#include <vector>
#include <iostream>

/**
 * 
 *      已知字符串pattern与字符串str， 
 *      一一对应， pattern只包含小写字母，str包含小写字母，并且空格隔离
 * 
 *      1.str使用空格划分每一个单词
 *          
 *          一个map存储     str中的word和pattern中的char
 *              std::map<std::string,char>  word_map;    
 *      
 *          流程：
 *         （1）首先设置执行patter的pos，设置有一个临时变量word用于存储空格间隔离的元素
 * 
 *          for循环中：
 *         （2）str向后移动，遇到空格，先判断pos是否达到了末尾，如果是就返回false；
 *         （3）遇到空格说明word获取到了一个完整的string，此时判断该word是否已经存在在
 *          map中，
 *                      如果该word不存在该map中。
 *                                 此时pattern对应的char已经被使用了，return false；
 *                                 此时pattern对应的char没有被使用，将word与对应的pattern[pos]插入到map。
 *                                 设置这个char已经被使用了。（user[128] 对应char_map）
 *                      如果word存在map。
 *                                 判断std::map中的模式是否匹配
 *         （4）清空world，pos后移。
 *          
 *          如果没有遇到‘ ’，就将world加上str[i],
 *             
 *          循环结束，如果pos ！= pattern.length();
 *                  return false;
 * 
 *          
 * **/

class Solution
{
public:
    bool wordPattern(std::string pattern, std::string str)
    {
        std::map<std::string, char> word_map;
        char used[128] = {0}; //已经被映射的pattern字符
        std::string word;     //临时保存拆分出的单词
        int pos = 0;
        str.push_back(' ');
        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] == ' ')
            {
                if (pos == pattern.length())
                {
                    return false;
                }
                if (word_map.find(word) == word_map.end())
                {
                    if (used[pattern[pos]])
                    {
                        return false;
                    }
                    word_map[word] = pattern[pos];
                    used[pattern[pos]] = 1;
                }
                else
                {
                    if (word_map[word] != pattern[pos])
                    {
                        return false;
                    }
                }
                word = "";
                pos++;
            }
            else
            {
                word += str[i];
            }
        }
        if (pos != pattern.length())
        {
            return false;
        }
        return true;
    }
};

/**
 * 
 *      划分str中的空格，存储其中的word元素到vector中。
 *      
 *          
 *         “_kokokok_kokoijiyuyutyyubb_kokoko_koko_”          
 * 
 * 
 * 
 * **/

std::vector<std::string> getString(std::string str)
{
    std::vector<std::string> res;
    std::string word;
    for (int i = 0; i < str.size(); ++i)
    {
        if (str[i] == ' ')
        {
            if (word.size() == 0)
            {
                continue;
            }
            else
            {
                res.push_back(word);
                word = "";
                continue;
            }
        }
        word += str[i];
    }
    return res;
}

int mainTest()
{
    Solution s;
    bool is_ = s.wordPattern("1bbcc", "kanon koko koko dkdk dkdk");
    if (is_ == true)
    {
        std::cout << "匹配成功" << std::endl;
    }
    else
    {
        std::cout << "匹配失败" << std::endl;
    }
    return 0;
}

void send_different()
{
    return 0;
}


int main()
{
    std::vector<std::string> res;
    res = getString(" kokok koko koko ooo oooojiuui ");
    for (auto it : res)
    {
        std::cout << it << std::endl;
        std::cout << "字符串的长度为：" << it.size() << std::endl; 
    }
    return 0;
}