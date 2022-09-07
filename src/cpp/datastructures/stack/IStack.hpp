#pragma once

#include <cstddef>      // size_t

// Interface of the Stack data structure
template<typename T>
class IStack
{
public:
    // return the number of elements in the stack
    virtual std::size_t size() const = 0;
    // return if the stack is empty
    virtual bool isEmpty() const = 0;
    // push the element on the stack
    virtual void push(const T &elem) = 0;
    // pop the element off the stack
    virtual T pop() = 0;
    virtual T peek() const = 0;
    // virtual destructor in case we delete an IStack pointer,
    // so the proper derived destructor is called
    virtual ~IStack() {}
};