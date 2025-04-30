#include <wndx/ds/binary_search_tree/BSTuptr.hpp>

#include <wndx/common/gen.hpp>
#include <wndx/common/pprint.hpp>

#include <gtest/gtest.h>

#include <algorithm>            // std::for_each
#include <cstddef>              // size_t
#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

using namespace wndx::algo;

class BSTuptrTest : public ::testing::Test
{
protected:
    static constexpr std::size_t LOOPS{ 100 };

    virtual void SetUp()
    {}

    template<typename T> // this is just a print debug info
    void print_initial(ds::BSTuptr<T> &tree, const std::vector<T> &rndm)
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
            const ds::TreeTravOrder order
        )
    {
        ds::BSTuptr<T> tree(l_i, order);
        const T* it{ l_o.begin() };
        // check size
        ASSERT_EQ(tree.size(), l_o.size());
        // check if the traversal order matches the expected order
        for (const T& e : tree) ASSERT_EQ(e, *(it++));
    }

};

TEST_F(BSTuptrTest, covVirtDtor)
{
    ASSERT_NO_THROW({
        ds::BSTuptr<int> *ptr{ new ds::BSTuptr<int> };
        if (ptr) delete ptr;
    });

    ASSERT_NO_THROW({
        ds::BSTuptr<char> *ptr{ new ds::BSTuptr<char> };
        if (ptr) delete ptr;
    });

    ASSERT_NO_THROW({
        ds::BSTuptr<std::string> *ptr{ new ds::BSTuptr<std::string> };
        if (ptr) delete ptr;
    });
}

TEST_F(BSTuptrTest, iteratorVirtDtor)
{
    ASSERT_NO_THROW({
        ds::BSTuptrIterator<int> *ptr{ new ds::BSTuptrIterator<int> };
        if (ptr) delete ptr;
    });
}

TEST_F(BSTuptrTest, testEmpty)
{
    ds::BSTuptr<std::string> tree;
    ASSERT_TRUE(tree.empty());
    tree.add("Hello World!");
    ASSERT_FALSE(tree.empty());
}

TEST_F(BSTuptrTest, testSize)
{
    ds::BSTuptr<std::string> tree;
    ASSERT_EQ(tree.size(), 0);
    tree.add("Hello World!");
    ASSERT_EQ(tree.size(), 1);
}

TEST_F(BSTuptrTest, testAdd)
{
    ds::BSTuptr<char> tree;
    // Add element which does not yet exist
    ASSERT_TRUE(tree.add('A'));
    // Try to add duplicate element
    ASSERT_FALSE(tree.add('A'));
    // Add a second element which is not a duplicate
    ASSERT_TRUE(tree.add('B'));
}

TEST_F(BSTuptrTest, testContains)
{
    ds::BSTuptr<char> tree;
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
    ds::BSTuptr<char> tree;
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
    ds::BSTuptr<char> tree;
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
    constexpr Ty max{ static_cast<Ty>(LOOPS) };
    std::vector<Ty> rndm;
    ds::BSTuptr<Ty> tree;
    for (std::size_t i = 0; i < LOOPS; i++) {
        const std::size_t size { i };
        // unique=true -> rndm should contain only unique values!
        rndm = gen::random<Ty>(size, 0, max, true);
        tree = ds::BSTuptr<Ty>();

        // assertion to make sure all values are inserted and unique!
        for (const Ty &x : rndm)
            ASSERT_TRUE(tree.add(x));

        // remove all the elements we just placed in the tree
        for (std::size_t j = 0; j < size; j++) {
            const Ty value{ rndm.at(j) };
            ASSERT_TRUE(tree.remove(value));
            ASSERT_EQ(tree.size(), size - j - 1);
        }
        ASSERT_TRUE(tree.empty());
    }
}

TEST_F(BSTuptrTest, iteratorRootNodeNull)
{
    // validate iterator behavior when tree is empty
    // empty tree => root node is nullptr => do not die (do nothing)
    ds::BSTuptr<int> tree;
    ds::BSTuptrIterator<int> it(tree.cbegin());

    ASSERT_NO_THROW({
        it.inorder();
    });
    ASSERT_NO_THROW({
        it.preorder();
    });
    ASSERT_NO_THROW({
        it.postorder();
    });
    ASSERT_NO_THROW({
        it.levelorder();
    });
}

