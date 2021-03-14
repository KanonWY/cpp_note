#include <vector>

//插入排序
void insertion_sort(std::vector<int> &nums, int n)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = i; j > 0 && nums[j] < nums[j - 1]; --j)
        {
            std::swap(nums[j], nums[j - 1]);
        }
    }
}

//冒泡排序
/**
 * 
 *  加入标志位
 * 
 * 
 * **/
void bubble_sort(std::vector<int> &nums, int n)
{
    bool swapped;
    for (int i = 1; i < n; ++i)
    {
        swapped = false;
        for (int j = 1; j < n - i + 1; ++j)
        {
            if (nums[j] < nums[j - 1])
            {
                std::swap(nums[j], nums[j - 1]);
                swapped = true;
            }
        }
        if (!swapped)
        {
            break;
        }
    }
}

