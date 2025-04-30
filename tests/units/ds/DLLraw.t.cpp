#include <wndx/ds/linked_list/DLLraw.hpp>

#include <wndx/common/gen.hpp>

#include <gtest/gtest.h>

#include <algorithm>            // std::for_each
#include <cstddef>              // std::size_t
#include <initializer_list>
#include <list>
#include <sstream>              // std::ostringstream
#include <stdexcept>            // std::runtime_error, std::out_of_range
#include <string>
#include <vector>

using namespace wndx;

class DLLrawTest : public ::testing::Test
{
protected:
    const std::string err_empty_list { "Empty list." };
    const std::string err_empty_list_desc { "Expected std::runtime_error " + err_empty_list };

    ds::DLLraw<int> *list;

    DLLrawTest()
    {
        list = new ds::DLLraw<int>();
    }

    virtual ~DLLrawTest()
    {
        if (list) delete list;
    }

    virtual void SetUp()
    {}

    virtual void TearDown()
    {
        list->clear();
    }

};

TEST_F(DLLrawTest, testEmptyList)
{
    EXPECT_TRUE(list->empty());
    EXPECT_EQ(list->size(), 0);
}

TEST_F(DLLrawTest, covVirtDtor)
{
    ASSERT_NO_THROW({
        ds::DLLraw<int> *ptr { new ds::DLLraw<int> };
        if (ptr) delete ptr;
    });

    ASSERT_NO_THROW({
        ds::DLLraw<std::string> *ptr { new ds::DLLraw<std::string> };
        if (ptr) delete ptr;
    });
}

TEST_F(DLLrawTest, testAddAtOutOfRange)
{
    const std::string exp_err{ "Index > size." };
    const std::size_t idx_out_of_range{ list->size() + 1 };
    try {
        list->addAt(idx_out_of_range, 1);
    } catch(std::out_of_range const &err) {
        EXPECT_EQ(err.what(), std::string(exp_err));
    } catch(...) {
        FAIL() << "Expected std::out_of_range " + exp_err;
    }
}

TEST_F(DLLrawTest, testRemoveAtOutOfRange)
{
    const std::string exp_err{ "Index >= size." };
    const std::size_t idx_out_of_range{ list->size() };
    try {
        list->removeAt(idx_out_of_range);
    } catch(std::out_of_range const &err) {
        EXPECT_EQ(err.what(), std::string(exp_err));
    } catch(...) {
        FAIL() << "Expected std::out_of_range " + exp_err;
    }
}

TEST_F(DLLrawTest, testRemoveFirstOfEmpty)
{
    try {
        list->removeFirst();
    } catch(std::runtime_error const &err) {
        EXPECT_EQ(err.what(), std::string(err_empty_list));
    } catch(...) {
        FAIL() << err_empty_list_desc;
    }
}

TEST_F(DLLrawTest, testRemoveLastOfEmpty)
{
    try {
        list->removeLast();
    } catch(std::runtime_error const &err) {
        EXPECT_EQ(err.what(), std::string(err_empty_list));
    } catch(...) {
        FAIL() << err_empty_list_desc;
    }
}

TEST_F(DLLrawTest, testPeekFirstOfEmpty)
{
    try {
        list->peekFirst();
    } catch(std::runtime_error const &err) {
        EXPECT_EQ(err.what(), std::string(err_empty_list));
    } catch(...) {
        FAIL() << err_empty_list_desc;
    }
}

TEST_F(DLLrawTest, testPeekLastOfEmpty)
{
    try {
        list->peekLast();
    } catch(std::runtime_error const &err) {
        EXPECT_EQ(err.what(), std::string(err_empty_list));
    } catch(...) {
        FAIL() << err_empty_list_desc;
    }
}

TEST_F(DLLrawTest, testAddFirst)
{
    list->addFirst(3);
    ASSERT_EQ(list->size(), 1);
    list->addFirst(5);
    ASSERT_EQ(list->size(), 2);
}

TEST_F(DLLrawTest, testAddLast)
{
    list->addLast(3);
    ASSERT_EQ(list->size(), 1);
    list->addLast(5);
    ASSERT_EQ(list->size(), 2);
}

TEST_F(DLLrawTest, testClear)
{
    list->add(22);
    list->add(33);
    list->add(44);
    ASSERT_EQ(list->size(), 3);
    list->clear();
    ASSERT_EQ(list->size(), 0);
    list->add(22);
    list->add(33);
    list->add(44);
    ASSERT_EQ(list->size(), 3);
    list->clear();
    ASSERT_EQ(list->size(), 0);
}

