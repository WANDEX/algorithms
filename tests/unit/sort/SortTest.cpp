#include "SortTest.hpp"

#include "Sort.hpp"

#include "bubble_sort.hpp"
#include "insertion_sort.hpp"
#include "quick_sort.hpp"
#include "selection_sort.hpp"

#include <gtest/gtest.h>

#include <algorithm>  // std::stable_sort, std::copy_n
#include <array>
#include <cstddef>    // std::size_t
#include <vector>

// NOTE: no quick sort in those lists
// -> accepts arguments that differ from other functions

// functions to call on C arrays
std::vector<void (*)(int*, const std::size_t)> srt_func_carray = {
    &srt::bubble_sort,
    &srt::insertion_sort,
    &srt::selection_sort,
};

// functions to call on std::array
template<typename T, const std::size_t n>
std::vector<void (*)(std::array<T, n>&)> srt_func_std_array = {
    &srt::bubble_sort,
    &srt::insertion_sort,
    &srt::selection_sort,
};

// functions to call on std::vector
template<typename T>
std::vector<void (*)(std::vector<T>&)> srt_func_std_vector = {
    &srt::bubble_sort,
    &srt::insertion_sort,
    &srt::selection_sort,
};

// check that C arrays are equal
bool are_equal(const int* const a, const int* const e, const std::size_t n)
{
    return std::memcmp(a, e, n * sizeof(a[0])) == 0;
}

TEST(sort_algos_c_array, 0)
{
    const std::size_t n = 0;
    const int e[n] = {};

    for (const auto &algo : srt_func_carray) {
        int a[n] = {};
        algo(a, n);
        EXPECT_TRUE(are_equal(a, e, n));
    }
    // quick sort has different arguments
    int a[n] = {};
    srt::quick_sort(a, 0, n);
    EXPECT_TRUE(are_equal(a, e, n));
}

TEST(sort_algos_c_array, 1)
{
    const std::size_t n = 1;
    const int e[n] = {1};

    for (const auto &algo : srt_func_carray) {
        int a[n] = {1};
        algo(a, n);
        EXPECT_TRUE(are_equal(a, e, n));
    }
    // quick sort has different arguments
    int a[n] = {1};
    srt::quick_sort(a, 0, n - 1);
    EXPECT_TRUE(are_equal(a, e, n));
}

TEST(sort_algos_c_array, 2)
{
    const std::size_t n = 5;
    const int e[n] = {0, 1, 3, 4, 5};

    for (const auto &algo : srt_func_carray) {
        int a[n] = {0, 3, 5, 1, 4};
        algo(a, n);
        EXPECT_TRUE(are_equal(a, e, n));
    }
    // quick sort has different arguments
    int a[n] = {0, 3, 5, 1, 4};
    srt::quick_sort(a, 0, n - 1);
    EXPECT_TRUE(are_equal(a, e, n));
}

TEST(sort_algos_c_array, 3)
{
    const std::size_t n = 8;
    const int e[n] = {0, 1, 2, 2, 3, 5, 6, 7};

    for (const auto &algo : srt_func_carray) {
        int a[n] = {1, 2, 7, 0, 3, 2, 5, 6};
        algo(a, n);
        EXPECT_TRUE(are_equal(a, e, n));
    }
    // quick sort has different arguments
    int a[n] = {1, 2, 7, 0, 3, 2, 5, 6};
    srt::quick_sort(a, 0, n - 1);
    EXPECT_TRUE(are_equal(a, e, n));
}

TEST(sort_algos_std_array, 1)
{
    const std::size_t n = 11;
    const std::array<int, n> e = {1, 4, 6, 43, 45, 47, 50, 62, 72, 84, 93};

    for (const auto &algo : srt_func_std_array<int, n>) {
        std::array<int, n> a = {50, 84, 43, 4, 47, 45, 62, 72, 93, 6, 1};
        algo(a);
        EXPECT_EQ(a, e);
    }
    // quick sort has different arguments
    std::array<int, n> a = {50, 84, 43, 4, 47, 45, 62, 72, 93, 6, 1};
    srt::quick_sort(a, 0, n - 1);
    EXPECT_EQ(a, e);
}

TEST(sort_algos_std_vector, 1)
{
    const std::vector<int> e = {7, 10, 42, 53, 54, 68, 69, 81, 85, 99};

    for (const auto &algo : srt_func_std_vector<int>) {
        std::vector<int> a = {7, 69, 10, 81, 54, 53, 99, 42, 85, 68};
        algo(a);
        EXPECT_EQ(a, e);
    }
    // quick sort has different arguments
    std::vector<int> a = {7, 69, 10, 81, 54, 53, 99, 42, 85, 68};
    srt::quick_sort(a, 0, a.size() - 1);
    EXPECT_EQ(a, e);
}
