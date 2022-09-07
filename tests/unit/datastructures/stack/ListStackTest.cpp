#include "ListStack.hpp"

#include <gtest/gtest.h>

#include <stdexcept>        // runtime_error
#include <string>

class ListStackTest : public ::testing::Test {
protected:
    ListStack<int> *stack;

    ListStackTest()
    {
        stack = new ListStack<int>();
    }

    ListStackTest(const int &elem)
    {
        stack = new ListStack<int>(elem);
    }

    virtual ~ListStackTest()
    {
        if (stack) delete stack;
    }

    virtual void SetUp()
    {}

    virtual void TearDown()
    {
        stack->clear();
    }

};

TEST_F(ListStackTest, testEmptyStack)
{
    EXPECT_TRUE(stack->isEmpty());
    EXPECT_EQ(stack->size(), 0);
}

TEST_F(ListStackTest, testPopOnEmpty)
{
    try {
        stack->pop();
    } catch(std::runtime_error const &err) {
        EXPECT_EQ(err.what(), std::string("Empty Stack"));
    } catch(...) {
        FAIL() << "Expected std::runtime_error Empty Stack";
    }
}

TEST_F(ListStackTest, testPeekOnEmpty)
{
    try {
        stack->peek();
    } catch(std::runtime_error const &err) {
        EXPECT_EQ(err.what(), std::string("Empty Stack"));
    } catch(...) {
        FAIL() << "Expected std::runtime_error Empty Stack";
    }
}

TEST_F(ListStackTest, testPush)
{
    stack->push(2);
    EXPECT_EQ(stack->size(), 1);
}

TEST_F(ListStackTest, testPeek)
{
    stack->push(2);
    EXPECT_EQ(2, stack->peek());
    EXPECT_EQ(stack->size(), 1);
}

TEST_F(ListStackTest, testPop)
{
    stack->push(2);
    EXPECT_EQ(2, stack->pop());
    EXPECT_EQ(stack->size(), 0);
}

TEST_F(ListStackTest, testExhaustively)
{
    ASSERT_TRUE(stack->isEmpty());
    stack->push(1);
    ASSERT_FALSE(stack->isEmpty());
    stack->push(2);
    EXPECT_EQ(stack->size(), 2);
    EXPECT_EQ(2, (int) stack->peek());
    EXPECT_EQ(stack->size(), 2);
    EXPECT_EQ(2, (int) stack->pop());
    EXPECT_EQ(stack->size(), 1);
    EXPECT_EQ(1, (int) stack->peek());
    EXPECT_EQ(stack->size(), 1);
    EXPECT_EQ(1, (int) stack->pop());
    EXPECT_EQ(stack->size(), 0);
    ASSERT_TRUE(stack->isEmpty());
}