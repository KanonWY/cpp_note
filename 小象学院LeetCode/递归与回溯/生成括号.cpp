#include <vector>
#include <string>

/**
 *      已知n组括号，开发一个程序，生成n组括号的所有的合法的可能
 *          
 *      input:
 *      n = 3
 *      output:
 *      ["((()))",]
 * 
 *      1、如何生成所有的括号？
 *      2、如何合法性筛选？     （1）左括号数量=右括号数；（2）必须先放左括号再放右括号
 *      如何加入限制条件？
 * 
 * **/

//生成所有括号
void generate_all(std::string item, int n, std::vector<std::string> &result)
{
    if (item.size() == 2 * n)
    {
        result.push_back(item);
        return;
    }
    generate_all(item + "(", n, result);
    generate_all(item + ")", n, result);
}


class Solution
{
public:
    std::vector<std::string> generateParenthesis(int n)
    {
        std::vector<std::string> result;
        generate("", n, n, result);
        return result;
    }

private:
    void generate(std::string item, int left, int right,
                  std::vector<std::string> &result)
    {
        if ((left == 0) && (right) == 0)
        {
            result.push_back(item);
            return;
        }
        if (left > 0) //先放左括号
        {
            generate(item + "(", --left, right, result);
        }
        if (right > left) //只有先放左括号之后才能放右括号
        {
            generate(item + ")", left, --right, result);
        }
    }
};