#pragma once

#include "IQueue.hpp"

#include <cstddef>              // size_t
#include <list>
#include <stdexcept>            // std::runtime_error

namespace wndx {
namespace ds {

template<typename T>
class ListQueue : public IQueue<T>
{
protected:
    std::list<T> queue {};

public:
    // create an empty queue
    ListQueue()
    {}
    // create a queue with an initial element
    ListQueue(const T &elem)
    {
        enqueue(elem);
    }

    virtual ~ListQueue() = default;

    /**
     * Empty this queue, O(n)
     */
    void clear()
    {
        queue.clear();
    }

    /**
     * return the size of the queue, O(1)
     */
    virtual std::size_t size() const
    {
        return queue.size();
    }

    /**
     * check that queue is empty, O(1)
     */
    virtual constexpr bool empty() const
    {
        return queue.empty();
    }

    /**
     * enqueue the element to the back of the queue, O(1)
     */
    virtual void enqueue(const T &elem)
    {
        queue.push_back(elem);
    }

    /**
     * dequeue the element from the front of the queue, O(1).
     * throws error if the queue is empty.
     */
    virtual T dequeue()
    {
        if (empty()) {
            throw std::runtime_error("Empty Queue");
        }
        T data { queue.front() }; // tmp store the data
        queue.pop_front();
        return data;
    }

    /**
     * return elem at the front of the queue, O(1).
     * throws error if the queue is empty.
     */
    virtual T peek() const
    {
        if (empty()) {
            throw std::runtime_error("Empty Queue");
        }
        return queue.front();
    }

};

} // namespace ds
} // namespace wndx
