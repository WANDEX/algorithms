#include "UnionFind.hpp"

#include <gtest/gtest.h>

using namespace wndx;

class UnionFindTest : public ::testing::Test
{
};

TEST_F(UnionFindTest, testNumComponents)
{
    ds::UnionFind unf = ds::UnionFind(5);
    ASSERT_EQ(5, unf.components());

    unf.unify(0, 1);
    ASSERT_EQ(4, unf.components());

    unf.unify(1, 0);
    ASSERT_EQ(4, unf.components());

    unf.unify(1, 2);
    ASSERT_EQ(3, unf.components());

    unf.unify(0, 2);
    ASSERT_EQ(3, unf.components());

    unf.unify(2, 1);
    ASSERT_EQ(3, unf.components());

    unf.unify(3, 4);
    ASSERT_EQ(2, unf.components());

    unf.unify(4, 3);
    ASSERT_EQ(2, unf.components());

    unf.unify(1, 3);
    ASSERT_EQ(1, unf.components());

    unf.unify(4, 0);
    ASSERT_EQ(1, unf.components());
}

TEST_F(UnionFindTest, testComponentSize)
{
    ds::UnionFind unf = ds::UnionFind(5);
    ASSERT_EQ(1, unf.componentSize(0));
    ASSERT_EQ(1, unf.componentSize(1));
    ASSERT_EQ(1, unf.componentSize(2));
    ASSERT_EQ(1, unf.componentSize(3));
    ASSERT_EQ(1, unf.componentSize(4));

    unf.unify(0, 1);
    ASSERT_EQ(2, unf.componentSize(0));
    ASSERT_EQ(2, unf.componentSize(1));
    ASSERT_EQ(1, unf.componentSize(2));
    ASSERT_EQ(1, unf.componentSize(3));
    ASSERT_EQ(1, unf.componentSize(4));

    unf.unify(1, 0);
    ASSERT_EQ(2, unf.componentSize(0));
    ASSERT_EQ(2, unf.componentSize(1));
    ASSERT_EQ(1, unf.componentSize(2));
    ASSERT_EQ(1, unf.componentSize(3));
    ASSERT_EQ(1, unf.componentSize(4));

    unf.unify(1, 2);
    ASSERT_EQ(3, unf.componentSize(0));
    ASSERT_EQ(3, unf.componentSize(1));
    ASSERT_EQ(3, unf.componentSize(2));
    ASSERT_EQ(1, unf.componentSize(3));
    ASSERT_EQ(1, unf.componentSize(4));

    unf.unify(0, 2);
    ASSERT_EQ(3, unf.componentSize(0));
    ASSERT_EQ(3, unf.componentSize(1));
    ASSERT_EQ(3, unf.componentSize(2));
    ASSERT_EQ(1, unf.componentSize(3));
    ASSERT_EQ(1, unf.componentSize(4));

    unf.unify(2, 1);
    ASSERT_EQ(3, unf.componentSize(0));
    ASSERT_EQ(3, unf.componentSize(1));
    ASSERT_EQ(3, unf.componentSize(2));
    ASSERT_EQ(1, unf.componentSize(3));
    ASSERT_EQ(1, unf.componentSize(4));

    unf.unify(3, 4);
    ASSERT_EQ(3, unf.componentSize(0));
    ASSERT_EQ(3, unf.componentSize(1));
    ASSERT_EQ(3, unf.componentSize(2));
    ASSERT_EQ(2, unf.componentSize(3));
    ASSERT_EQ(2, unf.componentSize(4));

    unf.unify(4, 3);
    ASSERT_EQ(3, unf.componentSize(0));
    ASSERT_EQ(3, unf.componentSize(1));
    ASSERT_EQ(3, unf.componentSize(2));
    ASSERT_EQ(2, unf.componentSize(3));
    ASSERT_EQ(2, unf.componentSize(4));

    unf.unify(1, 3);
    ASSERT_EQ(5, unf.componentSize(0));
    ASSERT_EQ(5, unf.componentSize(1));
    ASSERT_EQ(5, unf.componentSize(2));
    ASSERT_EQ(5, unf.componentSize(3));
    ASSERT_EQ(5, unf.componentSize(4));

    unf.unify(4, 0);
    ASSERT_EQ(5, unf.componentSize(0));
    ASSERT_EQ(5, unf.componentSize(1));
    ASSERT_EQ(5, unf.componentSize(2));
    ASSERT_EQ(5, unf.componentSize(3));
    ASSERT_EQ(5, unf.componentSize(4));
}

