#pragma once

#include <array>
#include <cstddef>              // std::size_t
#include <iostream>
#include <string>
#include <vector>

namespace wndx::algo {
namespace pp { // pretty print

/**
 * print C array
 */
template <typename T>
inline void print(const T *a, const std::size_t n, const std::string prefix="")
{
    if (n < 1) {
        std::cout << prefix << "{}" << std::endl;
        return;
    }
    std::cout << prefix << "{";
    for (std::size_t i = 0; i < n - 1; i++)
        std::cout << a[i] << ", ";
    std::cout << a[n - 1] << "}" << std::endl; // last element without additional ', '
}

/**
 * print std::array
 */
template <typename T, std::size_t n>
inline void print(const std::array<T, n> &a, const std::string prefix="")
{
    if (n < 1) {
        std::cout << prefix << "{}" << std::endl;
        return;
    }
    std::cout << prefix << "{";
    for (std::size_t i = 0; i < n - 1; i++)
        std::cout << a[i] << ", ";
    std::cout << a[n - 1] << "}" << std::endl; // last element without additional ', '
}

/**
 * print std::vector
 */
template <typename T>
inline void print(const std::vector<T> &a, const std::string prefix="")
{
    const std::size_t n = a.size();
    if (n < 1) {
        std::cout << prefix << "{}" << std::endl;
        return;
    }
    std::cout << prefix << "{";
    for (std::size_t i = 0; i < n - 1; i++)
        std::cout << a[i] << ", ";
    std::cout << a[n - 1] << "}" << std::endl; // last element without additional ', '
}

} // namespace pp
} // namespace wndx::algo