TEST_F(DLLrawTest, testAddAt)
{
    list->addAt(0, 1);
    ASSERT_EQ(list->size(), 1);
    list->addAt(1, 2);
    ASSERT_EQ(list->size(), 2);
    list->addAt(1, 3);
    ASSERT_EQ(list->size(), 3);
    list->addAt(2, 4);
    ASSERT_EQ(list->size(), 4);
    list->addAt(1, 8);
    ASSERT_EQ(list->size(), 5);
}

TEST_F(DLLrawTest, testRemoveFirst)
{
    list->addFirst(3);
    ASSERT_TRUE(list->removeFirst() == 3);
    ASSERT_TRUE(list->empty());
}

TEST_F(DLLrawTest, testRemoveLast)
{
    list->addLast(4);
    ASSERT_TRUE(list->removeLast() == 4);
    ASSERT_TRUE(list->empty());
}

TEST_F(DLLrawTest, testPeekFirst)
{
    list->addFirst(4);
    ASSERT_TRUE(list->peekFirst() == 4);
    ASSERT_EQ(list->size(), 1);
}

TEST_F(DLLrawTest, testPeekLast)
{
    list->addLast(4);
    ASSERT_TRUE(list->peekLast() == 4);
    ASSERT_EQ(list->size(), 1);
}

TEST_F(DLLrawTest, testPeeking)
{
    // 5
    list->addFirst(5);
    ASSERT_TRUE(list->peekFirst() == 5);
    ASSERT_TRUE(list->peekLast()  == 5);
    // 6 - 5
    list->addFirst(6);
    ASSERT_TRUE(list->peekFirst() == 6);
    ASSERT_TRUE(list->peekLast()  == 5);
    // 7 - 6 - 5
    list->addFirst(7);
    ASSERT_TRUE(list->peekFirst() == 7);
    ASSERT_TRUE(list->peekLast()  == 5);
    // 7 - 6 - 5 - 8
    list->addLast(8);
    ASSERT_TRUE(list->peekFirst() == 7);
    ASSERT_TRUE(list->peekLast()  == 8);
    // 7 - 6 - 5
    list->removeLast();
    ASSERT_TRUE(list->peekFirst() == 7);
    ASSERT_TRUE(list->peekLast()  == 5);
    // 7 - 6
    list->removeLast();
    ASSERT_TRUE(list->peekFirst() == 7);
    ASSERT_TRUE(list->peekLast()  == 6);
    // 6
    list->removeFirst();
    ASSERT_TRUE(list->peekFirst() == 6);
    ASSERT_TRUE(list->peekLast()  == 6);
}

TEST_F(DLLrawTest, testRemoving)
{
    // also cover list initialization
    ds::DLLraw<std::string> strs{ "a", "b", "c", "d", "e", "f" };

    ASSERT_TRUE(strs.contains("f"));
    // cover removeLast() for std::string case
    ASSERT_EQ("f", strs.removeLast());
    ASSERT_FALSE(strs.contains("f"));
    // cover false branch - was not found
    ASSERT_FALSE(strs.remove("f"));

    strs.remove("b");
    strs.remove("a");
    strs.remove("d");
    strs.remove("e");
    strs.remove("c");
    ASSERT_EQ(0, strs.size());
}

TEST_F(DLLrawTest, testRemoveAt)
{
    list->add(1);
    list->add(2);
    list->add(3);
    list->add(4);
    list->removeAt(3);
    list->removeAt(0);
    ASSERT_EQ(list->peekFirst(), 2);
    ASSERT_EQ(list->peekLast(), 3);
    list->removeAt(1);
    list->removeAt(0);
    ASSERT_EQ(list->size(), 0);
}

// cover all branches of the removeAt()
TEST_F(DLLrawTest, testRemoveAtBranches)
{
    constexpr std::size_t nloops{ 24 };
    std::vector<int> rndm(nloops);
    for (std::size_t sz{ 1 }; sz <= nloops; sz++) {
        rndm = gen::random<int>(sz, 0, nloops * 4);
        ds::DLLraw<int> tmp_list{ rndm };
        ASSERT_EQ(rndm.size(), tmp_list.size());
        while (!tmp_list.empty()) {
            tmp_list.removeAt(gen::srng<std::size_t>(0, tmp_list.size() - 1));
        }
    }
}

