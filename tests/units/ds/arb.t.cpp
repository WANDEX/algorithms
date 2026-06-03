#include "wndx/algo/ds/arb.hpp"

#include <gtest/gtest.h>

#include <stdexcept>            // std::runtime_error
#include <string>

using namespace wndx::algo;

class arbTest : public ::testing::Test
{
};

TEST_F(arbTest, test_empty)
{
    ds::arb<int, 0> arb{};
    EXPECT_TRUE(arb.empty());
    EXPECT_EQ(arb.size(), 0);
}

TEST_F(arbTest, test_init_initializer_list)
{
    ds::arb<int, 3> arb{ 0, 1, 2 };
    EXPECT_FALSE(arb.empty());
    EXPECT_EQ(arb.size(), 3);
}

TEST_F(arbTest, test_init_array_1)
{
    std::array<int, 6> a{ 0, 1, 2, 3, 4, 5 };
    ds::arb<int, 6> arb(a);
    EXPECT_FALSE(arb.empty());
    EXPECT_EQ(arb.size(), 6);
}

TEST_F(arbTest, test_init_array_const_iterator)
{
    std::array<size_t, 10> const a{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    ds::arb<size_t, 10> arb(a.cbegin(), a.cend());
    EXPECT_FALSE(arb.empty());
    EXPECT_EQ(arb.size(), 10);
}

TEST_F(arbTest, test_init_array_iterator)
{
    std::array<size_t, 10> const a{ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
    ds::arb<size_t, 10> arb(a.begin(), a.end());
    EXPECT_FALSE(arb.empty());
    EXPECT_EQ(arb.size(), 10);
}

