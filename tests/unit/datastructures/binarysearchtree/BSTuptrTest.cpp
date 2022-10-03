#include "BSTuptr.hpp"

#include "gen.hpp"
#include "pprint.hpp"

#include <gtest/gtest.h>

#include <algorithm>        // std::for_each
#include <cstddef>          // size_t
#include <initializer_list>
#include <string>
#include <vector>

class BSTuptrTest : public ::testing::Test {
protected:
    const std::size_t LOOPS = 100;

    virtual void SetUp()
    {}

    template<typename T> // this is just a print degug info
    void print_initial(BSTuptr<T> &tree, const std::vector<T> &rndm)
    {
        std::cout << '\n';
        std::cout << "initial =============v" << '\n';
        tree.print(tree.IN_ORDER,    "in  : ");
        tree.print(tree.PRE_ORDER,   "pre : ");
        tree.print(tree.POST_ORDER,  "post: ");
        tree.print(tree.LEVEL_ORDER, "lvl : ");
        pp::print(rndm, "vector sz=" + std::to_string(tree.size()) + "\t");
        std::cout << "initial =============^" << '\n';
    }

    template<typename T>
    void testTraversalOrder
        (
            const std::initializer_list<T> l_i,
            const std::initializer_list<T> l_o,
            const TreeTravOrder order
        )
    {
        BSTuptr tree = BSTuptr<T>(l_i, order);
        const T* it{ l_o.begin() };
        // check size
        ASSERT_EQ(tree.size(), l_o.size());
        // check if the traversal order matches the expected order
        for (const T& e : tree) ASSERT_EQ(e, *(it++));
    }

};

TEST_F(BSTuptrTest, testEmpty)
{
    BSTuptr tree = BSTuptr<std::string>();
    ASSERT_TRUE(tree.isEmpty());
    tree.add("Hello World!");
    ASSERT_FALSE(tree.isEmpty());
}

TEST_F(BSTuptrTest, testSize)
{
    BSTuptr tree = BSTuptr<std::string>();
    ASSERT_EQ(tree.size(), 0);
    tree.add("Hello World!");
    ASSERT_EQ(tree.size(), 1);
}

TEST_F(BSTuptrTest, testAdd)
{
    BSTuptr tree = BSTuptr<char>();
    // Add element which does not yet exist
    ASSERT_TRUE(tree.add('A'));
    // Try to add duplicate element
    ASSERT_FALSE(tree.add('A'));
    // Add a second element which is not a duplicate
    ASSERT_TRUE(tree.add('B'));
}

TEST_F(BSTuptrTest, testContains)
{
    BSTuptr tree = BSTuptr<char>();
    tree.add('B');
    tree.add('A');
    tree.add('C');
    // Try looking for an element which doesn't exist
    ASSERT_FALSE(tree.contains('D'));
    // Try looking for an element which exists in the root
    ASSERT_TRUE(tree.contains('B'));
    // Try looking for an element which exists as the left child of the root
    ASSERT_TRUE(tree.contains('A'));
    // Try looking for an element which exists as the right child of the root
    ASSERT_TRUE(tree.contains('C'));
}

TEST_F(BSTuptrTest, testHeight)
{
    BSTuptr tree = BSTuptr<char>();
    /* Tree should look like:
     *        M
     *      J  S
     *    B   N Z
     *  A
     */
    // no tree
    ASSERT_EQ(tree.height(), 0);
    // layer 1
    tree.add('M');
    ASSERT_EQ(tree.height(), 1);
    // layer 2
    tree.add('J');
    ASSERT_EQ(tree.height(), 2);
    tree.add('S');
    ASSERT_EQ(tree.height(), 2);
    // layer 3
    tree.add('B');
    ASSERT_EQ(tree.height(), 3);
    tree.add('N');
    ASSERT_EQ(tree.height(), 3);
    tree.add('Z');
    ASSERT_EQ(tree.height(), 3);
    // layer 4
    tree.add('A');
    ASSERT_EQ(tree.height(), 4);
}

