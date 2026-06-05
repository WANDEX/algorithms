#include "wndx/algo/ds/arb.hpp"

#include "wndx/sane/log.hpp"

#include <fmt/ranges.h>         // fmt::join
#include <gtest/gtest.h>

#include <chrono>
#include <functional>           // std::ref
#include <thread>               // std::thread

using namespace wndx::algo;

class arbTest : public ::testing::Test
{
protected:
    static constexpr std::chrono::nanoseconds m_sleep_ns{ 10'000'000 };

    template<typename T, size_t CAPACITY>
    static void write_th(ds::arb<T, CAPACITY> &arb_) {
        for (T i = 0; i < CAPACITY; i++) {
            arb_.push(i);
        }
    }

    template<typename T, size_t CAPACITY>
    static void read_th(ds::arb<T, CAPACITY> &arb, size_t &acc)
    {
        /// Small sleep is required because thread needs time to start.
        /// Otherwise race condition occurs:
        ///   The read thread finishes work before the write thread!
        std::this_thread::sleep_for(m_sleep_ns);
        while (!arb.empty()) {
            acc += arb.pop();
        }
    }

    template<typename T, size_t CAPACITY, size_t SUM>
    static void accumulate_wr_th()
    {
        size_t acc{ 0 };
        ds::arb<T, CAPACITY> arb{ 0 }; // init with first element is 0, to avoid race condition
        std::thread wth{ write_th<T, CAPACITY>, std::ref(arb) };
        std::thread rth{  read_th<T, CAPACITY>, std::ref(arb), std::ref(acc) };
        wth.join();
        rth.join();
        EXPECT_EQ(acc, SUM);
    }

    template<typename T, size_t CAPACITY, size_t SUM>
    static void accumulate_wr()
    {
        size_t acc{ 0 };
        ds::arb<T, CAPACITY> arb;
        for (T i = 0; i < CAPACITY; i++) {
            arb.push(i);
        }
        while (!arb.empty()) {
            acc += arb.pop();
        }
        EXPECT_EQ(acc, SUM);
    }

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
    accumulate_wr<size_t, 64, 2016>();
}

/// ```python3
/// >>> sum([i for i in range(0, 2048)])
/// 2096128
/// ```
TEST_F(arbTest, test_accumulator_2)
{
    accumulate_wr<size_t, 2048, 2096128>();
}

/// ```python3
/// >>> sum([i for i in range(0, 4096)])
/// 8386560
/// ```
TEST_F(arbTest, test_accumulator_3)
{
    accumulate_wr<size_t, 4096, 8386560>();
}

TEST_F(arbTest, test_accumulator_threads_1)
{
    accumulate_wr_th<size_t, 64, 2016>();
}

TEST_F(arbTest, test_accumulator_threads_2)
{
    accumulate_wr_th<size_t, 2048, 2096128>();
}

TEST_F(arbTest, test_accumulator_threads_3)
{
    accumulate_wr_th<size_t, 4096, 8386560>();
}
