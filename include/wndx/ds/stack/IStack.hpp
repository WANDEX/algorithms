#pragma once

#include <cstddef>              // std::size_t

namespace wndx {
namespace ds {

// Interface of the Stack data structure
template<typename T>
class IStack
{
public:
    // return the number of elements in the stack
    virtual std::size_t size() const = 0;

    // return if the stack is empty
    virtual constexpr bool empty() const = 0;

    // push the element on the stack
    virtual void push(const T &elem) = 0;

    // pop the element off the stack
    virtual T pop() = 0;

    // peek at the top of the stack
    virtual T peek() const = 0;

};

} // namespace ds
} // namespace wndx
