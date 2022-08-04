#pragma once

#include <cstddef>  // std::size_t
#include <array>    // std::array
#include <string>
#include <iostream>

/**
 * print C array
 */
inline void print_array(const int A[], const std::size_t n, const std::string prefix="")
{
    std::cout << prefix << "{";
    for (std::size_t i = 0; i < n - 1; i++)
        std::cout << A[i] << ", ";
    std::cout << A[n - 1] << "}" << std::endl; // last element without additional ', '
}

/**
 * print std::array
 */
template <const std::size_t n>
inline void print_array(const std::array<int, n> &A, const std::string prefix="")
{
    std::cout << prefix << "{";
    for (std::size_t i = 0; i < n - 1; i++)
        std::cout << A[i] << ", ";
    std::cout << A[n - 1] << "}" << std::endl; // last element without additional ', '
}
