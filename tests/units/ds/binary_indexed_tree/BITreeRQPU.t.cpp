#include "BITreeRQPU.hpp"

#include "gen.hpp"

#include <gtest/gtest.h>

#include <algorithm>            // std::for_each
#include <cstddef>              // std::size_t
#include <initializer_list>
#include <stdexcept>            // std::invalid_argument
#include <vector>

using namespace wndx;

class BITreeRQPUTest : public ::testing::Test
{
protected:
    const int MIN_RAND_NUM = -1000;
    const int MAX_RAND_NUM = +2000;

    const std::size_t LOOPS = 100;

    const int UNUSED_VAL = -9999;

    virtual void SetUp()
    {}

    int randNum() const
    {
        return gen::srng(MIN_RAND_NUM, MAX_RAND_NUM);
    }

    std::size_t loBound(const std::size_t N) const
    {
        return gen::srng(1uz, N);
    }

    std::size_t upBound(const std::size_t N, const std::size_t low) const
    {
        return gen::srng(low, N);
    }

    void doRandomRangeQuery(std::vector<int> &v, ds::BITreeRQPU &tree) const
    {
        int sum {0};
        std::size_t const N{ v.size() - 1 };

        std::size_t lo{ loBound(N) };
        std::size_t hi{ upBound(N, lo) };

        for (std::size_t k = lo; k <= hi; k++) sum += v.at(k);

        ASSERT_EQ(tree.sum(lo, hi), sum);
    }

};

TEST_F(BITreeRQPUTest, covVirtDtor)
{
    ASSERT_NO_THROW({
        ds::BITreeRQPU *ptr{ new ds::BITreeRQPU() };
        if (ptr) delete ptr;
    });
}

TEST_F(BITreeRQPUTest, testEmptyInitializer)
{
    const std::string exp_err{ "Empty initializer." };

    try {
        ds::BITreeRQPU tree({});
    } catch(std::invalid_argument const &err) {
        ASSERT_EQ(err.what(), std::string(exp_err));
    } catch(...) {
        FAIL() << "Expected std::invalid_argument " + exp_err ;
    }

    try {
        std::vector<int> v;
        ds::BITreeRQPU tree(v);
    } catch(std::invalid_argument const &err) {
        ASSERT_EQ(err.what(), std::string(exp_err));
    } catch(...) {
        FAIL() << "Expected std::invalid_argument " + exp_err ;
    }
}

TEST_F(BITreeRQPUTest, testSumException)
{
    const std::string exp_err{ "Make sure right >= left" };

    ds::BITreeRQPU tree({UNUSED_VAL, 1, 2, 3, 4, 5, 6});

    try {
        tree.sum(2, 1);
    } catch(std::invalid_argument const &err) {
        ASSERT_EQ(err.what(), std::string(exp_err));
    } catch(...) {
        FAIL() << "Expected std::invalid_argument " + exp_err ;
    }
}

TEST_F(BITreeRQPUTest, testGetVal)
{
    std::initializer_list<int> il {UNUSED_VAL, 1, 2, 3, 4, 5, 6};
    std::vector<int> expget {il};
    std::vector<int> expval {0, 1, 3, 3, 10, 5, 11};
    ds::BITreeRQPU tree(il);

    for (std::size_t i = 1; i < il.size(); i++) {
        ASSERT_EQ(tree.get(i), expget.at(i));
    }

    std::size_t i {0};
    for (const int e : tree) {
        ASSERT_EQ(e, expval.at(i++));
    }

    i = 0; // standard library algorithm
    std::for_each(tree.cbegin(), tree.cend(), [&](const int& e) {
        ASSERT_EQ(e, expval.at(i++));
    });
}

TEST_F(BITreeRQPUTest, testIntervalSumPositiveValues)
{
    std::initializer_list<int> il {UNUSED_VAL, 1, 2, 3, 4, 5, 6};
    ds::BITreeRQPU tree(il);

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

TEST_F(BITreeRQPUTest, testIntervalSumNegativeValues)
{
    std::initializer_list<int> il {UNUSED_VAL, -1, -2, -3, -4, -5, -6};
    ds::BITreeRQPU tree(il);

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

TEST_F(BITreeRQPUTest, testIntervalSumNegativeValues2)
{
    std::initializer_list<int> il {UNUSED_VAL, -76871, -164790};
    ds::BITreeRQPU tree(il);

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

TEST_F(BITreeRQPUTest, testRandomizedStaticSumQueries)
{
    std::vector<int> rndm;
    ds::BITreeRQPU tree;

    const int max{ static_cast<int>(LOOPS) };

    for (std::size_t sz = 2; sz <= LOOPS; sz++) {
        rndm = gen::random<int>(sz, 0, max, true);
        tree = ds::BITreeRQPU(rndm);

        for (std::size_t j = 0; j < LOOPS / 10; j++) {
            doRandomRangeQuery(rndm, tree);
        }
    }
}

TEST_F(BITreeRQPUTest, testRandomizedSetSumQueries)
{
    std::vector<int> rndm;
    ds::BITreeRQPU tree;

    const int max{ static_cast<int>(LOOPS) };

    for (std::size_t sz = 2; sz <= LOOPS; sz++) {
        rndm = gen::random<int>(sz, 0, max, true);
        tree = ds::BITreeRQPU(rndm);

        for (std::size_t j = 0; j < LOOPS / 10; j++) {
            const std::size_t index = gen::srng<std::size_t>(1, sz);
            const int randn = randNum();

            rndm[index]  += randn;
            tree.add(index, randn);

            doRandomRangeQuery(rndm, tree);
        }
    }
}

TEST_F(BITreeRQPUTest, testReusability)
{
    const std::size_t size {1000};
    std::vector<int> v;
    v.resize(size); // it is essential to populate vector with default values!
    ds::BITreeRQPU tree(size);

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
