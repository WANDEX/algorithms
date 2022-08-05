#include <cstddef>  // std::size_t

#include <gtest/gtest.h>
#include "selection_sort.h"

bool selection_sort_test(int a[], const int e[], const std::size_t n)
{
    selection_sort(a, n);
    return memcmp(a, e, n * sizeof(a[0])) == 0;
}

TEST(selection_sort_test, 1)
{
    const std::size_t n = 3;
    int a[n] = {7, 5, 6};
    int e[n] = {5, 6, 7}; // E - stands for Expected after sort
    EXPECT_TRUE(selection_sort_test(a, e, n));
}

TEST(selection_sort_test, 2)
{
    const std::size_t n = 5;
    int a[n] = {0, 3, 5, 1, 4};
    int e[n] = {0, 1, 3, 4, 5};
    EXPECT_TRUE(selection_sort_test(a, e, n));
}

TEST(selection_sort_test, 3)
{
    const std::size_t n = 8;
    int a[n] = {1, 2, 7, 0, 3, 2, 5, 6};
    int e[n] = {0, 1, 2, 2, 3, 5, 6, 7};
    EXPECT_TRUE(selection_sort_test(a, e, n));
}

TEST(selection_sort_test, 4)
{
    const std::size_t n = 10;
    int a[n] = {7, 69, 10, 81, 54, 53, 99, 42, 85, 68};
    int e[n] = {7, 10, 42, 53, 54, 68, 69, 81, 85, 99};
    EXPECT_TRUE(selection_sort_test(a, e, n));
}

TEST(selection_sort_test, 5)
{
    const std::size_t n = 11;
    int a[n] = {50, 84, 43, 4, 47, 45, 62, 72, 93, 6, 1};
    int e[n] = {1, 4, 6, 43, 45, 47, 50, 62, 72, 84, 93};
    EXPECT_TRUE(selection_sort_test(a, e, n));
}

TEST(selection_sort_test, 6)
{
    const std::size_t n = 42; // the meaning of life :)
    int a[n] = {203, 730, 187, 609, 536, 480, 106, 2, 4, 605, 42, 384, 724, 691, 723, 0, 467, 820, 676, 818, 427, 821, 910, 8, 272, 338, 762, 819, 109, 144, 779, 990, 679, 838, 450, 141, 44, 580, 289, 471, 573, 823};
    int e[n] = {0, 2, 4, 8, 42, 44, 106, 109, 141, 144, 187, 203, 272, 289, 338, 384, 427, 450, 467, 471, 480, 536, 573, 580, 605, 609, 676, 679, 691, 723, 724, 730, 762, 779, 818, 819, 820, 821, 823, 838, 910, 990};
    EXPECT_TRUE(selection_sort_test(a, e, n));
}
