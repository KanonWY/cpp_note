#include <vector>

// 执行效率
// 内存消耗
// 稳定性

// 冒泡排序
void bubleSort(std::vector<int> &arr) {}

// 插入排序：
// 左边为已排序区，右边为未排序区
// 从未排序区放入一个元素到已经排序区，始终保持未排序区是有序的。

// 原地排序？ yes
// 稳定吗？  yes
// 最好时间复杂度，O(n)
// 最坏时间复杂度，O(n2)
// 平均时间复杂度, O(n2)

void insertSort(std::vector<int> &arr) {
    int i = 0, j = 0;
    for (i = 1; i < arr.size(); ++i) {
        for (j = i - 1; j >= 0; --j) {
            if (arr[i] < arr[j]) {
                arr[j + 1] = arr[j];
            } else {
                break;
            }
        }
        arr[j + 1] = arr[i];
    }
}

// 也区分为未排序区和已排序区，
// 但是会从未排序区找最小的值，放到已排序区的末尾
//  | j | | | | | | | | | |

// 原地？ yes
// 稳定？ no  5 8 5 2 8          2 8 5 5 8
// O(n2) O(n2) O(n2)

void selectSort(std::vector<int> &arr) {
    for (int i = 0; i < arr.size() - 1; ++i) {
        int MinIndex = i;
        for (int j = i + 1; j < arr.size() - 1; ++j) {
            if (arr[j] < MinIndex) {
                MinIndex = j;
            }
        }
        std::swap(arr[i], arr[MinIndex]);
    }
}

int main() { return 0; }