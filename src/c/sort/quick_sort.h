#pragma once

#ifndef QPTR_SWAP_P_Q
#define QPTR_SWAP_P_Q
/**
 * swap pointers
 */
inline void qptr_swap(int *p, int *q)
{
    int tp = *p;
    *p = *q;
    *q = tp;
}
#endif // QPTR_SWAP_P_Q

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
        qptr_swap(&a[i], &a[j]);
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
