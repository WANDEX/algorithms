#pragma once
/// \brief thread-safe atomic ring buffer with static capacity.
///
/// AUTHOR : github.com/WANDEX
/// LICENSE: MIT
///
/// \see
///   https://en.wikipedia.org/wiki/Circular_buffer
///   https://en.cppreference.com/cpp/atomic/atomic
///   https://en.cppreference.com/cpp/atomic/atomic_thread_fence
///
/// \see ref NOTE
///   https://en.cppreference.com/cpp/atomic/memory_order
///
/// load & store:
///   memory_order_relaxed - no synchronization or ordering constraints
///   memory_order_seq_cst - load performs a acquire operation, store performs a release operation,
///     read-modify-write performs both an acquire operation and a release operation,
///     plus a single total order exists in which all threads observe all modifications in the same order.
///
/// load:
///   memory_order_acquire - no reads or writes in the current thread can be reordered before this load.
///     All writes in other threads that release the same atomic variable are visible in the current thread.
///
/// store:
///   memory_order_release - no reads or writes in the current thread can be reordered after this store.
///     All writes in the current thread are visible in other threads that acquire the same atomic variable.
///     And writes that carry a dependency into the atomic variable become visible in other threads

#include <array>
#include <atomic>
#include <concepts>             // std::copyable
#include <cstddef>              // size_t
#include <initializer_list>
#include <type_traits>          // std::is_trivially_copyable

namespace wndx::algo {
namespace ds {

/// \brief concept which satisfy to atomic constrains:
///   TriviallyCopyable, CopyConstructible, CopyAssignable.
/// \see
///   https://en.cppreference.com/cpp/atomic/atomic
///   https://en.cppreference.com/cpp/concepts/copyable.html
///   https://en.cppreference.com/cpp/types/is_trivially_copyable.html
template<typename T>
concept arb_copyable = std::is_trivially_copyable<T>::value && std::copyable<T>;

/// \brief thread-safe atomic ring buffer with static capacity.
template<typename T, size_t CAPACITY>
requires arb_copyable<T>
class arb final {
public:
    arb() noexcept                      = default;
    arb(arb&&) noexcept                 = default;
    arb(arb const&) noexcept            = default;
    arb& operator=(arb&&) noexcept      = default;
    arb& operator=(arb const&) noexcept = default;
    ~arb() noexcept                     = default;

    explicit constexpr arb(std::initializer_list<T> const& il) noexcept
    {
        for (T e : il) {
            push(e);
        }
    }

    explicit constexpr arb(std::array<T, CAPACITY> const& a) noexcept
    {
        for (T e : a) {
            push(e);
        }
    }

    /// \brief ctor for passing generic begin(), end() iterators.
    template<class InputIt = std::array<T, CAPACITY>::const_iterator>
    explicit constexpr arb(InputIt begin, InputIt end) noexcept
    {
        while(begin != end) {
            push(*begin++);
        }
    }

    //////////////////////////////////////////////////////////////////
    /// helper functions

    /// \brief helper - do not use except in the single-threaded tests.
    constexpr T operator[](size_t idx) const
    {
        return m_buf[idx];
    }

private:
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
        if (++m_ridx >= CAPACITY) {
            m_ridx.store(0, std::memory_order_relaxed);
        }
    }

    /// \brief helper - increment write index with wrap around behavior.
    inline constexpr void inc_widx() noexcept
    {
        if (++m_widx >= CAPACITY) {
            m_widx.store(0, std::memory_order_relaxed);
        }
    }

    /// \brief helper - increment size with wrap around behavior.
    inline constexpr void inc_size() noexcept
    {
        if (++m_size > CAPACITY) {
            m_size.store(CAPACITY, std::memory_order_relaxed);
        }
    }

    /// \brief helper - decrement size with wrap around behavior.
    inline constexpr void dec_size() noexcept
    {
        if (--m_size > CAPACITY) {
            m_size.store(CAPACITY, std::memory_order_relaxed);
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

    /// \brief push data into ring buffer write index.
    ///
    /// \param data to store.
    constexpr void push(T const& data) noexcept
    {
        inc_size();
        m_buf[get_widx()] = data;
        inc_widx();
    }

    /// \brief pop data from ring buffer read index.
    ///
    /// Does immediate cleanup after pop for cleaning ring buffer data,
    /// additional extra write is being done, for replacing potentially
    /// sensitive content in memory with the default value for the type.
    ///
    /// \return popped data.
    constexpr T pop() noexcept
    {
        dec_size();
        T data{ m_buf[get_ridx()] };
        m_buf[get_ridx()] = {}; // cleanup by replacing with default.
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
