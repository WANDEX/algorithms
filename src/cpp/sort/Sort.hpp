#pragma once

#include <cstddef>  // std::size_t
#include <array>    // std::array

template <typename A, const std::size_t n>
class Sort
{
protected:
    A* a { nullptr }; // ptr on array to sort
// TODO: enum with available sort algorithm names
public:
    Sort(A* a) : a{ a } {}
    ~Sort() { a = nullptr; }

    void sort_ins();

    /**
     * swap pointers
     */
    void swap(auto* p, auto* q)
    {
        auto tp { *p }; // tmp ptr
        *p = *q;
        *q = tp;
    }

};

/**
 * Sort array using insertion sort.
 * Concept: if array is sorted from [0, i] => insert at i+1.
 */
template <typename A, const std::size_t n>
void Sort<A, n>::sort_ins()
{
    for (std::size_t i = 1; i < n; i++) {
        for (std::size_t j = i; j > 0 && a[j] < a[j - 1]; j--)
            swap(&a[j - 1], &a[j]);
    }
}
