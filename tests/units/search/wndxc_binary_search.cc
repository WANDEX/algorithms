#include "wndxc_binary_search.h"

#include <gtest/gtest.h>

#include <stddef.h>             // size_t

TEST(binary_search_test, ZeroSizeArray)
{
    const int array[1] = {1};
    // Zero size arrays are an extension (but still, just in case)
    // here we supply fake 0 size (in order to have full branch coverage)
    ASSERT_DEATH(wndxc_binary_search(array, 0, 0), "n != 0");
    ASSERT_DEATH(wndxc_binary_search(array, 0, 1), "n != 0");
}

TEST(binary_search_test, simple)
{
    const size_t n = 1;
    const size_t NF = n + 1; // Not Found
    const int array[n] = {0};
    EXPECT_EQ(wndxc_binary_search(array, n, 0), 0);
    EXPECT_EQ(wndxc_binary_search(array, n, 1), NF);
}

TEST(binary_search_test, OutOfBounds)
{
    const size_t n = 1;
    const size_t NF = n + 1; // Not Found
    const int array[n] = {1};
    ASSERT_EQ(wndxc_binary_search(array, n, 0), NF);
    ASSERT_EQ(wndxc_binary_search(array, n, 1), 0);
}

TEST(binary_search_test, testExaustive)
{
    const int array[] = {1, 2, 3, 4, 5, 6, 7, 9, 16, 17};
    const size_t n = sizeof(array) / sizeof(array[0]);
    const size_t NF = n + 1; // Not Found
    ASSERT_EQ(wndxc_binary_search(array, n, 0), NF);
    EXPECT_EQ(wndxc_binary_search(array, n, 1), 0);
    EXPECT_EQ(wndxc_binary_search(array, n, 2), 1);
    EXPECT_EQ(wndxc_binary_search(array, n, 3), 2);
    EXPECT_EQ(wndxc_binary_search(array, n, 4), 3);
    EXPECT_EQ(wndxc_binary_search(array, n, 5), 4);
    EXPECT_EQ(wndxc_binary_search(array, n, 6), 5);
    EXPECT_EQ(wndxc_binary_search(array, n, 7), 6);
    EXPECT_EQ(wndxc_binary_search(array, n, 8), NF);
    EXPECT_EQ(wndxc_binary_search(array, n, 9), 7);
    EXPECT_EQ(wndxc_binary_search(array, n, 16), 8);
    EXPECT_EQ(wndxc_binary_search(array, n, 17), 9);
    EXPECT_EQ(wndxc_binary_search(array, n, 18), NF);
}

