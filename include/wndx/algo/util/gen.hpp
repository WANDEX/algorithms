#pragma once

#include <array>
#include <cmath>                // std::abs
#include <cstddef>              // std::size_t
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

namespace wndx::algo {
namespace gen {

/**
 * @brief construct once a static/global std::random_device and engine.
 * Because constructing them on every call is expensive and can degrade randomness on some platforms.
 */
inline std::mt19937& g_rng()
{
#if WNDX_MSC
    // deterministic seeding! std::random_device => drmemory "WARNING: writing to readonly memory".
    // because of the SH*TTY WIN random_* implementation: BCryptGenRandom, CryptGenRandom, etc.
    constexpr int seed{ 123456 };
    static std::mt19937 s_rng(seed);
#else
    static std::random_device s_rd;
    static std::mt19937 s_rng(s_rd());
#endif
    return s_rng;
}

#if 0
/**
 * @brief simple random number generator.
 *
 * @param  fr - range from the number (lower bound of the distribution).
 * @param  to - range to the number (upper bound of the distribution).
 * @return random number in range of distribution.
 *
 * NOTE: construcs a new std::random_device and engine on every call
 * => expensive and can degrade randomness on some platforms.
 */
template<typename T>
inline T srng(const T fr, const T to)
{
#if WNDX_MSC
    // deterministic seeding! std::random_device => drmemory error "UNADDRESSABLE ACCESS: reading".
    // because of the SH*TTY WIN random_* implementation: BCryptGenRandom, CryptGenRandom, etc.
    constexpr int seed{ 123456 };
    std::default_random_engine rng(seed);
#else
    std::random_device rd;
    std::default_random_engine rng(rd());
#endif
    std::uniform_real_distribution<double> dist(static_cast<double>(fr), static_cast<double>(to));
    return static_cast<T>(dist(rng));
}
#else
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
    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T>  dist(fr, to);
        return dist(g_rng());
    } else {
        std::uniform_real_distribution<T> dist(fr, to);
        return dist(g_rng());
    }
}
#endif

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
    // if (n == 0) return {}; // XXX
    if (unique) {
        std::unordered_set<T> uset{ gen::random_uset<T>(n, fr, to) };
        return{ uset.begin(), uset.end() };
    }
#if 1
    std::vector<T> out(n);
#else
    std::vector<T> out;
    out.reserve(n); // allocate space but keep size = 0
    // out.resize(n);  // size bocomes n
#endif
    // std::random_device rd;
    // std::mt19937 rng(rd());
#if 0
    // drmemory UNADDRESSABLE ACCESS: reading
    std::uniform_int_distribution<T> dist(fr, to);
    std::generate_n(out.begin(), n, [&rng, &dist]() { return dist(rng); });
#else
    for (std::size_t i = 0; i < n; i++) {
    #if 1
        out.at(i) = gen::srng<T>(fr, to);
        // out[i] = gen::srng<T>(fr, to);
    #elif 1
        out[i] = static_cast<T>(to); // XXX
    #elif 1
        std::uniform_int_distribution<T> dist(fr, to);
        T val = dist(rng);
        out[i] = val;
    #elif 0
        std::uniform_int_distribution<T> dist; // default ctor
        dist.param(typename std::uniform_int_distribution<T>::param_type(fr, to));
        out[i] = dist(rng);
    #else
        T val = gen::srng<T>(fr, to); // ensure a copy, not a reference | copy from read-only source
        out[i] = val;
    #endif
    }
#endif
    return std::move(out);
    // return out; // rely on NRVO (named return value optimization)
}

} // namespace gen
} // namespace wndx::algo
