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

inline void selection_sort(auto a[], const std::size_t n)
{
    for (std::size_t i = 0; i < n; i++) {
        std::size_t x = i; // find the index beyond i with a lower value than i
        for (std::size_t j = i + 1; j < n; j++)
            if (a[j] < a[x]) x = j;
        srt::swap(&a[i], &a[x]);
    }
}

template<typename T, const std::size_t n>
inline void selection_sort(std::array<T, n> &a)
{
    for (std::size_t i = 0; i < n; i++) {
        std::size_t x = i; // find the index beyond i with a lower value than i
        for (std::size_t j = i + 1; j < n; j++)
            if (a[j] < a[x]) x = j;
        srt::swap(&a[i], &a[x]);
    }
}

template<typename T>
inline void selection_sort(std::vector<T> &a)
{
    const std::size_t n = a.size();
    for (std::size_t i = 0; i < n; i++) {
        std::size_t x = i; // find the index beyond i with a lower value than i
        for (std::size_t j = i + 1; j < n; j++)
            if (a[j] < a[x]) x = j;
        srt::swap(&a[i], &a[x]);
    }
}

} // namespace srt
