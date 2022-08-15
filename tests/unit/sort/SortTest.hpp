#pragma once

#include "Sort.hpp"

#include "print.hpp" // XXX

#include <algorithm> // std::stable_sort, std::copy_n

#include <cstddef>   // std::size_t
#include <cstring>   // std::memcmp

template <typename A, const std::size_t n>
class SortTest: public Sort<A, n>
{
protected:
    A* m_e { nullptr }; // ptr on expected array after sort
public:
    SortTest(A* a)
        : Sort<A, n> { a }
    {
        m_e = new A[n]; // allocate memory for the array
        std::copy_n(this->a, n, m_e); // deep copy array
        // sort member array for later comparison with the result
        std::stable_sort(m_e, m_e + n, std::less<A>());
    }
    ~SortTest()
    {
        delete[] m_e;
    }
public:
    virtual bool are_equal() const;
    // check that sort() gives the same result as std::sort()
    virtual bool sort_test() const;
};

template <typename A, const std::size_t n>
bool SortTest<A, n>::are_equal() const
{
    return std::memcmp(this->a, m_e, n * sizeof(this->a[0])) == 0;
}

template <typename A, const std::size_t n>
bool SortTest<A, n>::sort_test() const
{
    return are_equal();
}
