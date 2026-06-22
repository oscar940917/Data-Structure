#ifndef y_sort
#define y_sort
#include <cmath>
#include <vector>
#include <algorithm>
#include <climits>

namespace ySort {

/**
 * Counting Sort
 * @param arr int* ORG array
 * @param n   int size
 * @param min int min
 * @param max int max
 * @warning Unstable Sorting; rebuilt all objects
 */
void sort_counting(int* arr, int n, int min, int max);

/**
 * Complex Sort
 * @param arr int* ORG array
 * @param n   int size
 */
void sort_yyy(int* arr, int n);

};

#endif
