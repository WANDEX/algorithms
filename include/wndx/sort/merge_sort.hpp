#pragma once
// merge sort algorithm without VLA (Variable Length Arrays).
// VLA are a C99 feature and not supported by msvc.

#include <array>
#include <cassert>
#include <cstddef>              // std::size_t
#include <vector>

namespace wndx {
namespace srt {

namespace /* (anonymous) */ {

template<typename T>
inline void
merge(T *A, const std::size_t p, const std::size_t q, const std::size_t r)
{
    std::size_t x{0}, y{0}, z{0};
    std::size_t const lsub{ q - p + 1 };
    std::size_t const rsub{ r - q };

    std::vector<T> la(lsub);
    std::vector<T> ra(rsub);

    // copy separate fragments of the input array into two temporary arrays
    for (x = 0; x < lsub; x++) la[x] = A[p + x];
    for (y = 0; y < rsub; y++) ra[y] = A[q + y + 1];

    x = 0; y = 0; z = p;
    while (x < lsub && y < rsub) {
        if (la[x] <= ra[y])
            A[z++] = la[x++];
        else
            A[z++] = ra[y++];
    }

    while (x < lsub) A[z++] = la[x++];
    while (y < rsub) A[z++] = ra[y++];
}

/**
 * @brief merge of merge sort algorithm.
 * sorts & merges the sorted sub-arrays.
 * (abbreviated as it is a well-known algorithm)
 * @p left_index, @q middle_index, @r right_index
 */
template<typename T, std::size_t n>
inline void
merge(std::array<T, n> &A, const std::size_t p, const std::size_t q, const std::size_t r)
{
    std::size_t x{0}, y{0}, z{0};
    std::size_t const lsub{ q - p + 1 };
    std::size_t const rsub{ r - q };

    std::vector<T> la(lsub);
    std::vector<T> ra(rsub);

    // copy separate fragments of the input array into two temporary arrays
    for (x = 0; x < lsub; x++) la[x] = A[p + x];
    for (y = 0; y < rsub; y++) ra[y] = A[q + y + 1];

    x = 0; y = 0; z = p;
    while (x < lsub && y < rsub) {
        if (la[x] <= ra[y])
            A[z++] = la[x++];
        else
            A[z++] = ra[y++];
    }

    while (x < lsub) A[z++] = la[x++];
    while (y < rsub) A[z++] = ra[y++];
}

template<typename T>
inline void
merge(std::vector<T> &A, const std::size_t p, const std::size_t q, const std::size_t r)
{
    std::size_t x{0}, y{0}, z{0};
    std::size_t const lsub{ q - p + 1 };
    std::size_t const rsub{ r - q };

    std::vector<T> la(lsub);
    std::vector<T> ra(rsub);

    // copy separate fragments of the input array into two temporary arrays
    for (x = 0; x < lsub; x++) la[x] = A[p + x];
    for (y = 0; y < rsub; y++) ra[y] = A[q + y + 1];

    x = 0; y = 0; z = p;
    while (x < lsub && y < rsub) {
        if (la[x] <= ra[y])
            A[z++] = la[x++];
        else
            A[z++] = ra[y++];
    }

    while (x < lsub) A[z++] = la[x++];
    while (y < rsub) A[z++] = ra[y++];
}

} // (anonymous) [internal_linkage]

template<typename T>
inline void merge_sort(T *A, const std::size_t p, const std::size_t r)
{
    if (p >= r) return;
    assert(A); // XXX
    const std::size_t q{ (p + r) / 2 }; // middle index
    merge_sort(A, p, q);
    merge_sort(A, q + 1, r);
    merge(A, p, q, r);
}

/**
 * @brief merge sort algorithm.
 * recursively calls itself to divide array until it becomes a single element,
 * then merges the sorted sub-arrays.
 * @p left_index, @r right_index
 */
template<typename T, std::size_t n>
inline void merge_sort(std::array<T, n> &A, const std::size_t p, const std::size_t r)
{
    if (p >= r) return;
    static_assert(n != 0); // XXX
    const std::size_t q{ (p + r) / 2 }; // middle index
    merge_sort(A, p, q);
    merge_sort(A, q + 1, r);
    merge(A, p, q, r);
}

template<typename T>
inline void merge_sort(std::vector<T> &A, const std::size_t p, const std::size_t r)
{
    if (p >= r) return;
    assert(!A.empty()); // XXX
    const std::size_t q{ (p + r) / 2 }; // middle index
    merge_sort(A, p, q);
    merge_sort(A, q + 1, r);
    merge(A, p, q, r);
}

} // namespace srt
} // namespace wndx
