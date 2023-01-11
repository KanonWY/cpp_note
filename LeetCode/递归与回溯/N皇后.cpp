#include <vector>
#include <string>

/**
 *      
 *          将N个皇后摆放到N*N的棋盘中，互不可攻击，有多少中摆放方式，每种摆放方式具体。
 *          
 *          不可攻击，指的是横竖斜不能有相同的。
 *          
 *          （1）棋盘如何表示？             二维数组
 *          （2）棋盘的更新操作             方向数组
 *          （3）
 * 
 * **/
void put_down_the_queen(int x, int y, std::vector<std::vector<int>> &mark)
{
    static const int dx[] = {-1, 1, 0, 0, -1, -1, 1, 1}; //方向数组，必须dx与dy对应
    static const int dy[] = {0, 0, -1, 1, -1, 1, -1, 1};
    mark[x][y] = 1;
    for (int i = 1; i < mark.size(); i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int new_x = x + dx[j] * i; //向八个方向延伸mark.size()-1长度（最多）。
            int new_y = y + dy[j] * i;
            if (new_x >= 0 && new_x < mark.size() && new_y >= 0 && new_y < mark.size())
            {
                mark[new_x][new_y] = 1;
            }
        }
    }
}

class Solution
{
public:
    std::vector<std::vector<std::string>> solveNQueens(int n)
    {
        std::vector<std::vector<std::string>> result;
        std::vector<std::vector<int>> mark;
        std::vector<std::string> location;
        for (int i = 0; i < n; ++i)
        {
            mark.push_back(std::vector<int>(n, 0));
            location.push_back(std::string(n, '.'));
        }
        generate(0, n, location, result, mark);
        return result;
    }

private:
    void generate(int k, int n,
                  std::vector<std::string> &location,
                  std::vector<std::vector<std::string>> &result,
                  std::vector<std::vector<int>> &mark)
    {
        if (k >= n)
        {
            result.push_back(location);
            return;
        }
        for (int i = 0; i < n; ++i)
        {
            if (mark[k][i] == 0)
            {
                auto temp_mark = mark;
                location[k][i] = 'Q';
                put_down_the_queen(k, i, mark);
                generate(k + 1, n, location, result, mark);
                mark = temp_mark;
                location[k][i] = '.';
            }
        }
    }
};