#pragma once

#include <array>
#include <cmath>                // std::abs, std::fabs
#include <cstddef>              // std::size_t
#include <cstdint>
#include <random>
#include <stdexcept>            // std::invalid_argument
#include <unordered_set>
#include <vector>

#ifndef WNDX_MSC // MSC/WIN32 specific - enforced by the OS-env!
#if defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
    #define WNDX_MSC 1
#else
    #define WNDX_MSC 0
#endif//defined ...
#endif//WNDX_MSC

#ifndef WNDX_ALGO_FIXED_SEED
#define WNDX_ALGO_FIXED_SEED 1
#endif//WNDX_ALGO_FIXED_SEED

#if WNDX_MSC && !WNDX_ALGO_FIXED_SEED
#include <chrono>
#endif//WNDX_MSC

namespace wndx::algo {
namespace gen {

/**
 * @brief construct once a static/global std::random_device and engine.
 * Because constructing them on every call is expensive and can degrade randomness on some platforms.
 */
inline std::mt19937& g_rng()
{
#if WNDX_MSC || WNDX_ALGO_FIXED_SEED
    // deterministic seeding! std::random_device => drmemory "WARNING: writing to readonly memory".
    // because of the SH*TTY WIN random_* implementation: BCryptGenRandom, CryptGenRandom, etc.
  #if WNDX_ALGO_FIXED_SEED
    // constexpr std::size_t seed{ 9876543210 }; // fixed seed (value chosen arbitrarily)
    // constexpr std::uint64_t seed{ 9876543210 }; // fixed seed (value chosen arbitrarily)
    constexpr auto seed{ static_cast<std::uint32_t>(9876543210) }; // fixed seed (value chosen arbitrarily)
  #else
    static const auto now{ std::chrono::steady_clock::now().time_since_epoch() };
    // static const std::uint64_t seed{ static_cast<std::uint64_t>(
        // std::chrono::duration_cast<std::chrono::seconds>(now).count()
    // )};
    static const auto seed{ static_cast<std::uint32_t>(
        std::chrono::duration_cast<std::chrono::seconds>(now).count()
    )};
  #endif//WNDX_ALGO_FIXED_SEED
    static std::mt19937 s_rng(seed);
#else
    static std::random_device s_rd;
    static std::mt19937 s_rng(s_rd());
#endif
    return s_rng;
}

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
    // NOTE: static_cast is essential, otherwise - ambiguous call to overloaded function.
    if constexpr (std::is_floating_point_v<T>) {
        std::uniform_real_distribution<T> dist(fr, to);
        return static_cast<T>(dist(g_rng()));
    } else if constexpr (std::is_integral_v<T> && sizeof(T) == 1) {
        std::uniform_int_distribution<int16_t> dist(fr, to); // smallest supported type
        return static_cast<T>(dist(g_rng()) & 0xFF); // width bitmask: int8_t, uint8_t
    } else if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> dist(fr, to);
        return static_cast<T>(dist(g_rng()));
    } else {
        return 0; // => not supported type
    }
}

template<typename T>
constexpr inline std::size_t calc_nums_in_range(const T fr, const T to)
{
    if constexpr (std::is_floating_point_v<T>) {
        constexpr std::size_t mul{ 1'000'000'000 }; // value chosen arbitrarily
        return static_cast<std::size_t>(std::fabs(fr - to) * mul);
    } else if constexpr (std::is_integral_v<T> && sizeof(T) == 1) {
        return static_cast<std::size_t>(std::abs(
            static_cast<std::int16_t>(fr) - static_cast<std::int16_t>(to)
        ) & 0xFF); // width bitmask: int8_t, uint8_t
    } else if constexpr (std::is_integral_v<T>) {
        return static_cast<std::size_t>(std::abs(
            static_cast<std::int64_t>(fr) - static_cast<std::int64_t>(to)
        )); // cast all unsigned std::abs args to the bigger signed integrals!
    } else {
        return 0; // => not supported type
    }
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
    const std::size_t nums_in_range{ calc_nums_in_range<T>(fr, to) };
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
    return std::move(uset);
}

/**
 * @brief random fill C-style array.
 *
 * @param  fr - range from the number (lower bound of the distribution).
 * @param  to - range to the number (upper bound of the distribution).
 * @param  n  - size.
 */
template<typename T>
inline void random(T *out, const std::size_t n, const T fr, const T to)
{
    for (std::size_t i = 0; i < n; i++) {
        out[i] = gen::srng<T>(fr, to);
    }
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
    for (std::size_t i = 0; i < n; i++) {
        out.at(i) = gen::srng<T>(fr, to);
    }
    return std::move(out);
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
    std::vector<T> out(n);
    for (std::size_t i = 0; i < n; i++) {
        out.at(i) = gen::srng<T>(fr, to);
    }
    return std::move(out);
}

} // namespace gen
} // namespace wndx::algo