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

inline void bubble_sort(auto a[], const std::size_t n)
{
    bool sorted { false };
    while (!sorted) {
        sorted = true;
        for (std::size_t i = 1; i < n; i++) {
            if (a[i] < a[i - 1]) {
                srt::swap(&a[i - 1], &a[i]);
                sorted = false;
            }
        }
    }
}

template<typename T, const std::size_t n>
inline void bubble_sort(std::array<T, n> &a)
{
    bool sorted { false };
    while (!sorted) {
        sorted = true;
        for (std::size_t i = 1; i < n; i++) {
            if (a[i] < a[i - 1]) {
                srt::swap(&a[i - 1], &a[i]);
                sorted = false;
            }
        }
    }
}

template<typename T>
inline void bubble_sort(std::vector<T> &a)
{
    const std::size_t n = a.size();
    bool sorted { false };
    while (!sorted) {
        sorted = true;
        for (std::size_t i = 1; i < n; i++) {
            if (a[i] < a[i - 1]) {
                srt::swap(&a[i - 1], &a[i]);
                sorted = false;
            }
        }
    }
}

} // namespace srt
