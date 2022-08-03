#include <stddef.h> // size_t

/**
 * find the index of value in sorted array! O(log(n))
 */
int binary_search(const int *array, const size_t n, const int value)
{
    size_t high = n - 1, mid = 0, low = 0;
    while (low <= high) {
        mid = (low + high) / 2;
        if (array[mid] == value) return mid;
        else if (array[mid] < value) low  = mid + 1;
        else if (array[mid] > value) high = mid - 1;
    }
    return -1; // Value not found
}
