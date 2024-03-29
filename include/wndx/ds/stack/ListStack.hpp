#pragma once

#include "IStack.hpp"

#include <cstddef>              // std::size_t
#include <forward_list>
#include <stdexcept>            // std::runtime_error

namespace wndx {
namespace ds {

template<typename T>
class ListStack : public IStack<T>
{
protected:
    std::size_t m_size {0};
    std::forward_list<T> stack {};

public:
    // create an empty stack
    ListStack()
    {}
    // create a stack with an initial element
    ListStack(const T &elem)
    {
        push(elem);
    }

    virtual ~ListStack() = default;

    /**
     * Empty this stack, O(n)
     */
    void clear()
    {
        stack.clear();
    }

    /**
     * return the size of the stack, O(1)
     */
    virtual std::size_t size() const
    {
        return m_size;
    }

    /**
     * check that stack is empty, O(1)
     */
    virtual constexpr bool empty() const
    {
        return stack.empty();
    }

    /**
     * add elem at the top of the stack, O(1)
     */
    virtual void push(const T &elem)
    {
        stack.push_front(elem);
        ++m_size;
    }

    /**
     * return & remove elem at the top of the stack, O(1).
     * throws error if the stack is empty.
     */
    virtual T pop()
    {
        if (empty()) {
            throw std::runtime_error("Empty Stack");
        }
        T data { stack.front() }; // tmp store the data
        stack.pop_front();
        --m_size;
        return data;
    }

    /**
     * return elem at the top of the stack, O(1).
     * throws error if the stack is empty.
     */
    virtual T peek() const
    {
        if (empty()) {
            throw std::runtime_error("Empty Stack");
        }
        return stack.front();
    }

};

} // namespace ds
} // namespace wndx
