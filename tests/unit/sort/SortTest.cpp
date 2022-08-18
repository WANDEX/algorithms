#include "SortTest.hpp"

#include "Sort.hpp"

#include "gen.hpp"
#include "bubble_sort.hpp"
#include "insertion_sort.hpp"
#include "quick_sort.hpp"
#include "selection_sort.hpp"

#include <gtest/gtest.h>

#include <algorithm>  // std::sort, std::copy_n
#include <array>
#include <cstdint>    // other types, *_MIN, etc
#include <cstddef>    // std::size_t
#include <vector>

// NOTE: no quick sort in those lists
// -> accepts arguments that differ from other functions

// functions to call on C arrays
template<typename T>
std::vector<void (*)(T*, const std::size_t)> srt_func_c_array = {
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


/**
 * check that C arrays are equal
 */
bool c_are_equal(const auto* const a, const auto* const e, const std::size_t n)
{
    return std::memcmp(a, e, n * sizeof(a[0])) == 0;
}

/****************************************************************************
 * C array random tests
 */

TEST(sort_algos_c_array_random, int)
{
    const std::size_t n = 9;
    for (const auto &algo : srt_func_c_array<int>) {
        int a_rnd[n]; gen::random(a_rnd, n, 0, 1000);
        int a_exp[n]; std::copy_n(a_rnd, n, a_exp);
        std::sort(a_exp, a_exp + n);
        algo(a_rnd, n);
        EXPECT_TRUE(c_are_equal(a_rnd, a_exp, n));
    }
}

TEST(sort_algos_c_array_random, double)
{
    const std::size_t n = 10;
    for (const auto &algo : srt_func_c_array<double>) {
        double a_rnd[n]; gen::random(a_rnd, n, 0, 99);
        double a_exp[n]; std::copy_n(a_rnd, n, a_exp);
        std::sort(a_exp, a_exp + n);
        algo(a_rnd, n);
        EXPECT_TRUE(c_are_equal(a_rnd, a_exp, n));
    }
}

TEST(sort_algos_c_array_random, negative_int8_t)
{
    const std::size_t n = 11;
    for (const auto &algo : srt_func_c_array<std::int8_t>) {
        std::int8_t a_rnd[n]; gen::random(a_rnd, n, INT8_MIN, 0);
        std::int8_t a_exp[n]; std::copy_n(a_rnd, n, a_exp);
        std::sort(a_exp, a_exp + n);
        algo(a_rnd, n);
        EXPECT_TRUE(c_are_equal(a_rnd, a_exp, n));
    }
}

/****************************************************************************
 * std::array random tests
 */

TEST(sort_algos_std_array_random, int)
{
    const std::size_t n = 22;
    for (const auto &algo : srt_func_std_array<int, n>) {
        std::array<int, n> a_rnd = gen::random<int, n>(0, 10);
        std::array<int, n> a_exp = a_rnd;
        std::sort(a_exp.begin(), a_exp.end());
        algo(a_rnd);
        EXPECT_EQ(a_rnd, a_exp);
    }
}

TEST(sort_algos_std_array_random, double)
{
    const std::size_t n = 22;
    for (const auto &algo : srt_func_std_array<double, n>) {
        std::array<double, n> a_rnd = gen::random<double, n>(0, 1000);
        std::array<double, n> a_exp = a_rnd;
        std::sort(a_exp.begin(), a_exp.end());
        algo(a_rnd);
        EXPECT_EQ(a_rnd, a_exp);
    }
}

TEST(sort_algos_std_array_random, negative_int_least_16t)
{
    const std::size_t n = 22;
    for (const auto &algo : srt_func_std_array<std::int_least16_t, n>) {
        std::array<std::int_least16_t, n> a_rnd = gen::random<std::int_least16_t, n>(INT_LEAST16_MIN, 0);
        std::array<std::int_least16_t, n> a_exp = a_rnd;
        std::sort(a_exp.begin(), a_exp.end());
        algo(a_rnd);
        EXPECT_EQ(a_rnd, a_exp);
    }
}

/****************************************************************************
 * std::vector random tests
 */

TEST(sort_algos_std_vector_random, int)
{
    const std::size_t n = 99;
    for (const auto &algo : srt_func_std_vector<int>) {
        std::vector<int> a_rnd = gen::random<int>(n, 0, 99);
        std::vector<int> a_exp = a_rnd;
        std::sort(a_exp.begin(), a_exp.end());
        algo(a_rnd);
        EXPECT_EQ(a_rnd, a_exp);
    }
}

TEST(sort_algos_std_vector_random, double)
{
    const std::size_t n = 100;
    for (const auto &algo : srt_func_std_vector<double>) {
        std::vector<double> a_rnd = gen::random<double>(n, 0, 100);
        std::vector<double> a_exp = a_rnd;
        std::sort(a_exp.begin(), a_exp.end());
        algo(a_rnd);
        EXPECT_EQ(a_rnd, a_exp);
    }
}

TEST(sort_algos_std_vector_random, negative_int16_t)
{
    const std::size_t n = 101;
    for (const auto &algo : srt_func_std_vector<std::int16_t>) {
        std::vector<std::int16_t> a_rnd = gen::random<std::int16_t>(n, INT16_MIN, 0);
        std::vector<std::int16_t> a_exp = a_rnd;
        std::sort(a_exp.begin(), a_exp.end());
        algo(a_rnd);
        EXPECT_EQ(a_rnd, a_exp);
    }
}


/****************************************************************************
 * quick sort tests (no quick sort algorithm in above tests!).
 * => accepts arguments that differ from other sort functions.
 */

TEST(sort_algos_c_array_random, quick_sort__int)
{
    for (std::size_t n = 0; n < 55; n+=11) {
        int a_rnd[n]; gen::random(a_rnd, n, 0, 1000);
        int a_exp[n]; std::copy_n(a_rnd, n, a_exp);
        std::sort(a_exp, a_exp + n);
        srt::quick_sort(a_rnd, 0, n - 1);
        EXPECT_TRUE(c_are_equal(a_rnd, a_exp, n));
    }
}

TEST(sort_algos_std_vector_random, quick_sort__double)
{
    for (std::size_t n = 0; n < 33; n+=3) {
        std::vector<double> a_rnd = gen::random<double>(n, -99, 99);
        std::vector<double> a_exp = a_rnd;
        std::sort(a_exp.begin(), a_exp.end());
        srt::quick_sort(a_rnd, 0, n - 1);
        EXPECT_EQ(a_rnd, a_exp);
    }
}

TEST(sort_algos_std_array_random, quick_sort__negative_int_fast8_t)
{
    constexpr std::size_t n = 21; // => require constexpr size
    for (int i = 0; i < 5; i++) {
        std::array<std::int_fast8_t, n> a_rnd = gen::random<std::int_fast8_t, n>(INT_FAST8_MIN, 0);
        std::array<std::int_fast8_t, n> a_exp = a_rnd;
        std::sort(a_exp.begin(), a_exp.end());
        srt::quick_sort(a_rnd, 0, n - 1);
        EXPECT_EQ(a_rnd, a_exp);
    }
}
