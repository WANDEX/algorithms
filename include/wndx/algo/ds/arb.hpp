#pragma once
/// \brief thread-safe atomic ring buffer with static capacity.

#include <wndx/sane/log.hpp> // XXX

#include <algorithm>
#include <array>
#include <atomic>
#include <cstddef>              // size_t
// #include <cstdlib>              // std::abs
#include <initializer_list>
#include <iterator>

namespace wndx::algo {
namespace ds {

/// \brief thread-safe atomic ring buffer with static capacity.
// TODO: Очередь должна поддерживать конструктор копирования и оператор присваивания.
//       requires?
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

private:
    //////////////////////////////////////////////////////////////////
    /// helper functions

    /// \brief helper - get read index.
    inline constexpr size_t get_ridx() const noexcept
    {
        return m_ridx.load(std::memory_order_relaxed);
    }

    /// \brief helper - get write index.
    inline constexpr size_t get_widx() const noexcept
    {
        return m_widx.load(std::memory_order_relaxed);
    }

    /// \brief helper - increment read index with wrap around behavior.
    inline constexpr void inc_ridx() noexcept
    {
        m_ridx.store(get_ridx() + 1, std::memory_order_relaxed);
        if (get_ridx() >= CAPACITY) {
            m_ridx.store(0, std::memory_order_relaxed);
        }
    }

    /// \brief helper - increment write index with wrap around behavior.
    inline constexpr void inc_widx() noexcept
    {
        m_widx.store(get_widx() + 1, std::memory_order_relaxed);
        if (get_widx() >= CAPACITY) {
            m_widx.store(0, std::memory_order_relaxed);
        }
        // WNDX_LOG(wndx::sane::LL::DBUG, "widx: {} size:{}\n", get_widx(), size());
    }

    /// \brief helper - increment size with wrap around behavior.
    inline constexpr void inc_size() noexcept
    {
        if (size() + 1 > CAPACITY) {
            m_size.store(CAPACITY, std::memory_order_relaxed);
        } else {
            m_size.store(size() + 1, std::memory_order_relaxed);
        }
    }

    /// \brief helper - decrement size with wrap around behavior.
    inline constexpr void dec_size() noexcept
    {
        // FIXME: ?
        if (size() - 1 > CAPACITY) {
            m_size.store(CAPACITY, std::memory_order_relaxed);
        } else {
            m_size.store(size() - 1, std::memory_order_relaxed);
        }
    }

public:
    constexpr auto begin() const noexcept
    {
        return m_buf.begin();
    }

    constexpr auto end() const noexcept
    {
        return m_buf.end();
    }

    constexpr auto cbegin() const noexcept
    {
        return m_buf.cbegin();
    }

    constexpr auto cend() const noexcept
    {
        return m_buf.cend();
    }

    constexpr T operator[](size_t idx) const
    {
        return m_buf[idx];
    }

    constexpr void push(T const& data) noexcept
    {
        inc_size();
        WNDX_LOG(wndx::sane::LL::DBUG, "BEF widx: {} size:{}\n", get_widx(), size());
        m_buf[get_widx()] = data;
        // std::atomic_thread_fence(std::memory_order_acquire);
        inc_widx();
        // std::atomic_thread_fence(std::memory_order_release);
        WNDX_LOG(wndx::sane::LL::DBUG, "AFT widx: {} size:{}\n", get_widx(), size());
    }

    constexpr T pop() noexcept
    {
        dec_size();
        T data{ m_buf[get_ridx()] };
        inc_ridx();
        return data;
    }

    inline constexpr bool empty() const noexcept
    {
        return size() == 0;
    }

    inline constexpr bool error() const noexcept
    {
        return size() > CAPACITY;
    }

    inline constexpr bool full() const noexcept
    {
        return size() == CAPACITY;
    }

    inline constexpr size_t size() const noexcept
    {
        return m_size.load(std::memory_order_relaxed);
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
    std::atomic<size_t>     m_size{ 0 };
    std::atomic<size_t>     m_ridx{ 0 };
    std::atomic<size_t>     m_widx{ 0 };
    std::array<T, CAPACITY> m_buf{};
};

} // namespace ds
} // namespace wndx::algo
