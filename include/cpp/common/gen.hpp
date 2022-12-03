#pragma once

#include <array>
#include <cstddef>          // std::size_t
#include <random>
#include <unordered_set>
#include <vector>

namespace wndx {
namespace gen {

/**
 * @brief simple random number generator.
 *
 * @param  fr - range from the number (lower bound of the distribution).
 * @param  to - range to the number (upper bound of the distribution).
 * @return random number in range of distribution.
 */
inline double srng(const double fr, const double to)
{
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::uniform_real_distribution<> dist(fr, to);
    return dist(rng);
}

/**
 * @brief @return generate random unordered_set containing n unique elements.
 *
 * @param  fr - range from the number (lower bound of the distribution).
 * @param  to - range to the number (upper bound of the distribution).
 * @param  n  - size.
 */
template<typename T>
inline std::unordered_set<T> random_uset(const std::size_t n, const double fr, const double to)
{
    std::unordered_set<T> uset;
    uset.reserve(n);

    // TODO: if n > fr..to possible unique elements
    // => throw error ("impossible to generate enough unique elements for the given span fr..to")
    T rn {}; // randomly generated value
    for (std::size_t i = 0; i < n; i++) {
        do {
            rn = gen::srng(fr, to);
        } while(!((uset.insert(rn)).second));
        // ^ loop till successful insert of unique value
    }
    return uset;
}

/**
 * @brief generate random C array.
 *
 * @param  fr - range from the number (lower bound of the distribution).
 * @param  to - range to the number (upper bound of the distribution).
 * @param  n  - size.
 */
template<typename T>
inline void random(T out[], const std::size_t n, const double fr, const double to)
{
    for (std::size_t i = 0; i < n; i++)
        out[i] = gen::srng(fr, to);
}

/**
 * @brief generate random std::array.
 *
 * @param  fr - range from the number (lower bound of the distribution).
 * @param  to - range to the number (upper bound of the distribution).
 * @param  n  - size.
 */
template<typename T, const std::size_t n>
inline std::array<T, n> random(const double fr, const double to)
{
    std::array<T, n> out;
    for (std::size_t i = 0; i < n; i++)
        out.at(i) = gen::srng(fr, to);
    return out;
}

/**
 * @brief generate random std::vector.
 *
 * @param  unique=true vector will contain only unique elements.
 * @param  fr - range from the number (lower bound of the distribution).
 * @param  to - range to the number (upper bound of the distribution).
 * @param  n  - size.
 */
template<typename T>
inline std::vector<T> random(const std::size_t n, const double fr, const double to, const bool unique=false)
{
    if (unique) {
        std::unordered_set<T> uset{ gen::random_uset<T>(n, fr, to) };
        return{ uset.begin(), uset.end() };
    }

    std::vector<T> out;
    out.resize(n);
    for (std::size_t i = 0; i < n; i++)
        out.at(i) = gen::srng(fr, to);
    return out;
}

} // namespace gen
} // namespace wndx
