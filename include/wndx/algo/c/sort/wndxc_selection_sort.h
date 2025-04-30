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

inline void wndxc_selection_sort(int *a, const size_t n)
{
    for (size_t i = 0; i < n; i++) {
        int x = i; // find the index beyond i with a lower value than i
        for (size_t j = i + 1; j < n; j++)
            if (a[j] < a[x]) x = j;
        wndxc_qptr_swap(&a[i], &a[x]);
    }
}
