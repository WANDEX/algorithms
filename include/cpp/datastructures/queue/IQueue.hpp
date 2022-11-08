#pragma once

#include <cstddef>      // size_t

namespace wndx {
namespace ds {

// Interface of the Queue data structure
template<typename T>
class IQueue
{
public:
    // return the number of elements in the queue
    virtual std::size_t size() const = 0;

    // return if the queue is empty
    virtual bool isEmpty() const = 0;

    // enqueue the element to the back of the queue
    virtual void enqueue(const T &elem) = 0;

    // dequeue the element from the front of the queue
    virtual T dequeue() = 0;

    // peek the element at the front of the queue
    virtual T peek() const = 0;

    // virtual destructor in case we delete an IQueue pointer,
    // so the proper derived destructor is called
    virtual ~IQueue() {}
};

} // namespace ds
} // namespace wndx
