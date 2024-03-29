#include <wndx/common/gen.hpp>

#include <wndx/sort/bubble_sort.hpp>
#include <wndx/sort/insertion_sort.hpp>
#include <wndx/sort/selection_sort.hpp>

#include <wndx/sort/merge_sort.hpp>
#include <wndx/sort/quick_sort.hpp>

#include <gtest/gtest.h>

#include <algorithm>            // std::sort, std::copy_n
#include <array>
#include <cstddef>              // std::size_t
#include <cstdint>              // other types, *_MIN, etc
#include <cstring>              // std::memcmp
#include <vector>

using namespace wndx;

/****************************************************************************
 * sort functions for different value types & containers
 */

template<typename T> // C arrays
std::vector<void (*)(T*, const std::size_t)>
srt_func_c_array {
    &srt::bubble_sort,
    &srt::insertion_sort,
    &srt::selection_sort,
};

template<typename T, std::size_t n> // std::array
std::vector<void (*)(std::array<T, n>&)>
srt_func_std_array {
    &srt::bubble_sort,
    &srt::insertion_sort,
    &srt::selection_sort,
};

template<typename T> // std::vector
std::vector<void (*)(std::vector<T>&)>
srt_func_std_vector {
    &srt::bubble_sort,
    &srt::insertion_sort,
    &srt::selection_sort,
};

/****************************************************************************
 * slightly different arguments are required to call the function
 */

template<typename T> // C arrays
std::vector<void (*)(T*, const std::size_t a, const std::size_t b)>
srt_func_c_array_mq {
    &srt::merge_sort,
    &srt::quick_sort,
};

template<typename T, std::size_t n>
std::vector<void (*)(std::array<T, n>&, const std::size_t a, const std::size_t b)>
srt_func_std_array_mq {
    &srt::merge_sort,
    &srt::quick_sort,
};

template<typename T>
std::vector<void (*)(std::vector<T>&, const std::size_t a, const std::size_t b)>
srt_func_std_vector_mq {
    &srt::merge_sort,
    &srt::quick_sort,
};

/**
 * check that C arrays are equal
 */
template<typename T>
bool c_are_equal(T const* const a, T const* const e, const std::size_t n)
{
    return std::memcmp(a, e, n * sizeof(a[0])) == 0;
}

/****************************************************************************
 * C array random tests
 */

TEST(sort_algos_c_array_random, int)
{
    constexpr std::size_t n{ 9 };
    int a_rnd[n]; int a_exp[n];
    for (const auto &algo : srt_func_c_array<int>) {
        gen::random(a_rnd, n, 0, 1000);
        std::copy_n(a_rnd, n, a_exp);
        std::sort(a_exp, a_exp + n);
        algo(a_rnd, n);
        EXPECT_TRUE(c_are_equal(a_rnd, a_exp, n));
    }
}

TEST(sort_algos_c_array_random, double)
{
    constexpr std::size_t n{ 10 };
    double a_rnd[n]; double a_exp[n];
    for (const auto &algo : srt_func_c_array<double>) {
        gen::random(a_rnd, n, 0., 99.);
        std::copy_n(a_rnd, n, a_exp);
        std::sort(a_exp, a_exp + n);
        algo(a_rnd, n);
        EXPECT_TRUE(c_are_equal(a_rnd, a_exp, n));
    }
}

TEST(sort_algos_c_array_random, negative_int8_t)
{
    constexpr std::size_t n{ 11 };
    constexpr std::int8_t min{ INT8_MIN };
    constexpr std::int8_t max{ 0 };
    std::int8_t a_rnd[n]; std::int8_t a_exp[n];
    for (const auto &algo : srt_func_c_array<std::int8_t>) {
        gen::random(a_rnd, n, min, max);
        std::copy_n(a_rnd, n, a_exp);
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
    constexpr std::size_t n{ 22 };
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
    constexpr std::size_t n{ 23 };
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
    constexpr std::size_t n{ 24 };
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
    constexpr std::size_t n{ 99 };
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
    constexpr std::size_t n{ 100 };
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
    constexpr std::size_t n{ 101 };
    for (const auto &algo : srt_func_std_vector<std::int16_t>) {
        std::vector<std::int16_t> a_rnd = gen::random<std::int16_t>(n, INT16_MIN, 0);
        std::vector<std::int16_t> a_exp = a_rnd;
        std::sort(a_exp.begin(), a_exp.end());
        algo(a_rnd);
        EXPECT_EQ(a_rnd, a_exp);
    }
}

/****************************************************************************
 * merge/quick sort tests.
 * => accepts arguments that differ from other sort functions.
 */

TEST(sort_algos_c_array_random, mq__int)
{
    constexpr std::size_t n{ 55 };
    int a_rnd[n], a_exp[n];
    for (const auto &algo : srt_func_c_array_mq<int>) {
        for (std::size_t i = 1; i < 5; i++) {
            gen::random(a_rnd, n, 0, 1000);
            std::copy_n(a_rnd, n, a_exp);
            std::sort(a_exp, a_exp + n);
            algo(a_rnd, 0, n - 1);
            EXPECT_TRUE(c_are_equal(a_rnd, a_exp, n));
        }
    }
}

TEST(sort_algos_std_vector_random, mq__double)
{
    for (const auto &algo : srt_func_std_vector_mq<double>) {
        for (std::size_t n = 1; n < 33; n+=3) {
            std::vector<double> a_rnd = gen::random<double>(n, -99., 99.);
            std::vector<double> a_exp = a_rnd;
            std::sort(a_exp.begin(), a_exp.end());
            algo(a_rnd, 0, n - 1);
            EXPECT_EQ(a_rnd, a_exp);
        }
    }
}

TEST(sort_algos_std_array_random, mq__negative_int_fast8_t)
{
    constexpr std::size_t n{ 21 };
    for (const auto &algo : srt_func_std_array_mq<std::int_fast8_t, n>) {
        for (int i = 0; i < 5; i++) {
            std::array<std::int_fast8_t, n> a_rnd = gen::random<std::int_fast8_t, n>(INT_FAST8_MIN, 0);
            std::array<std::int_fast8_t, n> a_exp = a_rnd;
            std::sort(a_exp.begin(), a_exp.end());
            algo(a_rnd, 0, n - 1);
            EXPECT_EQ(a_rnd, a_exp);
        }
    }
}
