#include <stddef.h> // size_t

#define COUT 0 // NOTE: toggle printing of stdout messages ON/OFF

#if COUT
#include <stdio.h>
#endif

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

int test(const int *array, const int value, const int eindex)
{
    if (binary_search(array, sizeof(array), value) == eindex)
        return 1;
    return 0;
}

int tests()
{
    int pass = 0, fail = 0;
    const int array[] = {1, 2, 3, 4, 5, 6, 7, 9, 16, 17};

    test(array, 3,  2) ? ++pass : ++fail;
    test(array, 8, -1) ? ++pass : ++fail;
    test(array, 9,  7) ? ++pass : ++fail;

#if COUT
    if (!fail)
        printf("[binary_search.c] ALL PASSED: %d\n", pass);
    else
        printf("[PASS]: %d\t[FAIL]: %d\t[TNUM]: %d\n", pass, fail, pass + fail);
#endif
    return fail; // 0 -> all tests are passed, else num of failed tests
}

int main(void)
{
    return tests();
}
