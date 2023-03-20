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

inline void wndxc_bubble_sort(int *a, const size_t n)
{
    int sorted = 0;
    while (!sorted) {
        sorted = 1;
        for (size_t i = 1; i < n; i++) {
            if (a[i] < a[i - 1]) {
                wndxc_qptr_swap(&a[i - 1], &a[i]);
                sorted = 0;
            }
        }
    }
}