TEST_F(BSTuptrTest, testRemove)
{
    BSTuptr tree = BSTuptr<char>();
    // Try removing an element which does not exist
    ASSERT_TRUE(tree.add('A'));
    ASSERT_TRUE(tree.contains('A'));
    ASSERT_EQ(tree.size(), 1);
    ASSERT_FALSE(tree.contains('B'));
    ASSERT_FALSE(tree.remove('B'));
    ASSERT_EQ(tree.size(), 1);

    // Try removing an element which does exist
    ASSERT_TRUE(tree.add('B'));
    ASSERT_TRUE(tree.contains('B'));
    ASSERT_EQ(tree.size(), 2);
    ASSERT_TRUE(tree.remove('B'));
    ASSERT_FALSE(tree.contains('B'));
    ASSERT_EQ(tree.size(), 1);
    ASSERT_EQ(tree.height(), 1);

    // Try removing the root
    ASSERT_TRUE(tree.contains('A'));
    ASSERT_TRUE(tree.remove('A'));
    ASSERT_FALSE(tree.contains('A'));
    ASSERT_EQ(tree.size(), 0);
    ASSERT_EQ(tree.height(), 0);
}

TEST_F(BSTuptrTest, testRemoveRandom)
{
    using Ty = int;
    std::vector<Ty> rndm;
    BSTuptr    <Ty> tree;
    for (std::size_t i = 0; i < LOOPS; i++) {
        const std::size_t size { i };
        // unique=true -> rndm should contain only unique values!
        rndm = gen::random<Ty>(size, 0, LOOPS, true);
        tree = BSTuptr    <Ty>();

        // assertion to make sure all values are inserted and unique!
        for (const std::size_t x : rndm)
            ASSERT_TRUE(tree.add(x));

        // remove all the elements we just placed in the tree
        for (std::size_t j = 0; j < size; j++) {
            const Ty value { rndm[j] };
            ASSERT_TRUE(tree.remove(value));
            ASSERT_EQ(tree.size(), size - j - 1);
        }
        ASSERT_TRUE(tree.isEmpty());
    }
}

TEST_F(BSTuptrTest, iterator)
{
    using Ty = int;
    BSTuptr tree = BSTuptr<Ty>({ 3, 2, 4, 1}, TreeTravOrder::IN_ORDER);
    auto it = tree.begin();

    // equality / inequality
    ASSERT_EQ(it == it, 1);
    ASSERT_EQ(it != it, 0);

    // dereferencing & iteration
    ASSERT_EQ(*it,     1);
    ASSERT_EQ(*(++it), 2);
    ASSERT_EQ(*(--it), 1);
    ASSERT_EQ(*(++it), 2);
    ASSERT_EQ(*(it++), 2);
    ASSERT_EQ(*it,     3);
    ASSERT_EQ(*(++it), 4);

    Ty i {0}; // range for loop
    for (const Ty& n : tree) {
        ASSERT_EQ(n, ++i);
    }

    i = 0; // standard library algorithm
    std::for_each(tree.begin(), tree.end(), [&](const Ty& n) {
        ASSERT_EQ(n, ++i);
    });
}

TEST_F(BSTuptrTest, iteratorTraversalOrderIn)
{
    testTraversalOrder({3, 2, 4, 1}, {1, 2, 3, 4}, TreeTravOrder::IN_ORDER);
}

TEST_F(BSTuptrTest, iteratorTraversalOrderPre)
{
    testTraversalOrder({3, 2, 4, 1}, {3, 2, 1, 4}, TreeTravOrder::PRE_ORDER);
}

TEST_F(BSTuptrTest, iteratorTraversalOrderPost)
{
    testTraversalOrder({3, 2, 4, 1}, {1, 2, 4, 3}, TreeTravOrder::POST_ORDER);
}

TEST_F(BSTuptrTest, iteratorTraversalOrderLevel)
{
    testTraversalOrder({3, 2, 4, 1}, {3, 2, 4, 1}, TreeTravOrder::LEVEL_ORDER);
}
