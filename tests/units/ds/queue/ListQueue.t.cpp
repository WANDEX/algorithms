#include <wndx/ds/queue/ListQueue.hpp>

#include <gtest/gtest.h>

#include <stdexcept>            // std::runtime_error
#include <string>

using namespace wndx;

class ListQueueTest : public ::testing::Test
{
protected:
    ds::ListQueue<int> *queue;

    ListQueueTest()
    {
        queue = new ds::ListQueue<int>();
    }

    ListQueueTest(const int &elem)
    {
        queue = new ds::ListQueue<int>(elem);
    }

    virtual ~ListQueueTest()
    {
        if (queue) delete queue;
    }

    virtual void SetUp()
    {}

    virtual void TearDown()
    {
        queue->clear();
    }

};

TEST_F(ListQueueTest, testEmptyQueue)
{
    EXPECT_TRUE(queue->isEmpty());
    EXPECT_EQ(queue->size(), 0);
}

TEST_F(ListQueueTest, testDequeueOnEmpty)
{
    try {
        queue->dequeue();
    } catch(std::runtime_error const &err) {
        EXPECT_EQ(err.what(), std::string("Empty Queue"));
    } catch(...) {
        FAIL() << "Expected std::runtime_error Empty Queue";
    }
}

TEST_F(ListQueueTest, testPeekOnEmpty)
{
    try {
        queue->peek();
    } catch(std::runtime_error const &err) {
        EXPECT_EQ(err.what(), std::string("Empty Queue"));
    } catch(...) {
        FAIL() << "Expected std::runtime_error Empty Queue";
    }
}

TEST_F(ListQueueTest, testEnqueue)
{
    queue->enqueue(2);
    EXPECT_EQ(queue->size(), 1);
}

TEST_F(ListQueueTest, testPeek)
{
    queue->enqueue(2);
    EXPECT_EQ(2, queue->peek());
    EXPECT_EQ(queue->size(), 1);
}

TEST_F(ListQueueTest, testDequeue)
{
    queue->enqueue(2);
    EXPECT_EQ(2, queue->dequeue());
    EXPECT_EQ(queue->size(), 0);
}

TEST_F(ListQueueTest, testExhaustively)
{
    ASSERT_TRUE(queue->isEmpty());
    queue->enqueue(1);
    ASSERT_FALSE(queue->isEmpty());
    queue->enqueue(2);
    EXPECT_EQ(queue->size(), 2);
    EXPECT_EQ(1, queue->peek());
    EXPECT_EQ(queue->size(), 2);
    EXPECT_EQ(1, queue->dequeue());
    EXPECT_EQ(queue->size(), 1);
    EXPECT_EQ(2, queue->peek());
    EXPECT_EQ(queue->size(), 1);
    EXPECT_EQ(2, queue->dequeue());
    EXPECT_EQ(queue->size(), 0);
    ASSERT_TRUE(queue->isEmpty());
}
