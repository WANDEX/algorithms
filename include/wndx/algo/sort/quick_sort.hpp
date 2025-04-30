#pragma once

#include <cstddef>              // std::size_t
#include <cassert>
#include <array>
#include <vector>

namespace wndx::algo {
namespace srt {

namespace /* (anonymous) */ {

/**
 * @brief swap pointers.
 */
inline void swap(auto* p, auto* q)
{
    auto tp{ *p }; // tmp ptr
    *p = *q;
    *q = tp;
}

/**
 * @brief performs Hoare partition algorithm for quick sort.
 *
 * @param   a - input array to sort in place.
 * @param  lo - lower bound.
 * @param  up - upper bound.
 */
template<typename T>
inline std::size_t
hoare_partition(T *a, const std::size_t lo, const std::size_t up)
{
    T pivot{ a[lo] };
    std::size_t i{ lo - 1 }, j{ up + 1 };
    while (true) {
        do { i++; } while(a[i] < pivot);
        do { j--; } while(a[j] > pivot);
        if (i >= j) return j;
        swap(&a[i], &a[j]);
    }
}

/**
 * @brief performs Hoare partition algorithm for quick sort.
 *
 * @param   a - input array to sort in place.
 * @param  lo - lower bound.
 * @param  up - upper bound.
 */
template<typename T, std::size_t n>
inline std::size_t
hoare_partition(std::array<T, n> &a, const std::size_t lo, const std::size_t up)
{
    T pivot{ a[lo] };
    std::size_t i{ lo - 1 }, j{ up + 1 };
    while (true) {
        do { i++; } while(a[i] < pivot);
        do { j--; } while(a[j] > pivot);
        if (i >= j) return j;
        swap(&a[i], &a[j]);
    }
}

/**
 * @brief performs Hoare partition algorithm for quick sort.
 *
 * @param   a - input array to sort in place.
 * @param  lo - lower bound.
 * @param  up - upper bound.
 */
template<typename T>
inline std::size_t
hoare_partition(std::vector<T> &a, const std::size_t lo, const std::size_t up)
{
    T pivot{ a[lo] };
    std::size_t i{ lo - 1 }, j{ up + 1 };
    while (true) {
        do { i++; } while(a[i] < pivot);
        do { j--; } while(a[j] > pivot);
        if (i >= j) return j;
        swap(&a[i], &a[j]);
    }
}

} // (anonymous) [internal_linkage]

/**
 * @brief quick sort algorithm.
 *
 * @param   a - input array to sort in place.
 * @param  lo - lower bound.
 * @param  up - upper bound.
 */
template<typename T>
inline void quick_sort(T *a, const std::size_t lo, const std::size_t up)
{
    if (lo >= up) return;
    assert(a); // XXX
    const std::size_t split{ hoare_partition<T>(a, lo, up) };
    quick_sort(a, lo, split);
    quick_sort(a, split + 1, up);
}


/**
 * @brief quick sort algorithm.
 *
 * @param   a - input array to sort in place.
 * @param  lo - lower bound.
 * @param  up - upper bound.
 */
template<typename T, std::size_t n>
inline void quick_sort(std::array<T, n> &a, const std::size_t lo, const std::size_t up)
{
    if (lo >= up) return;
    static_assert(n != 0); // XXX
    const std::size_t split{ hoare_partition<T>(a, lo, up) };
    quick_sort(a, lo, split);
    quick_sort(a, split + 1, up);
}


/**
 * @brief quick sort algorithm.
 *
 * @param   a - input array to sort in place.
 * @param  lo - lower bound.
 * @param  up - upper bound.
 *
 * initially: lo = 0, up = size of array - 1.
 */
template<typename T>
inline void quick_sort(std::vector<T> &a, const std::size_t lo, const std::size_t up)
{
    if (lo >= up) return;
    assert(!a.empty()); // XXX
    const std::size_t split{ hoare_partition<T>(a, lo, up) };
    quick_sort(a, lo, split);
    quick_sort(a, split + 1, up);
}

} // namespace srt
} // namespace wndx::algo
