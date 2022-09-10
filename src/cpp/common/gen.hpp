#pragma once

#include <array>
#include <cstddef>          // std::size_t
#include <random>
#include <unordered_set>
#include <vector>

namespace gen {

/**
 * generate random unordered_set containing n unique elements.
 * n: size, limits: fr ... to
 */
template<typename T>
inline std::unordered_set<T> random_uset(const std::size_t n, const double fr, const double to)
{
    if (n < 1) return {};
    std::unordered_set<T> uset;
    uset.reserve(n);
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::uniform_real_distribution<> dist(fr, to);

    // TODO: if n > fr..to possible unique elements
    // => throw error ("impossible to generate enough unique elements for the given span fr..to")
    T rn {}; // randomly generated value
    for (std::size_t i = 0; i < n; i++) {
        do {
            rn = dist(rng);
        } while(!((uset.insert(rn)).second));
        // ^ loop till successful insert of unique value
    }
    return std::move(uset);
}

/**
 * generate random C array.
 * n: size, limits: fr ... to
 */
inline void random(auto out[], const std::size_t n, const double fr, const double to)
{
    if (n < 1) return;
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
    if (n < 1) return {};
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
 * if unique=true vector will contain only unique elements.
 */
template<typename T>
inline std::vector<T> random(const std::size_t n, const double fr, const double to, const bool unique=false)
{
    if (n < 1) return {};
    if (unique) {
        std::unordered_set<T> uset = gen::random_uset<T>(n, fr, to);
        std::vector<T> out { uset.begin(), uset.end() };
        return std::move(out);
    }

    std::vector<T> out;
    out.reserve(n);
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::uniform_real_distribution<> dist(fr, to);
    for (std::size_t i = 0; i < n; i++)
        out[i] = dist(rng);
    return std::move(out);
}

} // namespace gen
