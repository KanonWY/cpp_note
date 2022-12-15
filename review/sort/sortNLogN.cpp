#include <vector>

// 分治思想

// O(n)   找K大的元素

// 归并，将数据分为两个部分，然后将数据合并在一起。

void MergSort(std::vector<int> &nums, int left, int right,
              std::vector<int> &tmp) {
    if (left + 1 >= right) {
        return;
    }
    int mid = left + (right - 1) / 2;
    MergSort(nums, left, mid, tmp);
    MergSort(nums, mid, right, tmp);
    int p = left, q = mid, i = left;
    while (p < mid || q < right) {
        if (q >= right || (p < mid && nums[p] < nums[q])) {
            tmp[i++] = nums[p++];
        } else {
            tmp[i++] = nums[q++];
        }
    }
    for (i = left; i < right; ++i) {
        nums[i] = tmp[i];
    }
}

// T(a) = T(b) + T(c) + K
// k =为合并的时间消耗
// T(1) = C;
// T(n) = 2*T(n/2) + n;

// nlogn

// 不是原地排序算法
// 空间复杂度O(n)
// 稳定

// 快排

// 分区思想
// 从p->q中选择一个数据座位分区点 pivot
// 那么将大于该分区点的数据放右边，小于分区点的放左边。

//[p -> pivot - 1] - [pivot] - [pivot + 1 -> q]

// 递推公式:
// quick_sort(p...r) = quick_sort(p, q-1) + quick_sort(q+1, r)
// 终止条件，p >= r

void quick_sort(std::vector<int> &nums, int left, int right) {
    if (left + 1 >= right) {
        return;
    }
    int first = left, last = right - 1, pirot = nums[first];
    while (first < last) {
    }
}