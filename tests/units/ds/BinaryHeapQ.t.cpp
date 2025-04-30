#include <wndx/ds/queue_priority/BinaryHeapQ.hpp>

#include <wndx/common/gen.hpp>

#include <gtest/gtest.h>

#include <algorithm>            // std::sort, std::find
#include <cstddef>              // std::size_t
#include <cstdint>              // other types, *_MIN, etc
#include <functional>           // std::greater
#include <queue>                // std::priority_queue
#include <stdexcept>            // invalid_argument, runtime_error, out_of_range
#include <string>
#include <vector>

using namespace wndx;

class BinaryHeapQTest : public ::testing::Test
{
protected:
    static constexpr std::size_t LOOPS{ 50 };

    virtual void SetUp()
    {}

    void sequentialRemoving(const std::vector<int> &input, const std::vector<int> &rmord)
    {
        ASSERT_EQ(input.size(), rmord.size());

        ds::BinaryHeapQ<int> pq { input };
        // make copy of input & sort vector
        std::vector<int>     sv { input };
        std::sort(sv.begin(), sv.end(), std::less<int>());

        EXPECT_TRUE(pq.isMinHeap(0));

        std::vector<int>::iterator rmpos;

        for (const int elem : rmord) {
            // first found pos of elem in vector by value (to remove only one element, not duplicates)
            rmpos = std::find(sv.begin(), sv.end(), elem);

            ASSERT_EQ(pq.peek(), sv.front());
            ASSERT_TRUE(pq.remove(elem));
            sv.erase(rmpos);
            ASSERT_EQ(pq.size(), sv.size());
            EXPECT_TRUE(pq.isMinHeap(0));
        }
    }

};

TEST_F(BinaryHeapQTest, testEmpty)
{
    ds::BinaryHeapQ<int> q;
    ASSERT_TRUE(q.empty());
    ASSERT_EQ(q.size(), 0);
}

TEST_F(BinaryHeapQTest, testClear)
{
    ds::BinaryHeapQ<int> q {0, 1, 2};
    ASSERT_FALSE(q.empty());
    q.clear();
    ASSERT_EQ(q.size(), 0);
    ASSERT_TRUE(q.empty());
}

TEST_F(BinaryHeapQTest, testMapGetNoThrow)
{
    ds::BinaryHeapQ<int> q;
    ASSERT_TRUE(q.empty());
    ASSERT_NO_THROW(q.remove(1));
}

TEST_F(BinaryHeapQTest, covVirtDtor)
{
    ASSERT_NO_THROW({
        ds::BinaryHeapQ<int> *ptr { new ds::BinaryHeapQ<int>(32) };
        if (ptr) delete ptr;
    });

    ASSERT_NO_THROW({
        ds::BinaryHeapQ<std::string> *ptr { new ds::BinaryHeapQ<std::string>(255) };
        if (ptr) delete ptr;
    });
}

TEST_F(BinaryHeapQTest, EmptyIL)
{
    const std::string exp_err{ "Empty initializer list was provided." };
    try {
        ds::BinaryHeapQ<int> q({});
    } catch(std::invalid_argument const &err) {
        ASSERT_EQ(err.what(), std::string(exp_err));
    } catch(...) {
        FAIL() << "Expected std::invalid_argument " + exp_err ;
    }
}

TEST_F(BinaryHeapQTest, EmptyVector)
{
    const std::string exp_err{ "Empty std::vector was provided." };
    const std::vector<int> v;
    ASSERT_TRUE(v.empty());
    try {
        ds::BinaryHeapQ<int> q(v);
    } catch(std::invalid_argument const &err) {
        ASSERT_EQ(err.what(), std::string(exp_err));
    } catch(...) {
        FAIL() << "Expected std::invalid_argument " + exp_err ;
    }
}

