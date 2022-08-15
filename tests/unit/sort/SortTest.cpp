#include "SortTest.hpp"
#include "Sort.hpp"

#include "print.hpp"

#include <gtest/gtest.h>

#include <cstddef>  // std::size_t

TEST(ins_sort_test, 1)
{
    const std::size_t n = 3;
    int a[n] = {7, 5, 6};
    Sort<int, n> s { a };
    s.sort_ins();
    SortTest<int, n> st { a };
    EXPECT_TRUE(st.sort_test());
}

TEST(ins_sort_test, 2)
{
    const std::size_t n = 5;
    int a[n] = {0, 3, 5, 1, 4};
    Sort<int, n> s { a };
    s.sort_ins();
    SortTest<int, n> st { a };
    EXPECT_TRUE(st.sort_test());
}

