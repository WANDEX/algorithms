#include "DoublyLinkedList.hpp"

#include <gtest/gtest.h>

#include <stdexcept>        // runtime_error
#include <string>

class DoublyLinkedListTest : public ::testing::Test
{
protected:
    DoublyLinkedList<int> *list;

    DoublyLinkedListTest()
    {
        list = new DoublyLinkedList<int>();
    }

    virtual ~DoublyLinkedListTest()
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

TEST_F(DoublyLinkedListTest, testEmptyList)
{
    EXPECT_TRUE(list->isEmpty());
    EXPECT_EQ(list->size(), 0);
}

TEST_F(DoublyLinkedListTest, testRemoveFirstOfEmpty)
{
    try {
        list->removeFirst();
    } catch(std::runtime_error const &err) {
        EXPECT_EQ(err.what(), std::string("Empty list"));
    } catch(...) {
        FAIL() << "Expected std::runtime_error Empty list";
    }
}

TEST_F(DoublyLinkedListTest, testRemoveLastOfEmpty)
{
    try {
        list->removeLast();
    } catch(std::runtime_error const &err) {
        EXPECT_EQ(err.what(), std::string("Empty list"));
    } catch(...) {
        FAIL() << "Expected std::runtime_error Empty list";
    }
}

TEST_F(DoublyLinkedListTest, testPeekFirstOfEmpty)
{
    try {
        list->peekFirst();
    } catch(std::runtime_error const &err) {
        EXPECT_EQ(err.what(), std::string("Empty list"));
    } catch(...) {
        FAIL() << "Expected std::runtime_error Empty list";
    }
}

TEST_F(DoublyLinkedListTest, testPeekLastOfEmpty)
{
    try {
        list->peekLast();
    } catch(std::runtime_error const &err) {
        EXPECT_EQ(err.what(), std::string("Empty list"));
    } catch(...) {
        FAIL() << "Expected std::runtime_error Empty list";
    }
}

TEST_F(DoublyLinkedListTest, testAddFirst)
{
    list->addFirst(3);
    ASSERT_EQ(list->size(), 1);
    list->addFirst(5);
    ASSERT_EQ(list->size(), 2);
}

TEST_F(DoublyLinkedListTest, testAddLast)
{
    list->addLast(3);
    ASSERT_EQ(list->size(), 1);
    list->addLast(5);
    ASSERT_EQ(list->size(), 2);
}

TEST_F(DoublyLinkedListTest, testAddAt)
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

TEST_F(DoublyLinkedListTest, testRemoveFirst)
{
    list->addFirst(3);
    ASSERT_TRUE(list->removeFirst() == 3);
    ASSERT_TRUE(list->isEmpty());
}

TEST_F(DoublyLinkedListTest, testRemoveLast)
{
    list->addLast(4);
    ASSERT_TRUE(list->removeLast() == 4);
    ASSERT_TRUE(list->isEmpty());
}

TEST_F(DoublyLinkedListTest, testPeekFirst)
{
    list->addFirst(4);
    ASSERT_TRUE(list->peekFirst() == 4);
    ASSERT_EQ(list->size(), 1);
}

TEST_F(DoublyLinkedListTest, testPeekLast)
{
    list->addLast(4);
    ASSERT_TRUE(list->peekLast() == 4);
    ASSERT_EQ(list->size(), 1);
}

TEST_F(DoublyLinkedListTest, testPeeking)
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

TEST_F(DoublyLinkedListTest, testRemoving)
{
    DoublyLinkedList<std::string> strs = DoublyLinkedList<std::string>();
    strs.add("a");
    strs.add("b");
    strs.add("c");
    strs.add("d");
    strs.add("e");
    strs.add("f");
    strs.remove("b");
    strs.remove("a");
    strs.remove("d");
    strs.remove("e");
    strs.remove("c");
    strs.remove("f");
    ASSERT_EQ(0, strs.size());
}

TEST_F(DoublyLinkedListTest, testRemoveAt)
{
    list->add(1);
    list->add(2);
    list->add(3);
    list->add(4);
    list->removeAt(0);
    list->removeAt(2);
    ASSERT_TRUE(list->peekFirst() == 2);
    ASSERT_TRUE(list->peekLast()  == 3);
    list->removeAt(1);
    list->removeAt(0);
    ASSERT_EQ(list->size(), 0);
}

TEST_F(DoublyLinkedListTest, testClear)
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

TEST_F(DoublyLinkedListTest, testToString)
{
    DoublyLinkedList<std::string> strs = DoublyLinkedList<std::string>();
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
