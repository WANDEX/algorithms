#pragma once
/// \brief thread-safe atomic ring buffer with static capacity.

#include <algorithm>
#include <array>
#include <atomic>
#include <cstddef>              // size_t
#include <initializer_list>
#include <iterator>

#ifndef WNDX_ALGO_ARB_THREAD_SAFETY_DISABLE
#define WNDX_ALGO_ARB_THREAD_SAFETY_DISABLE 0
#endif//WNDX_ALGO_ARB_THREAD_SAFETY_DISABLE

namespace wndx::algo {
namespace ds {

/// \brief thread-safe atomic ring buffer with static capacity.
template<typename T, size_t CAPACITY>
class arb final {
public:
    arb() noexcept                      = default;
    arb(arb&&) noexcept                 = default;
    arb(arb const&) noexcept            = default;
    arb& operator=(arb&&) noexcept      = default;
    arb& operator=(arb const&) noexcept = default;
    ~arb() noexcept                     = default;

    explicit arb(std::initializer_list<T> const& il) noexcept
    {
        for (const T &e : il) {
            push(e);
        }
    }

    explicit arb(std::array<T, CAPACITY> const& a) noexcept
    {
        for (const T &e : a) {
            push(e);
        }
    }

    /// \brief ctor for passing generic begin(), end() iterators.
    template<class InputIt = std::array<T, CAPACITY>::const_iterator>
    explicit arb(InputIt begin, InputIt end) noexcept
    {
        while(begin != end) {
            push(*begin++);
        }
    }

    constexpr void push(T const& data) noexcept
    {
#if WNDX_ALGO_ARB_THREAD_SAFETY_DISABLE
        m_buf[m_widx & (CAPACITY - 1)] = data;
        m_widx++;
#else
        m_buf[       m_widx.load(std::memory_order_relaxed) & (CAPACITY - 1)] = data;
        m_widx.store(m_widx.load(std::memory_order_relaxed) + 1, std::memory_order_relaxed);
#endif
    }

    constexpr T pop() noexcept
    {
#if WNDX_ALGO_ARB_THREAD_SAFETY_DISABLE
        T data{ m_buf[m_ridx & (CAPACITY - 1)] };
        m_ridx++;
#else
        T data{ m_buf[m_ridx.load(std::memory_order_relaxed) & (CAPACITY - 1)] };
        m_ridx.store( m_ridx.load(std::memory_order_relaxed) + 1, std::memory_order_relaxed);
#endif
        return data;
    }

    inline constexpr bool empty() const noexcept
    {
        return size() == 0;
    }

    inline constexpr size_t size() const noexcept
    {
        return m_widx.load(std::memory_order_relaxed) - m_ridx.load(std::memory_order_relaxed);
    }

    inline constexpr size_t available_size() const noexcept
    {
        return CAPACITY - size();
    }

    inline constexpr size_t capacity() const noexcept
    {
        return CAPACITY;
    }

private:
    std::atomic<size_t>     m_ridx{ 0 };
    std::atomic<size_t>     m_widx{ 0 };
    std::array<T, CAPACITY> m_buf{};
};

} // namespace ds
} // namespace wndx::algo
