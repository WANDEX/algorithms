#include <cstddef>  // std::size_t
#include <array>    // std::array
#include <iostream>

#include "merge_sort.hpp"
#include "gtest/gtest.h"

template <const std::size_t n>
bool merge_sort_test(std::array<int, n> &A, const std::array<int, n> &A_expected)
{
    merge_sort(A, 0, n - 1);
    return (A == A_expected) ? true : false;
}

TEST(merge_sort_test, 1)
{
    const std::size_t n = 3;
    std::array<int, n> A = {7, 5, 6};
    std::array<int, n> E = {5, 6, 7}; // E - stands for Expected after sort
    EXPECT_TRUE(merge_sort_test(A, E));
}

TEST(merge_sort_test, 2)
{
    const std::size_t n = 5;
    std::array<int, n> A = {0, 3, 5, 1, 4};
    std::array<int, n> E = {0, 1, 3, 4, 5};
    EXPECT_TRUE(merge_sort_test(A, E));
}

TEST(merge_sort_test, 3)
{
    const std::size_t n = 8;
    std::array<int, n> A = {1, 2, 7, 0, 3, 2, 5, 6};
    std::array<int, n> E = {0, 1, 2, 2, 3, 5, 6, 7};
    EXPECT_TRUE(merge_sort_test(A, E));
}

TEST(merge_sort_test, 4)
{
    const std::size_t n = 10;
    std::array<int, n> A = {7, 69, 10, 81, 54, 53, 99, 42, 85, 68};
    std::array<int, n> E = {7, 10, 42, 53, 54, 68, 69, 81, 85, 99};
    EXPECT_TRUE(merge_sort_test(A, E));
}

TEST(merge_sort_test, 5)
{
    const std::size_t n = 11;
    std::array<int, n> A = {50, 84, 43, 4, 47, 45, 62, 72, 93, 6, 1};
    std::array<int, n> E = {1, 4, 6, 43, 45, 47, 50, 62, 72, 84, 93};
    EXPECT_TRUE(merge_sort_test(A, E));
}

TEST(merge_sort_test, 6)
{
    const std::size_t n = 42; // the meaning of life :)
    std::array<int, n> A = {203, 730, 187, 609, 536, 480, 106, 2, 4, 605, 42, 384, 724, 691, 723, 0, 467, 820, 676, 818, 427, 821, 910, 8, 272, 338, 762, 819, 109, 144, 779, 990, 679, 838, 450, 141, 44, 580, 289, 471, 573, 823};
    std::array<int, n> E = {0, 2, 4, 8, 42, 44, 106, 109, 141, 144, 187, 203, 272, 289, 338, 384, 427, 450, 467, 471, 480, 536, 573, 580, 605, 609, 676, 679, 691, 723, 724, 730, 762, 779, 818, 819, 820, 821, 823, 838, 910, 990};
    EXPECT_TRUE(merge_sort_test(A, E));
}