TEST_F(UnionFindTest, testConnectivity)
{
    const int sz = 7;
    ds::UnionFind unf = ds::UnionFind(sz);

    for (int i = 0; i < sz; i++) ASSERT_TRUE(unf.connected(i, i));
    unf.unify(0, 2);
    ASSERT_TRUE(unf.connected(0, 2));
    ASSERT_TRUE(unf.connected(2, 0));

    ASSERT_FALSE(unf.connected(0, 1));
    ASSERT_FALSE(unf.connected(3, 1));
    ASSERT_FALSE(unf.connected(6, 4));
    ASSERT_FALSE(unf.connected(5, 0));

    for (int i = 0; i < sz; i++) ASSERT_TRUE(unf.connected(i, i));
    unf.unify(3, 1);
    ASSERT_TRUE(unf.connected(0, 2));
    ASSERT_TRUE(unf.connected(2, 0));
    ASSERT_TRUE(unf.connected(1, 3));
    ASSERT_TRUE(unf.connected(3, 1));

    ASSERT_FALSE(unf.connected(0, 1));
    ASSERT_FALSE(unf.connected(1, 2));
    ASSERT_FALSE(unf.connected(2, 3));
    ASSERT_FALSE(unf.connected(1, 0));
    ASSERT_FALSE(unf.connected(2, 1));
    ASSERT_FALSE(unf.connected(3, 2));

    ASSERT_FALSE(unf.connected(1, 4));
    ASSERT_FALSE(unf.connected(2, 5));
    ASSERT_FALSE(unf.connected(3, 6));

    for (int i = 0; i < sz; i++) ASSERT_TRUE(unf.connected(i, i));
    unf.unify(2, 5);
    ASSERT_TRUE(unf.connected(0, 2));
    ASSERT_TRUE(unf.connected(2, 0));
    ASSERT_TRUE(unf.connected(1, 3));
    ASSERT_TRUE(unf.connected(3, 1));
    ASSERT_TRUE(unf.connected(0, 5));
    ASSERT_TRUE(unf.connected(5, 0));
    ASSERT_TRUE(unf.connected(5, 2));
    ASSERT_TRUE(unf.connected(2, 5));

    ASSERT_FALSE(unf.connected(0, 1));
    ASSERT_FALSE(unf.connected(1, 2));
    ASSERT_FALSE(unf.connected(2, 3));
    ASSERT_FALSE(unf.connected(1, 0));
    ASSERT_FALSE(unf.connected(2, 1));
    ASSERT_FALSE(unf.connected(3, 2));

    ASSERT_FALSE(unf.connected(4, 6));
    ASSERT_FALSE(unf.connected(4, 5));
    ASSERT_FALSE(unf.connected(1, 6));

    for (int i = 0; i < sz; i++) ASSERT_TRUE(unf.connected(i, i));
    // connect everything
    unf.unify(1, 2);
    unf.unify(3, 4);
    unf.unify(4, 6);

    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            ASSERT_TRUE(unf.connected(i, j));
        }
    }
}

TEST_F(UnionFindTest, testSize)
{
    ds::UnionFind unf = ds::UnionFind(5);
    ASSERT_EQ(unf.size(), 5);
    unf.unify(0, 1);
    unf.find(3);
    ASSERT_EQ(unf.size(), 5);
    unf.unify(1, 2);
    ASSERT_EQ(unf.size(), 5);
    unf.unify(0, 2);
    unf.find(1);
    ASSERT_EQ(unf.size(), 5);
    unf.unify(2, 1);
    ASSERT_EQ(unf.size(), 5);
    unf.unify(3, 4);
    unf.find(0);
    ASSERT_EQ(unf.size(), 5);
    unf.unify(4, 3);
    unf.find(3);
    ASSERT_EQ(unf.size(), 5);
    unf.unify(1, 3);
    ASSERT_EQ(unf.size(), 5);
    unf.find(2);
    unf.unify(4, 0);
    ASSERT_EQ(unf.size(), 5);
}
