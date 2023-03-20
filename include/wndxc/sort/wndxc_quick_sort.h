#pragma once

#ifndef WNDXC_QPTR_SWAP_P_Q
#define WNDXC_QPTR_SWAP_P_Q
/**
 * swap pointers
 */
inline void wndxc_qptr_swap(int *p, int *q)
{
    int tp = *p;
    *p = *q;
    *q = tp;
}
#endif // WNDXC_QPTR_SWAP_P_Q

/**
 * performs Hoare partition algorithm for quick sort
 */
inline int wndxc_hoare_partition(int *a, int lo, int hi)
{
    int pivot = a[lo];
    int i = lo - 1, j = hi + 1;
    while (1) {
        do { i++; } while(a[i] < pivot);
        do { j--; } while(a[j] > pivot);
        if (i >= j) return j;
        wndxc_qptr_swap(&a[i], &a[j]);
    }
}

/**
 * sort interval [lo, hi] in-place recursively.
 * initially: lo = 0, hi = size of array - 1
 */
inline void wndxc_quick_sort(int *a, int lo, int hi)
{
    if (lo < hi) {
        int split = wndxc_hoare_partition(a, lo, hi);
        wndxc_quick_sort(a, lo, split);
        wndxc_quick_sort(a, split + 1, hi);
    }
}
