#pragma once

#include <stddef.h>             // size_t

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

inline void selection_sort(int *a, const size_t n)
{
    for (size_t i = 0; i < n; i++) {
        int x = i; // find the index beyond i with a lower value than i
        for (size_t j = i + 1; j < n; j++)
            if (a[j] < a[x]) x = j;
        qptr_swap(&a[i], &a[x]);
    }
}