TEST_F(BSTuptrTest, iteratorBasics)
{
    using Ty = int;
    ds::BSTuptr<Ty> tree({3, 2, 1, 4}, ds::TreeTravOrder::IN_ORDER);
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
    ASSERT_EQ(*(it--), 4);
    ASSERT_EQ(*it,     3);

    Ty i {0}; // range for loop
    for (const Ty& n : tree) {
        ASSERT_EQ(n, ++i);
    }

    i = 0; // standard library algorithm
    std::for_each(tree.cbegin(), tree.cend(), [&](const Ty& n) {
        ASSERT_EQ(n, ++i);
    });
}

TEST_F(BSTuptrTest, iteratorTraversalOrderIn)
{
    testTraversalOrder({3, 2, 4, 1}, {1, 2, 3, 4}, ds::TreeTravOrder::IN_ORDER);
    testTraversalOrder({3, 4, 2, 1}, {1, 2, 3, 4}, ds::TreeTravOrder::IN_ORDER);
    testTraversalOrder({2, 1, 4, 3}, {1, 2, 3, 4}, ds::TreeTravOrder::IN_ORDER);
    testTraversalOrder({4, 3, 2, 1}, {1, 2, 3, 4}, ds::TreeTravOrder::IN_ORDER);

    testTraversalOrder({1, 2, 3, 4}, {1, 2, 3, 4}, ds::TreeTravOrder::IN_ORDER);
    testTraversalOrder({1, 2, 4, 3}, {1, 2, 3, 4}, ds::TreeTravOrder::IN_ORDER);
    testTraversalOrder({1, 4, 3, 2}, {1, 2, 3, 4}, ds::TreeTravOrder::IN_ORDER);
    testTraversalOrder({3, 1, 2, 4}, {1, 2, 3, 4}, ds::TreeTravOrder::IN_ORDER);
    testTraversalOrder({3, 2, 1, 4}, {1, 2, 3, 4}, ds::TreeTravOrder::IN_ORDER);
}

TEST_F(BSTuptrTest, iteratorTraversalOrderPre)
{
    // the same result
    testTraversalOrder({3, 4, 2, 1}, {3, 2, 1, 4}, ds::TreeTravOrder::PRE_ORDER);
    testTraversalOrder({3, 2, 4, 1}, {3, 2, 1, 4}, ds::TreeTravOrder::PRE_ORDER);

    // #1 => then input == output
    testTraversalOrder({3, 4, 1, 2}, {3, 1, 2, 4}, ds::TreeTravOrder::PRE_ORDER);
    testTraversalOrder({3, 1, 2, 4}, {3, 1, 2, 4}, ds::TreeTravOrder::PRE_ORDER);

    // #2 => then input == output
    testTraversalOrder({2, 4, 3, 1}, {2, 1, 4, 3}, ds::TreeTravOrder::PRE_ORDER);
    testTraversalOrder({2, 1, 4, 3}, {2, 1, 4, 3}, ds::TreeTravOrder::PRE_ORDER);

    // #3 => then input == output
    testTraversalOrder({1, 3, 4, 2}, {1, 3, 2, 4}, ds::TreeTravOrder::PRE_ORDER);
    testTraversalOrder({1, 3, 2, 4}, {1, 3, 2, 4}, ds::TreeTravOrder::PRE_ORDER);

    // #4 => then input == output
    testTraversalOrder({2, 3, 4, 1}, {2, 1, 3, 4}, ds::TreeTravOrder::PRE_ORDER);
    testTraversalOrder({2, 1, 3, 4}, {2, 1, 3, 4}, ds::TreeTravOrder::PRE_ORDER);

    // input == output
    testTraversalOrder({1, 2, 3, 4}, {1, 2, 3, 4}, ds::TreeTravOrder::PRE_ORDER);
    testTraversalOrder({1, 2, 4, 3}, {1, 2, 4, 3}, ds::TreeTravOrder::PRE_ORDER);
    testTraversalOrder({1, 4, 3, 2}, {1, 4, 3, 2}, ds::TreeTravOrder::PRE_ORDER);
    testTraversalOrder({3, 1, 2, 4}, {3, 1, 2, 4}, ds::TreeTravOrder::PRE_ORDER);
    testTraversalOrder({3, 2, 1, 4}, {3, 2, 1, 4}, ds::TreeTravOrder::PRE_ORDER);

}

