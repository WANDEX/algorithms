#pragma once

#include <cstddef>              // std::size_t
#include <cassert>
#include <array>
#include <vector>

namespace wndx {
namespace srt {

namespace /* (anonymous) */ {

/**
 * swap pointers
 */
inline void swap(auto* p, auto* q)
{
    auto tp{ *p }; // tmp ptr
    *p = *q;
    *q = tp;
}

/**
 * @brief performs Hoare partition algorithm for quick sort.
 */
template<typename T>
inline std::size_t
hoare_partition(T *a, const std::size_t lo, const std::size_t hi)
{
    T pivot{ a[lo] };
    std::size_t i{ lo - 1 }, j{ hi + 1 };
    while (true) {
        do { i++; } while(a[i] < pivot);
        do { j--; } while(a[j] > pivot);
        if (i >= j) return j;
        swap(&a[i], &a[j]);
    }
}

/**
 * @brief performs Hoare partition algorithm for quick sort.
 */
template<typename T, std::size_t n>
inline std::size_t
hoare_partition(std::array<T, n> &a, const std::size_t lo, const std::size_t hi)
{
    T pivot{ a[lo] };
    std::size_t i{ lo - 1 }, j{ hi + 1 };
    while (true) {
        do { i++; } while(a[i] < pivot);
        do { j--; } while(a[j] > pivot);
        if (i >= j) return j;
        swap(&a[i], &a[j]);
    }
}

/**
 * @brief performs Hoare partition algorithm for quick sort.
 */
template<typename T>
inline std::size_t
hoare_partition(std::vector<T> &a, const std::size_t lo, const std::size_t hi)
{
    T pivot{ a[lo] };
    std::size_t i{ lo - 1 }, j{ hi + 1 };
    while (true) {
        do { i++; } while(a[i] < pivot);
        do { j--; } while(a[j] > pivot);
        if (i >= j) return j;
        swap(&a[i], &a[j]);
    }
}

} // (anonymous) [internal_linkage]

/**
 * @brief quick sort interval [lo, hi] in-place recursively.
 * initially: lo = 0, hi = size of array - 1.
 */
template<typename T>
inline void quick_sort(T *a, const std::size_t lo, const std::size_t hi)
{
    if (lo >= hi) return;
    assert(a); // XXX
    const std::size_t split{ hoare_partition<T>(a, lo, hi) };
    quick_sort(a, lo, split);
    quick_sort(a, split + 1, hi);
}


/**
 * @brief quick sort interval [lo, hi] in-place recursively.
 * initially: lo = 0, hi = size of array - 1.
 */
template<typename T, std::size_t n>
inline void quick_sort(std::array<T, n> &a, const std::size_t lo, const std::size_t hi)
{
    if (lo >= hi) return;
    static_assert(n != 0); // XXX
    const std::size_t split{ hoare_partition<T>(a, lo, hi) };
    quick_sort(a, lo, split);
    quick_sort(a, split + 1, hi);
}


/**
 * @brief quick sort interval [lo, hi] in-place recursively.
 * initially: lo = 0, hi = size of array - 1.
 */
template<typename T>
inline void quick_sort(std::vector<T> &a, const std::size_t lo, const std::size_t hi)
{
    if (lo >= hi) return;
    assert(!a.empty()); // XXX
    const std::size_t split{ hoare_partition<T>(a, lo, hi) };
    quick_sort(a, lo, split);
    quick_sort(a, split + 1, hi);
}

} // namespace srt
} // namespace wndx
