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
inline void swap(auto* p, auto* q)
{
    auto tp { *p }; // tmp ptr
    *p = *q;
    *q = tp;
}
#endif // SRT_SWAP_P_Q

/**
 * performs Hoare partition algorithm for quick sort
 */
inline int hoare_partition(auto a[], int lo, int hi)
{
    auto pivot = a[lo];
    int i = lo - 1, j = hi + 1;
    while (true) {
        do { i++; } while(a[i] < pivot);
        do { j--; } while(a[j] > pivot);
        if (i >= j) return j;
        srt::swap(&a[i], &a[j]);
    }
}

/**
 * sort interval [lo, hi] in-place recursively.
 * initially: lo = 0, hi = size of array - 1
 */
inline void quick_sort(auto a[], int lo, int hi)
{
    if (lo < hi) {
        int split = srt::hoare_partition(a, lo, hi);
        quick_sort(a, lo, split);
        quick_sort(a, split + 1, hi);
    }
}


template<typename T, const std::size_t n>
inline int hoare_partition(std::array<T, n> &a, int lo, int hi)
{
    T pivot = a[lo];
    int i = lo - 1, j = hi + 1;
    while (true) {
        do { i++; } while(a[i] < pivot);
        do { j--; } while(a[j] > pivot);
        if (i >= j) return j;
        srt::swap(&a[i], &a[j]);
    }
}

template<typename T, const std::size_t n>
inline void quick_sort(std::array<T, n> &a, int lo, int hi)
{
    if (lo < hi) {
        int split = srt::hoare_partition(a, lo, hi);
        quick_sort(a, lo, split);
        quick_sort(a, split + 1, hi);
    }
}


template<typename T>
inline int hoare_partition(std::vector<T> &a, int lo, int hi)
{
    T pivot = a[lo];
    int i = lo - 1, j = hi + 1;
    while (true) {
        do { i++; } while(a[i] < pivot);
        do { j--; } while(a[j] > pivot);
        if (i >= j) return j;
        srt::swap(&a[i], &a[j]);
    }
}

template<typename T>
inline void quick_sort(std::vector<T> &a, int lo, int hi)
{
    if (lo < hi) {
        int split = srt::hoare_partition(a, lo, hi);
        quick_sort(a, lo, split);
        quick_sort(a, split + 1, hi);
    }
}

} // namespace srt