TEST_F(BSTuptrTest, iteratorTraversalOrderPost)
{
    // #1
    testTraversalOrder({3, 2, 4, 1}, {1, 2, 4, 3}, ds::TreeTravOrder::POST_ORDER);
    // => then reverses the order back and forth
    testTraversalOrder({1, 2, 4, 3}, {3, 4, 2, 1}, ds::TreeTravOrder::POST_ORDER);
    testTraversalOrder({3, 4, 2, 1}, {1, 2, 4, 3}, ds::TreeTravOrder::POST_ORDER);

    // #2
    testTraversalOrder({1, 3, 2, 4}, {2, 4, 3, 1}, ds::TreeTravOrder::POST_ORDER);
    // => then reverses the order back and forth
    testTraversalOrder({2, 4, 3, 1}, {1, 3, 4, 2}, ds::TreeTravOrder::POST_ORDER);
    testTraversalOrder({1, 3, 4, 2}, {2, 4, 3, 1}, ds::TreeTravOrder::POST_ORDER);

    // #3
    testTraversalOrder({2, 1, 3, 4}, {1, 4, 3, 2}, ds::TreeTravOrder::POST_ORDER);
    // => then reverses the order back and forth
    testTraversalOrder({1, 4, 3, 2}, {2, 3, 4, 1}, ds::TreeTravOrder::POST_ORDER);
    testTraversalOrder({2, 3, 4, 1}, {1, 4, 3, 2}, ds::TreeTravOrder::POST_ORDER);

    // #4 - just reverses the order back and forth
    testTraversalOrder({1, 2, 3, 4}, {4, 3, 2, 1}, ds::TreeTravOrder::POST_ORDER);
    testTraversalOrder({4, 3, 2, 1}, {1, 2, 3, 4}, ds::TreeTravOrder::POST_ORDER);
}

TEST_F(BSTuptrTest, iteratorTraversalOrderLevel)
{
    // #1
    testTraversalOrder({2, 4, 3, 1}, {2, 1, 4, 3}, ds::TreeTravOrder::LEVEL_ORDER);
    // => then input == output
    testTraversalOrder({2, 1, 4, 3}, {2, 1, 4, 3}, ds::TreeTravOrder::LEVEL_ORDER);

    // #2
    testTraversalOrder({2, 3, 4, 1}, {2, 1, 3, 4}, ds::TreeTravOrder::LEVEL_ORDER);
    // => then input == output
    testTraversalOrder({2, 1, 3, 4}, {2, 1, 3, 4}, ds::TreeTravOrder::LEVEL_ORDER);

    // the same result
    testTraversalOrder({3, 4, 1, 2}, {3, 1, 4, 2}, ds::TreeTravOrder::LEVEL_ORDER);
    testTraversalOrder({3, 1, 2, 4}, {3, 1, 4, 2}, ds::TreeTravOrder::LEVEL_ORDER);

    // input == output
    testTraversalOrder({1, 2, 3, 4}, {1, 2, 3, 4}, ds::TreeTravOrder::LEVEL_ORDER);
    testTraversalOrder({1, 3, 2, 4}, {1, 3, 2, 4}, ds::TreeTravOrder::LEVEL_ORDER);
    testTraversalOrder({3, 2, 4, 1}, {3, 2, 4, 1}, ds::TreeTravOrder::LEVEL_ORDER);
    testTraversalOrder({4, 3, 2, 1}, {4, 3, 2, 1}, ds::TreeTravOrder::LEVEL_ORDER);
}
