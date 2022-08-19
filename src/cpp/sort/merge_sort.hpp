#pragma once

#include <cstddef>  // std::size_t
#include <array>
#include <vector>

namespace srt {

template<typename T>
inline void merge(T A[], int p, int q, int r)
{
    int x{0}, y{0}, z{0};
    int lsub = q - p + 1;
    int rsub = r - q;
    T la[lsub], ra[rsub];

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
inline void merge_sort(T A[], int p, int r)
{
    if (p >= r) return;
    int q = (p + r) / 2; // middle_index (round down implicitly)
    merge_sort(A, p, q);
    merge_sort(A, q + 1, r);
    srt::merge(A, p, q, r);
}

/**
 * merge of merge sort algorithm.
 * sorts & merges the sorted sub-arrays.
 * (abbreviated as it is a well-known algorithm)
 * @p left_index, @q middle_index, @r right_index
 */
template<typename T, const std::size_t n>
inline void merge(std::array<T, n> &A, int p, int q, int r)
{
    int x{0}, y{0}, z{0};
    int lsub = q - p + 1;
    int rsub = r - q;
    T la[lsub], ra[rsub];

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
 * merge sort algorithm.
 * recursively calls itself to divide array until it becomes a single element,
 * then merges the sorted sub-arrays.
 * @p left_index, @r right_index
 */
template<typename T, const std::size_t n>
inline void merge_sort(std::array<T, n> &A, int p, int r)
{
    if (p >= r) return;
    int q = (p + r) / 2; // middle_index (round down implicitly)
    merge_sort(A, p, q);
    merge_sort(A, q + 1, r);
    srt::merge(A, p, q, r);
}

template<typename T>
inline void merge(std::vector<T> &A, int p, int q, int r)
{
    int x{0}, y{0}, z{0};
    int lsub = q - p + 1;
    int rsub = r - q;
    T la[lsub], ra[rsub];

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
inline void merge_sort(std::vector<T> &A, int p, int r)
{
    if (p >= r) return;
    int q = (p + r) / 2; // middle_index (round down implicitly)
    merge_sort(A, p, q);
    merge_sort(A, q + 1, r);
    srt::merge(A, p, q, r);
}

} // namespace srt