TEST_F(DLLrawTest, testRangeBasedLoop)
{
    ds::DLLraw<int> tmp_list{ 0, 1, 2, 3, 4 };
    int i{ 0 };
    for (const int e : tmp_list) {
        EXPECT_EQ(i++, e);
    }
}

TEST_F(DLLrawTest, testToString)
{
    ds::DLLraw<std::string> strs;
    ASSERT_EQ(strs.toString(), "{  }");
    strs.add("a");
    ASSERT_EQ(strs.toString(), "{ a }");
    strs.add("b");
    ASSERT_EQ(strs.toString(), "{ a, b }");
    strs.add("c");
    strs.add("d");
    strs.add("e");
    strs.add("f");
    ASSERT_EQ(strs.toString(), "{ a, b, c, d, e, f }");
}

TEST_F(DLLrawTest, testIterator_begin_end_postfix)
{
    ds::DLLraw<int> lst{ 0, 1, 2, 3, 4 };

    int i {0};
    auto it{ lst.begin() };
    for (; it != lst.end(); it++, ++i) {
        ASSERT_EQ(i, *it);
    }
}

TEST_F(DLLrawTest, testOstream)
{
    ds::DLLraw<int> lst{ 0, 1, 2, 3, 4 };
    std::ostringstream oss;
    oss << lst;
    ASSERT_EQ(lst.toString(), oss.str());
}

TEST_F(DLLrawTest, iterator)
{
    const std::initializer_list<int> il{ 0, 1, 2, 3, 4 };
    const std::list <int> slst{ il };
    const ds::DLLraw<int> dllr{ il };

    ASSERT_EQ(slst.size(), dllr.size());

    auto it_l{ slst.begin() };
    auto it_d{ dllr.begin() };
    ASSERT_EQ(*it_l, 0);
    ASSERT_EQ(*it_d, 0);

    // check that container has all of the provided elements,
    // and traversal order is the same as in std::list
    for (;it_l != slst.end(); ++it_l, ++it_d) {
        ASSERT_EQ(*it_l, *it_d);
    }
}

TEST_F(DLLrawTest, reverse_iterator)
{
    const std::initializer_list<int> il{ 0, 1, 2, 3, 4 };
    const std::list <int> slst{ il };
    const ds::DLLraw<int> dllr{ il };

    ASSERT_EQ(slst.size(), dllr.size());

    auto it_l{ slst.rbegin() };
    auto it_d{ dllr.rbegin() };
    ASSERT_EQ(*it_l, 4);
    ASSERT_EQ(*it_d, 4);

    // check that container has all of the provided elements,
    // and traversal order is the same as in std::list
    for (;it_l != slst.rend(); ++it_l, ++it_d) {
        ASSERT_EQ(*it_l, *it_d);
    }
}

/* // XXX uncomment following tests after proper implementation:
//        const_iterator, const_reverse_iterator
TEST_F(DLLrawTest, const_iterator)
{
    const std::initializer_list<int> il{ 0, 1, 2, 3, 4 };
    const std::list <int> list{ il };
    const ds::DLLraw<int> dllr{ il };

    ASSERT_EQ(list.size(), dllr.size());

    auto it_l{ list.cbegin() };
    auto it_d{ dllr.cbegin() };
    ASSERT_EQ(*it_l, 0);
    ASSERT_EQ(*it_d, 0);

    // check that container has all of the provided elements,
    // and traversal order is the same as in std::list
    for (;it_l != list.cend(); ++it_l, ++it_d) {
        ASSERT_EQ(*it_l, *it_d);
    }
}

TEST_F(DLLrawTest, const_reverse_iterator)
{
    const std::initializer_list<int> il{ 0, 1, 2, 3, 4 };
    const std::list <int> list{ il };
    const ds::DLLraw<int> dllr{ il };

    ASSERT_EQ(list.size(), dllr.size());

    auto it_l{ list.crbegin() };
    auto it_d{ dllr.crbegin() };
    ASSERT_EQ(*it_l, 4);
    ASSERT_EQ(*it_d, 4);

    // check that container has all of the provided elements,
    // and traversal order is the same as in std::list
    for (;it_l != list.crend(); ++it_l, ++it_d) {
        ASSERT_EQ(*it_l, *it_d);
    }
}
*/ // XXX

