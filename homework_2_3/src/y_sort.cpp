#include "y_sort.h"
#define LL long long

void ySort::sort_counting(int* arr, int n, int min, int max) {
    std::vector<int> tv(max-min + 1, 0);   // {0}
    for(int i = 0; i < n; ++i)
        ++tv[arr[i]-min];

    for(int i = 0, ai = 0; i < tv.size(); ++i) {
        while (tv[i] > 0) {
            arr[ai] = i + min;
            --tv[i]; ++ai;
        }
    }
};


void ySort::sort_yyy(int* arr, int n) {
    if (!n) return;
    
    // check whether do a quickly
    if (n <= 32) {
        std::sort(arr, arr + n);
        return;
    }

    // analysis             // O(n)
    int min = INT_MAX, max = INT_MIN;
    bool is_decreasing = true, is_increasing = true;
    if (arr[0] < min) min = arr[0];
    if (arr[0] > max) max = arr[0];
    for (int i = 1; i < n; ++i) {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
        if (arr[i-1] > arr[i]) is_increasing = false;   // should a < b
        if (arr[i-1] < arr[i]) is_decreasing = false;   // should a > b

        // mis < k => use merge #TODO
    }

    if (is_increasing) return;
    if (is_decreasing) {
        std::reverse(arr, arr+n);
        return;
    }


    // Counting Sort
    if ((LL)max - min < n) {
        ySort::sort_counting(arr, n, min, max);
        return;
    }

    // Stalin + Cart Sort
    // #TODO
    /// if

    // heap sort byt STD
    std::make_heap(arr, arr + n);
    std::sort_heap(arr, arr + n);
    // , std::greater<int>()
};