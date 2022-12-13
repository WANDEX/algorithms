#pragma once

#include <assert.h>
#include <stddef.h>             // size_t

/**
 * @brief find the index of value in sorted array! O(log(n))
 *
 * @param  sorted_array
 * @param  n - number of elements in array
 * @param  needle - find a needle in a haystack
 * @return n + 1 if not found
 * @return index of needle
 */
size_t binary_search(const int *sorted_array, const size_t n, const int needle)
{
    // Zero size arrays are an extension (but still, just in case)
    assert(n != 0); // XXX LCOV_EXCL_LINE (what is & why: hidden 0 branch?)
    size_t hi = n, lo = 0;
    size_t mid = (lo + hi) / 2;
    while (mid <= n && lo <= hi) {
        if (sorted_array[mid] == needle)
            return mid;
        if (sorted_array[mid] <  needle)
            lo = mid + 1;
        else
            hi = mid - 1;
        mid = (lo + hi) / 2;
    }
    return n + 1; // Not Found
}
