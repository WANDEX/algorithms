#pragma once

#include <array>
#include <cmath>                // std::abs
#include <cstddef>              // std::size_t
#include <random>
#include <stdexcept>            // std::invalid_argument
#include <unordered_set>
#include <vector>

namespace wndx::algo {
namespace gen {

/**
 * @brief simple random number generator.
 *
 * @param  fr - range from the number (lower bound of the distribution).
 * @param  to - range to the number (upper bound of the distribution).
 * @return random number in range of distribution.
 */
template<typename T>
inline T srng(const T fr, const T to)
{
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::uniform_real_distribution<double> dist(static_cast<double>(fr), static_cast<double>(to));
    return static_cast<T>(dist(rng));
}

/**
 * @brief @return generate random unordered_set containing n unique elements.
 *
 * @param  fr - range from the number (lower bound of the distribution).
 * @param  to - range to the number (upper bound of the distribution).
 * @param  n  - size.
 */
template<typename T>
inline std::unordered_set<T> random_uset(const std::size_t n, const T fr, const T to)
{
    std::unordered_set<T> uset(n);
    // TODO: different check/formula (for double/float T the real range is bigger)
    const std::size_t nums_in_range{ static_cast<size_t>(std::abs(to) + std::abs(fr)) };
    // if n > fr..to range of possible unique elements => impossible (insufficient_range)
    // => Not enough unique elements in range fr..to
    if (n > nums_in_range)
        throw std::invalid_argument("Insufficient range");
    T rn {};
    for (std::size_t i = 0; i < n; i++) {
        do {
            rn = gen::srng<T>(fr, to);
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
inline void random(T *out, const std::size_t n, const T fr, const T to)
{
    for (std::size_t i = 0; i < n; i++)
        out[i] = gen::srng<T>(fr, to);
}

/**
 * @brief generate random std::array.
 *
 * @param  fr - range from the number (lower bound of the distribution).
 * @param  to - range to the number (upper bound of the distribution).
 * @param  n  - size.
 */
template<typename T, std::size_t n>
inline std::array<T, n> random(const T fr, const T to)
{
    std::array<T, n> out;
    for (std::size_t i = 0; i < n; i++)
        out.at(i) = gen::srng<T>(fr, to);
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
inline std::vector<T> random(const std::size_t n, const T fr, const T to, const bool unique=false)
{
    if (unique) {
        std::unordered_set<T> uset{ gen::random_uset<T>(n, fr, to) };
        return{ uset.begin(), uset.end() };
    }

    std::vector<T> out;
    out.resize(n);
    for (std::size_t i = 0; i < n; i++)
        out.at(i) = gen::srng<T>(fr, to);
    return out;
}

} // namespace gen
} // namespace wndx::algo
