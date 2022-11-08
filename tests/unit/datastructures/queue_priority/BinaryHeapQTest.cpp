#include "BinaryHeapQ.hpp"

#include "gen.hpp"

#include <gtest/gtest.h>

#include <algorithm>        // sort, find
#include <cstdint>          // other types, *_MIN, etc
#include <functional>       // std::greater
#include <queue>            // priority_queue
#include <stdexcept>        // runtime_error
#include <string>
#include <vector>

using namespace wndx;

class BinaryHeapQTest : public ::testing::Test
{
protected:
    static const int LOOPS  = 100;
    static const int MAX_SZ = 100;

    // aliases for shortness
    using i8f = int_fast8_t;

    virtual void SetUp()
    {}

    void sequentialRemoving(const std::vector<int> &input, const std::vector<int> &rmord)
    {
        ASSERT_EQ(input.size(), rmord.size());

        BinaryHeapQ pq { BinaryHeapQ<int>(input) };

        // make copy of input & sort vector
        std::vector<int> sv { input.begin(), input.end() };
        std::sort(sv.begin(), sv.end(), std::less<int>());

        EXPECT_TRUE(pq.isMinHeap(0));

        std::vector<int>::iterator rmpos;

        for (int i = 0; i < rmord.size(); i++) {
            int elem { rmord[i] };
            // first found pos of elem in vector by value (to remove only one element, not duplicates)
            rmpos = std::find(sv.begin(), sv.end(), elem);

            ASSERT_EQ(pq.peek(), sv.front());
            ASSERT_EQ(pq.remove(elem), bool(*sv.erase(rmpos)));
            ASSERT_EQ(pq.size(), sv.size());
            EXPECT_TRUE(pq.isMinHeap(0));
        }
    }

};

TEST_F(BinaryHeapQTest, testEmpty)
{
    BinaryHeapQ q = BinaryHeapQ<int>();
    EXPECT_TRUE(q.isEmpty());
    EXPECT_EQ(q.size(), 0);
}

TEST_F(BinaryHeapQTest, testHeapProperty)
{
    const std::vector<int> nums {3, 2, 5, 6, 7, 9, 4, 8, 1};

    // Try manually creating heap
    BinaryHeapQ q = BinaryHeapQ<int>();
    for (int n : nums) q.add(n);
    for (int i = 1; i <= nums.size(); i++) ASSERT_EQ(q.poll(), i);
    q.clear(); ASSERT_EQ(q.size(), 0);

    // Try heapify constructor
    q = BinaryHeapQ<int>(nums);
    for (int i = 1; i <= nums.size(); i++) ASSERT_EQ(q.poll(), i);
    q.clear(); ASSERT_EQ(q.size(), 0);
}

TEST_F(BinaryHeapQTest, testHeapify)
{
    std::priority_queue<i8f, std::vector<i8f>, std::greater<i8f>> PQ;
    BinaryHeapQ<i8f> pq;
    std::vector<i8f> rndm;
    for (int i = 1; i < LOOPS; i++) {
        rndm = gen::random<i8f>(i, INT_FAST8_MIN, INT_FAST8_MAX);
        pq   = BinaryHeapQ<i8f>(rndm);
        PQ   = { rndm.begin(), rndm.end() };

        EXPECT_TRUE(pq.isMinHeap(0));
        while (!PQ.empty()) {
            ASSERT_EQ(pq.poll(), PQ.top());
            PQ.pop();
        }
    }
}

TEST_F(BinaryHeapQTest, testClear)
{
    const std::vector<std::string> strs {"aa", "bb", "cc", "dd", "ee"};
    BinaryHeapQ q { BinaryHeapQ<std::string>(strs) };
    q.clear();
    ASSERT_EQ(q.size(), 0);
    ASSERT_TRUE(q.isEmpty());
}

TEST_F(BinaryHeapQTest, testContainment)
{
    const std::vector<std::string> strs {"aa", "bb", "cc", "dd", "ee"};
    BinaryHeapQ q { BinaryHeapQ<std::string>(strs) };
    EXPECT_FALSE(q.remove("NOT_EXIST"));
    q.remove("aa");
    EXPECT_FALSE(q.contains("aa"));
    q.remove("bb");
    EXPECT_FALSE(q.contains("bb"));
    q.remove("cc");
    EXPECT_FALSE(q.contains("cc"));
    q.remove("dd");
    EXPECT_FALSE(q.contains("dd"));
    q.remove("ee");
    EXPECT_FALSE(q.contains("ee"));
    EXPECT_EQ(q.size(), 0);
    EXPECT_TRUE(q.isEmpty());
}

// hacky test since std::priority_queue does not have contains(), remove(val) methods
TEST_F(BinaryHeapQTest, testContainmentRandomized)
{
    std::priority_queue<int, std::vector<int>, std::greater<int>> PQ;
    BinaryHeapQ<int> pq;
    std::vector<int> rndm;
    for (int i = 1; i < LOOPS; i++) {
        rndm = gen::random<int>(i, INT_MIN, INT_MAX);
        pq   = BinaryHeapQ<int>(rndm);
        PQ   = { rndm.begin(), rndm.end() };

        for (int j = 0; j < rndm.size(); j++) {
            int randVal { rndm[j] };
            ASSERT_EQ(pq.contains(randVal), true);
            EXPECT_TRUE(pq.remove(randVal));
            PQ.pop();
            ASSERT_EQ(pq.contains(randVal), false);
        }
    }
    ASSERT_TRUE(pq.isEmpty());
}

TEST_F(BinaryHeapQTest, testRemovingDuplicates)
{
    const std::vector<int> in {2, 7, 2, 11, 7, 13, 2};
    BinaryHeapQ pq { BinaryHeapQ<int>(in) };
    ASSERT_EQ(2,  pq.peek());
    pq.add(3);
    ASSERT_EQ(2,  pq.poll());
    ASSERT_EQ(2,  pq.poll());
    ASSERT_EQ(2,  pq.poll());
    ASSERT_EQ(3,  pq.poll());
    ASSERT_EQ(7,  pq.poll());
    ASSERT_EQ(7,  pq.poll());
    ASSERT_EQ(11, pq.poll());
    ASSERT_EQ(13, pq.poll());
}

TEST_F(BinaryHeapQTest, testRemovingOrder)
{
    std::vector<int> input, rmord;
    input = {1, 2, 3, 4, 5, 6, 7};
    rmord = {1, 3, 6, 4, 5, 7, 2}; // remove order
    sequentialRemoving(input, rmord);

    input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    rmord = {7, 4, 6, 10, 2, 5, 11, 3, 1, 8, 9};
    sequentialRemoving(input, rmord);

    input = {8, 1, 3, 3, 5, 3};
    rmord = {3, 3, 5, 8, 1, 3};
    sequentialRemoving(input, rmord);

    input = {7, 7, 3, 1, 1, 2};
    rmord = {2, 7, 1, 3, 7, 1};
    sequentialRemoving(input, rmord);

    input = {32, 66, 93, 42, 41, 91, 54, 64, 9, 35};
    rmord = {64, 93, 54, 41, 35, 9, 66, 42, 32, 91};
    sequentialRemoving(input, rmord);
}
