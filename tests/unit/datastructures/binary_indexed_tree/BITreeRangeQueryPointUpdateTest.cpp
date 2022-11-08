#include "BITreeRangeQueryPointUpdate.hpp"

#include "gen.hpp"

#include <gtest/gtest.h>

#include <cstddef>              // std::size_t
#include <initializer_list>
#include <vector>

using namespace wndx;

class BITreeRangeQueryPointUpdateTest : public ::testing::Test
{
protected:
    const int MIN_RAND_NUM = -1000;
    const int MAX_RAND_NUM = +2000;

    const std::size_t LOOPS   = 100;
    const std::size_t TEST_SZ = 1000;

    const int UNUSED_VAL = -9999;

    virtual void SetUp()
    {}

    int randNum() const
    {
        return gen::srng(MIN_RAND_NUM, MAX_RAND_NUM);
    }

    int loBound(const int N) const
    {
        return gen::srng(1, N);
    }

    int upBound(const int N, const int low) const
    {
        return gen::srng(low, N);
    }

    void doRandomRangeQuery(std::vector<int> v, BITreeRangeQueryPointUpdate tree) const
    {
        int sum {0};
        int N = v.size() - 1;

        int lo {loBound(N)};
        int hi {upBound(N, lo)};

        for (int k = lo; k <= hi; k++) sum += v.at(k);

        ASSERT_EQ(tree.sum(lo, hi), sum);
    }

};

TEST_F(BITreeRangeQueryPointUpdateTest, testIntervalSumPositiveValues)
{
    std::initializer_list<int> il {UNUSED_VAL, 1, 2, 3, 4, 5, 6};
    BITreeRangeQueryPointUpdate tree = BITreeRangeQueryPointUpdate(il);

    ASSERT_EQ(tree.sum(1, 6), 21);
    ASSERT_EQ(tree.sum(1, 5), 15);
    ASSERT_EQ(tree.sum(1, 4), 10);
    ASSERT_EQ(tree.sum(1, 3), 6);
    ASSERT_EQ(tree.sum(1, 2), 3);
    ASSERT_EQ(tree.sum(1, 1), 1);

    ASSERT_EQ(tree.sum(3, 4), 7);
    ASSERT_EQ(tree.sum(2, 6), 20);
    ASSERT_EQ(tree.sum(4, 5), 9);
    ASSERT_EQ(tree.sum(6, 6), 6);
    ASSERT_EQ(tree.sum(5, 5), 5);
    ASSERT_EQ(tree.sum(4, 4), 4);
    ASSERT_EQ(tree.sum(3, 3), 3);
    ASSERT_EQ(tree.sum(2, 2), 2);
    ASSERT_EQ(tree.sum(1, 1), 1);
}

TEST_F(BITreeRangeQueryPointUpdateTest, testIntervalSumNegativeValues)
{
    std::initializer_list<int> il {UNUSED_VAL, -1, -2, -3, -4, -5, -6};
    BITreeRangeQueryPointUpdate tree = BITreeRangeQueryPointUpdate(il);

    ASSERT_EQ(tree.sum(1, 6), -21);
    ASSERT_EQ(tree.sum(1, 5), -15);
    ASSERT_EQ(tree.sum(1, 4), -10);
    ASSERT_EQ(tree.sum(1, 3), -6);
    ASSERT_EQ(tree.sum(1, 2), -3);
    ASSERT_EQ(tree.sum(1, 1), -1);

    ASSERT_EQ(tree.sum(6, 6), -6);
    ASSERT_EQ(tree.sum(5, 5), -5);
    ASSERT_EQ(tree.sum(4, 4), -4);
    ASSERT_EQ(tree.sum(3, 3), -3);
    ASSERT_EQ(tree.sum(2, 2), -2);
    ASSERT_EQ(tree.sum(1, 1), -1);
}

TEST_F(BITreeRangeQueryPointUpdateTest, testIntervalSumNegativeValues2)
{
    std::initializer_list<int> il {UNUSED_VAL, -76871, -164790};
    BITreeRangeQueryPointUpdate tree = BITreeRangeQueryPointUpdate(il);

    for (std::size_t i = 0; i < LOOPS; i++) {
        ASSERT_EQ(tree.sum(1, 1), -76871);
        ASSERT_EQ(tree.sum(1, 1), -76871);
        ASSERT_EQ(tree.sum(1, 2), -241661);
        ASSERT_EQ(tree.sum(1, 2), -241661);
        ASSERT_EQ(tree.sum(1, 2), -241661);
        ASSERT_EQ(tree.sum(2, 2), -164790);
        ASSERT_EQ(tree.sum(2, 2), -164790);
        ASSERT_EQ(tree.sum(2, 2), -164790);
    }
}

TEST_F(BITreeRangeQueryPointUpdateTest, testRandomizedStaticSumQueries)
{
    std::vector<int> rndm;
    BITreeRangeQueryPointUpdate tree;

    for (std::size_t sz = 2; sz <= LOOPS; sz++) {
        rndm = gen::random<int>(sz, 0, LOOPS, true);
        tree = BITreeRangeQueryPointUpdate(rndm);

        for (std::size_t j = 0; j < LOOPS / 10; j++) {
            doRandomRangeQuery(rndm, tree);
        }
    }
}

TEST_F(BITreeRangeQueryPointUpdateTest, testRandomizedSetSumQueries)
{
    std::vector<int> rndm;
    BITreeRangeQueryPointUpdate tree;

    for (std::size_t sz = 2; sz <= LOOPS; sz++) {
        rndm = gen::random<int>(sz, 0, LOOPS, true);
        tree = BITreeRangeQueryPointUpdate(rndm);

        for (std::size_t j = 0; j < LOOPS / 10; j++) {
            const int index = gen::srng(1, sz);
            const int randn = randNum();

            rndm[index]  += randn;
            tree.add(index, randn);

            doRandomRangeQuery(rndm, tree);
        }
    }
}

TEST_F(BITreeRangeQueryPointUpdateTest, testReusability)
{
    const std::size_t size {1000};
    std::vector<int> v;
    v.resize(size); // it is essential to populate vector with default values!
    BITreeRangeQueryPointUpdate tree = BITreeRangeQueryPointUpdate(size);

    for (std::size_t loop = 0; loop < LOOPS; loop++) {
        v.at(0) = UNUSED_VAL;
        for (std::size_t i = 1; i < size; i++) {
            const int randn = randNum();

            v.at(i)   = randn;
            tree.set(i, randn);
        }
        doRandomRangeQuery(v, tree);
    }
}