TEST_F(BinaryHeapQTest, EmptyHeap)
{
    const std::string exp_err{ "Empty Heap." };
    ds::BinaryHeapQ<int> q;
    ASSERT_TRUE(q.empty());

    try {
        q.peek();
    } catch(std::runtime_error const &err) {
        ASSERT_EQ(err.what(), std::string(exp_err));
    } catch(...) {
        FAIL() << "Expected std::runtime_error " + exp_err ;
    }

    try {
        q.poll();
    } catch(std::runtime_error const &err) {
        ASSERT_EQ(err.what(), std::string(exp_err));
    } catch(...) {
        FAIL() << "Expected std::runtime_error " + exp_err ;
    }
}

TEST_F(BinaryHeapQTest, testHeapProperty)
{
    const std::vector<int> nums {3, 2, 5, 6, 7, 9, 4, 8, 1};

    // Try manually creating heap
    ds::BinaryHeapQ<int> q;
    for (int n : nums) q.add(n);
    for (std::size_t i = 1; i <= nums.size(); i++) ASSERT_EQ(q.poll(), i);
    ASSERT_EQ(q.size(), 0);

    // Try heapify constructor
    q = ds::BinaryHeapQ<int>(nums);
    ASSERT_EQ(q.size(), nums.size());
    for (std::size_t i = 1; i <= nums.size(); i++) ASSERT_EQ(q.poll(), i);
    ASSERT_EQ(q.size(), 0);
}

TEST_F(BinaryHeapQTest, testHeapify)
{
    std::priority_queue<int, std::vector<int>, std::greater<int>> PQ;
    ds::BinaryHeapQ<int> pq;
    std::vector<int> rndm;
    for (std::size_t i = 1; i < LOOPS; i++) {
        rndm = gen::random<int>(i, INT_MIN, INT_MAX);
        pq   = ds::BinaryHeapQ<int>(rndm);
        PQ   = { rndm.begin(), rndm.end() };

        ASSERT_EQ(pq.size(), PQ.size());

        EXPECT_TRUE(pq.isMinHeap(0));
        while (!PQ.empty()) {
            ASSERT_EQ(pq.poll(), PQ.top());
            PQ.pop();
        }
    }
}

TEST_F(BinaryHeapQTest, testBasicsStr)
{
    ds::BinaryHeapQ<std::string> q {"aa", "bb", "cc", "dd", "ee"};
    q.clear();
    ASSERT_EQ(q.size(), 0);
    q.add("ff");
    ASSERT_EQ(q.size(), 1);
    ASSERT_EQ(q.peek(), "ff");
    ASSERT_EQ(q.poll(), "ff");
    ASSERT_EQ(q.size(), 0);
    ASSERT_TRUE(q.empty());
}

TEST_F(BinaryHeapQTest, testContainmentStr)
{
    ds::BinaryHeapQ<std::string> q {"aa", "bb", "cc", "dd", "ee"};
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
    EXPECT_TRUE(q.empty());
}

// hacky test since std::priority_queue does not have contains(), remove(val) methods
TEST_F(BinaryHeapQTest, testContainmentRandomized)
{
    std::priority_queue<int, std::vector<int>, std::greater<int>> PQ;
    ds::BinaryHeapQ<int> pq;
    std::vector<int> rndm;
    for (std::size_t i = 1; i < LOOPS; i++) {
        rndm = gen::random<int>(i, INT_MIN, INT_MAX);
        pq   = ds::BinaryHeapQ<int>(rndm);
        PQ   = { rndm.begin(), rndm.end() };

        for (const int &randVal : rndm) {
            ASSERT_EQ(pq.contains(randVal), true);
            EXPECT_TRUE(pq.remove(randVal));
            PQ.pop();
            ASSERT_EQ(pq.contains(randVal), false);
        }
    }
    ASSERT_TRUE(pq.empty());
}

TEST_F(BinaryHeapQTest, testRemovingDuplicates)
{
    ds::BinaryHeapQ<int> pq {2, 7, 2, 11, 7, 13, 2};
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
