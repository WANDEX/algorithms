#pragma once

#include <stddef.h>             // size_t

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
 * Sort the given array using insertion sort.
 * Concept: if array is sorted from [0, i] => insert at i+1.
 */
inline void wndxc_insertion_sort(int *a, const size_t n)
{
    for (size_t i = 1; i < n; i++) {
        for (size_t j = i; j > 0 && a[j] < a[j - 1]; j--)
            wndxc_qptr_swap(&a[j - 1], &a[j]);
    }
}
