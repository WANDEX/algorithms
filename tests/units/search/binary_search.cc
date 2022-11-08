#include <gtest/gtest.h>
#include "binary_search.h"

int binary_search_test(const int *array, const int value, const int eindex)
{
    if (binary_search(array, sizeof(array), value) == eindex)
        return 1;
    return 0;
}

TEST(binary_search_test, 1)
{
    const int array[] = {1, 2, 3, 4, 5, 6, 7, 9, 16, 17};
    EXPECT_TRUE(binary_search_test(array, 3, 2));
}

TEST(binary_search_test, 2)
{
    const int array[] = {1, 2, 3, 4, 5, 6, 7, 9, 16, 17};
    EXPECT_TRUE(binary_search_test(array, 8, -1));
}

TEST(binary_search_test, 3)
{
    const int array[] = {1, 2, 3, 4, 5, 6, 7, 9, 16, 17};
    EXPECT_TRUE(binary_search_test(array, 9, 7));
}
