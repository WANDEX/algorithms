#pragma once

#include <cstddef>  // std::size_t
#include <array>
#include <vector>

namespace srt {

#ifndef SRT_SWAP_P_Q
#define SRT_SWAP_P_Q
/**
 * swap pointers
 */
inline void swap(int *p, int *q)
{
    int tp { *p }; // tmp ptr
    *p = *q;
    *q = tp;
}
#endif // SRT_SWAP_P_Q

/**
 * Sort the given array using insertion sort.
 * Concept: if array is sorted from [0, i] => insert at i+1.
 */
inline void insertion_sort(auto a[], const std::size_t n)
{
    for (std::size_t i = 1; i < n; i++) {
        for (std::size_t j = i; j > 0 && a[j] < a[j - 1]; j--)
            srt::swap(&a[j - 1], &a[j]);
    }
}

template<typename T, const std::size_t n>
inline void insertion_sort(std::array<T, n> &a)
{
    for (std::size_t i = 1; i < n; i++) {
        for (std::size_t j = i; j > 0 && a[j] < a[j - 1]; j--)
            srt::swap(&a[j - 1], &a[j]);
    }
}

template<typename T>
inline void insertion_sort(std::vector<T> &a)
{
    for (std::size_t i = 1; i < a.size(); i++) {
        for (std::size_t j = i; j > 0 && a[j] < a[j - 1]; j--)
            srt::swap(&a[j - 1], &a[j]);
    }
}

} // namespace srt
