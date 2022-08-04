#pragma once

#ifndef A_ELEM_SWAP
#define A_ELEM_SWAP
inline void swap(int a[], int i, int j)
{
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}
#endif // A_ELEM_SWAP

/**
 * performs Hoare partition algorithm for quick sort
 */
inline int hoare_partition(int a[], int lo, int hi)
{
    int pivot = a[lo];
    int i = lo - 1, j = hi + 1;
    while (1) {
        do { i++; } while(a[i] < pivot);
        do { j--; } while(a[j] > pivot);
        if (i >= j) return j;
        swap(a, i, j);
    }
}

/**
 * sort interval [lo, hi] in-place recursively.
 * initially: lo = 0, hi = size of array - 1
 */
inline void quick_sort(int a[], int lo, int hi)
{
    if (lo < hi) {
        int split = hoare_partition(a, lo, hi);
        quick_sort(a, lo, split);
        quick_sort(a, split + 1, hi);
    }
}
