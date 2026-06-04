#include "wndx/algo/ds/arb.hpp"

#include "wndx/sane/log.hpp"

#include <fmt/ranges.h>         // fmt::join

#include <gtest/gtest.h>

using namespace wndx::algo;

class arbTest : public ::testing::Test
{
public:
    template<typename T, size_t CAPACITY>
    void dbg_print(ds::arb<T, CAPACITY> const& arb)
    {
        WNDX_LOG(wndx::sane::LL::DBUG, "[{}]\n", fmt::join(arb.begin(), arb.end(), ", "));
    }

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

TEST_F(arbTest, test_push_1)
{
    ds::arb<int, 3> arb;
    EXPECT_TRUE(arb.empty());
    EXPECT_EQ(arb.size(), 0);
    arb.push(3);
    arb.push(4);
    arb.push(5);
    EXPECT_EQ(arb.size(), 3);
}

TEST_F(arbTest, test_push_2)
{
    ds::arb<int, 6> arb{ 0, 1, 2 };
    EXPECT_FALSE(arb.empty());
    EXPECT_EQ(arb.size(), 3);
    arb.push(3);
    arb.push(4);
    arb.push(5);
    EXPECT_EQ(arb.size(), 6);
}

TEST_F(arbTest, test_order_1)
{
    ds::arb<int, 3> arb{ 0, 1, 2 };
    ASSERT_EQ(arb.size(), 3);
    EXPECT_EQ(arb[0], 0);
    EXPECT_EQ(arb[1], 1);
    EXPECT_EQ(arb[2], 2);
    dbg_print<int, 3>(arb);
}

TEST_F(arbTest, test_order_2)
{
    ds::arb<int, 4> arb{ 0, 1, 2, 3 };
    ASSERT_EQ(arb.size(), 4);
    EXPECT_EQ(arb[0], 0);
    EXPECT_EQ(arb[1], 1);
    EXPECT_EQ(arb[2], 2);
    EXPECT_EQ(arb[3], 3);
    dbg_print<int, 4>(arb);
}

TEST_F(arbTest, test_wrap_1)
{
    ds::arb<int, 3> arb{ 0, 1, 2 };
    EXPECT_FALSE(arb.empty());
    EXPECT_EQ(arb.size(), 3);
    arb.push(3);
    EXPECT_EQ(arb[0], 3);
    arb.push(4);
    EXPECT_EQ(arb[1], 4);
    arb.push(5);
    EXPECT_EQ(arb[2], 5);
    EXPECT_EQ(arb.size(), 3);
    dbg_print<int, 3>(arb);
}

TEST_F(arbTest, test_wrap_2)
{
    ds::arb<int, 4> arb{ 0, 1, 2 };
    EXPECT_FALSE(arb.empty());
    EXPECT_EQ(arb.size(), 3);
    arb.push(3);
    EXPECT_EQ(arb.size(), 4);
    EXPECT_EQ(arb[0], 0);
    EXPECT_EQ(arb[1], 1);
    EXPECT_EQ(arb[2], 2);
    EXPECT_EQ(arb[3], 3);
    EXPECT_EQ(arb.size(), 4);
    dbg_print<int, 4>(arb);
}

TEST_F(arbTest, test_wrap_3)
{
    ds::arb<size_t, 4> arb{ 0, 1, 2 };
    EXPECT_FALSE(arb.empty());
    EXPECT_EQ(arb.size(), 3);
    arb.push(3);
    EXPECT_EQ(arb.size(), 4);
    for (size_t i = 0; i < 4; i++) {
        EXPECT_EQ(arb[i], i);
    }
    dbg_print<size_t, 4>(arb);
    for (size_t i = 0, v = i + 4; i < 4; i++, v++) {
        arb.push(v);
        EXPECT_EQ(arb[i], v);
    }
    EXPECT_EQ(arb.size(), 4);
    dbg_print<size_t, 4>(arb);
}

TEST_F(arbTest, test_pop_1)
{
    ds::arb<int, 3> arb{ 0, 1, 2 };
    EXPECT_EQ(arb.empty(), false);
    ASSERT_EQ(arb.pop(),  0);
    ASSERT_EQ(arb.size(), 2);
    ASSERT_EQ(arb.pop(),  1);
    ASSERT_EQ(arb.size(), 1);
    ASSERT_EQ(arb.pop(),  2);
    ASSERT_EQ(arb.size(), 0);
    EXPECT_EQ(arb.empty(), true);
}

TEST_F(arbTest, test_pop_2)
{
    ds::arb<int, 4> arb{ 0, 1, 2, 3 };
    EXPECT_EQ(arb.empty(), false);
    ASSERT_EQ(arb.pop(),  0);
    ASSERT_EQ(arb.size(), 3);
    ASSERT_EQ(arb.pop(),  1);
    ASSERT_EQ(arb.size(), 2);
    ASSERT_EQ(arb.pop(),  2);
    ASSERT_EQ(arb.size(), 1);
    ASSERT_EQ(arb.pop(),  3);
    ASSERT_EQ(arb.size(), 0);
    EXPECT_EQ(arb.empty(), true);
}

/// ```python3
/// >>> sum([i for i in range(0, 64)])
/// 2016
/// ```
TEST_F(arbTest, test_accumulator_1)
{
    static constexpr size_t CAPACITY{ 64 };
    ds::arb<size_t, CAPACITY> arb;
    size_t acc{ 0 };
    for (size_t i = 0, e = 0; i < CAPACITY; i++) {
        arb.push(i);
        e = arb.pop();
        EXPECT_EQ(e, i);
        acc += e;
    }
    EXPECT_EQ(acc, 2016);
}

/// ```python3
/// >>> sum([i for i in range(0, 64, 2)])
/// 992
/// ```
TEST_F(arbTest, test_accumulator_2)
{
    static constexpr size_t CAPACITY{ 64 };
    ds::arb<size_t, CAPACITY> arb;
    for (size_t i = 0; i < CAPACITY; i+=2) {
        arb.push(i);
    }
    size_t acc{ 0 };
    while (!arb.empty()) {
        acc += arb.pop();
    }
    EXPECT_EQ(acc, 992);
}
