#pragma once

#include <stddef.h>  // size_t

#ifndef A_ELEM_SWAP
#define A_ELEM_SWAP
inline void swap(int array[], int i, int j)
{
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}
#endif // A_ELEM_SWAP

inline void bubble_sort(int array[], const size_t n)
{
    int sorted = 0;
    while (!sorted) {
        sorted = 1;
        for (size_t i = 1; i < n; ++i) {
            if (array[i] < array[i - 1]) {
                swap(array, i - 1, i);
                sorted = 0;
            }
        }
    }
}
