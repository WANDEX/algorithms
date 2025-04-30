#pragma once
// merge sort algorithm using std::vector for the sub-arrays.
// written without VLA (Variable Length Arrays) =>
// VLA are a C99 feature and not supported by MSVC.

#include <array>
#include <cassert>
#include <cstddef>              // std::size_t
#include <vector>

namespace wndx::algo {
namespace srt {

namespace /* (anonymous) */ {

/**
 * @brief merge of merge_sort - sorts sub-arrays, then merges them together.
 *
 * @param  a - input array to sort in place.
 * @param lo - lower bound.
 * @param  m - index middle.
 * @param up - upper bound.
 */
template<typename T>
inline void
merge(T *a, const std::size_t lo, const std::size_t m, const std::size_t up)
{
    std::size_t x{0}, y{0}, z{0};
    std::size_t const lsub{ m - lo + 1 };
    std::size_t const rsub{ up - m };

    std::vector<T> la(lsub);
    std::vector<T> ra(rsub);

    // split input array on the halves
    for (x = 0; x < lsub; x++) la[x] = a[lo + x];
    for (y = 0; y < rsub; y++) ra[y] = a[m + y + 1];

    x = 0; y = 0; z = lo;
    while (x < lsub && y < rsub) {
        if (la[x] <= ra[y])
            a[z++] = la[x++];
        else
            a[z++] = ra[y++];
    }

    while (x < lsub) a[z++] = la[x++];
    while (y < rsub) a[z++] = ra[y++];
}

/**
 * @brief merge of merge_sort - sorts sub-arrays, then merges them together.
 *
 * @param  a - input array to sort in place.
 * @param lo - lower bound.
 * @param  m - index middle.
 * @param up - upper bound.
 */
template<typename T, std::size_t n>
inline void
merge(std::array<T, n> &a, const std::size_t lo, const std::size_t m, const std::size_t up)
{
    std::size_t x{0}, y{0}, z{0};
    std::size_t const lsub{ m - lo + 1 };
    std::size_t const rsub{ up - m };

    std::vector<T> la(lsub);
    std::vector<T> ra(rsub);

    // split input array on the halves
    for (x = 0; x < lsub; x++) la[x] = a[lo + x];
    for (y = 0; y < rsub; y++) ra[y] = a[m + y + 1];

    x = 0; y = 0; z = lo;
    while (x < lsub && y < rsub) {
        if (la[x] <= ra[y])
            a[z++] = la[x++];
        else
            a[z++] = ra[y++];
    }

    while (x < lsub) a[z++] = la[x++];
    while (y < rsub) a[z++] = ra[y++];
}

/**
 * @brief merge of merge_sort - sorts sub-arrays, then merges them together.
 *
 * @param  a - input array to sort in place.
 * @param lo - lower bound.
 * @param  m - index middle.
 * @param up - upper bound.
 */
template<typename T>
inline void
merge(std::vector<T> &a, const std::size_t lo, const std::size_t m, const std::size_t up)
{
    std::size_t x{0}, y{0}, z{0};
    std::size_t const lsub{ m - lo + 1 };
    std::size_t const rsub{ up - m };

    std::vector<T> la(lsub);
    std::vector<T> ra(rsub);

    // split input array on the halves
    for (x = 0; x < lsub; x++) la[x] = a[lo + x];
    for (y = 0; y < rsub; y++) ra[y] = a[m + y + 1];

    x = 0; y = 0; z = lo;
    while (x < lsub && y < rsub) {
        if (la[x] <= ra[y])
            a[z++] = la[x++];
        else
            a[z++] = ra[y++];
    }

    while (x < lsub) a[z++] = la[x++];
    while (y < rsub) a[z++] = ra[y++];
}

} // (anonymous) [internal_linkage]

/**
 * @brief merge sort algorithm.
 *
 * @param   a - input array to sort in place.
 * @param  lo - lower bound.
 * @param  up - upper bound.
 *
 * recursively calls itself to split array until it becomes a single element.
 * sorts sub-arrays, then merges them together using merge function.
 */
template<typename T>
inline void merge_sort(T *a, const std::size_t lo, const std::size_t up)
{
    if (lo >= up) return;
    assert(a); // XXX
    const std::size_t m{ (lo + up) / 2 }; // middle index
    merge_sort(a, lo, m);
    merge_sort(a, m + 1, up);
    merge(a, lo, m, up);
}

/**
 * @brief merge sort algorithm.
 *
 * @param   a - input array to sort in place.
 * @param  lo - lower bound.
 * @param  up - upper bound.
 *
 * recursively calls itself to split array until it becomes a single element.
 * sorts sub-arrays, then merges them together using merge function.
 */
template<typename T, std::size_t n>
inline void merge_sort(std::array<T, n> &a, const std::size_t lo, const std::size_t up)
{
    if (lo >= up) return;
    static_assert(n != 0); // XXX
    const std::size_t m{ (lo + up) / 2 }; // middle index
    merge_sort(a, lo, m);
    merge_sort(a, m + 1, up);
    merge(a, lo, m, up);
}

/**
 * @brief merge sort algorithm.
 *
 * @param   a - input array to sort in place.
 * @param  lo - lower bound.
 * @param  up - upper bound.
 *
 * recursively calls itself to split array until it becomes a single element.
 * sorts sub-arrays, then merges them together using merge function.
 */
template<typename T>
inline void merge_sort(std::vector<T> &a, const std::size_t lo, const std::size_t up)
{
    if (lo >= up) return;
    assert(!a.empty()); // XXX
    const std::size_t m{ (lo + up) / 2 }; // middle index
    merge_sort(a, lo, m);
    merge_sort(a, m + 1, up);
    merge(a, lo, m, up);
}

} // namespace srt
} // namespace wndx::algo
