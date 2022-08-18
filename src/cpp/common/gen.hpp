#pragma once

#include <array>
#include <cstddef>      // std::size_t
#include <random>
#include <vector>

namespace gen {

/**
 * generate random C array.
 * n: size, limits: fr ... to
 */
inline void random(auto out[], const std::size_t n, const double fr, const double to)
{
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::uniform_real_distribution<> dist(fr, to);
    for (std::size_t i = 0; i < n; i++)
        out[i] = dist(rng);
}

/**
 * generate random std::array.
 * n: size, limits: fr ... to
 */
template<typename T, const std::size_t n>
inline std::array<T, n> random(const double fr, const double to)
{
    std::array<T, n> out;
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::uniform_real_distribution<> dist(fr, to);
    for (std::size_t i = 0; i < n; i++)
        out[i] = dist(rng);
    return std::move(out);
}

/**
 * generate random std::vector.
 * n: size, limits: fr ... to
 */
template<typename T>
inline std::vector<T> random(const std::size_t n, const double fr, const double to)
{
    std::vector<T> out;
    out.reserve(n);
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::uniform_real_distribution<> dist(fr, to);
    for (std::size_t i = 0; i < n; i++)
        out.push_back(dist(rng));
    return std::move(out);
}

} // namespace gen
