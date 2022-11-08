#pragma once
/**
 * @brief Binary Indexed Tree implementation which supports point updates and sum range queries.
 *
 * Binary Indexed Tree (Fenwick Tree) data structure that can
 * efficiently update elements and calculate prefix sums in a table of numbers.
 * Structure was proposed by Boris Ryabko in 1989.
 */

#include <cstddef>              // std::size_t
#include <initializer_list>
#include <iostream>
#include <stdexcept>            // std::invalid_argument
#include <utility>              // std::move
#include <vector>

namespace wndx {

class BITreeRangeQueryPointUpdate
{
protected:
    using Ti = int; // alias just in case default int is not enough!

    // The size of the array holding the Fenwick tree values
    std::size_t m_sz {0};

    std::vector<Ti> m_tree;

public:
    /**
     * @brief Create an empty Binary Indexed Tree with 'sz' parameter zero based.
     */
    BITreeRangeQueryPointUpdate(const std::size_t sz)
        : m_sz{ sz + 1 }
    {
        m_tree.resize(m_sz);
        m_tree.at(0) = {};
    }

    /**
     * @brief Construct a Binary Indexed Tree with an initial set of values.
     *
     * @param il std::initializer_list
     * The 'values' array MUST BE ONE BASED meaning values[0]
     * does not get used, O(n) construction.
     */
    BITreeRangeQueryPointUpdate(const std::initializer_list<Ti> &il)
        : m_sz{ il.size() }, m_tree{ il }
    {
        if (m_sz < 1) throw std::invalid_argument("size() < 1");
        m_tree.resize(m_sz);
        m_tree.at(0) = {};
        for (std::size_t i = 1; i < m_sz; i++) {
            const std::size_t parent { i + lsb(i) };
            if (parent < m_sz) m_tree.at(parent) += m_tree.at(i);
        }
    }

    BITreeRangeQueryPointUpdate(const std::vector<Ti> &v)
        : m_sz{ v.size() }, m_tree{ v }
    {
        if (m_sz < 1) throw std::invalid_argument("size() < 1");
        m_tree.resize(m_sz);
        m_tree.at(0) = {};
        for (std::size_t i = 1; i < m_sz; i++) {
            const std::size_t parent { i + lsb(i) };
            if (parent < m_sz) m_tree.at(parent) += m_tree.at(i);
        }
    }

    BITreeRangeQueryPointUpdate() = default;
    BITreeRangeQueryPointUpdate(BITreeRangeQueryPointUpdate &&) = default;
    BITreeRangeQueryPointUpdate(const BITreeRangeQueryPointUpdate &) = default;
    BITreeRangeQueryPointUpdate &operator=(BITreeRangeQueryPointUpdate &&) = default;
    BITreeRangeQueryPointUpdate &operator=(const BITreeRangeQueryPointUpdate &) = default;
    virtual ~BITreeRangeQueryPointUpdate() = default;

public:
    auto cbegin() const noexcept { return m_tree.cbegin(); }
    auto cend()   const noexcept { return m_tree.cend();   }
    auto begin()  const noexcept { return m_tree.begin();  }
    auto end()    const noexcept { return m_tree.end();    }

private:

    /**
     * @brief @return value of the least significant bit (LSB).
     *
     * @param i index.
     * lsb(108) = lsb(0b1101100) =     0b100 = 4  .
     * lsb(104) = lsb(0b1101000) =    0b1000 = 8  .
     * lsb(96)  = lsb(0b1100000) =  0b100000 = 32 .
     * lsb(64)  = lsb(0b1000000) = 0b1000000 = 64 .
     */
    Ti lsb(const std::size_t i) const
    {
        return i & -i; // isolate the lowest one bit value
    }

    /**
     * @brief @return Computes the prefix sum from [1, i], O(log(n)).
     *
     * @param i index.
     */
    Ti prefixSum(std::size_t i) const
    {
        Ti sum {0};
        while (i != 0) {
            sum += m_tree.at(i);
            i &= ~lsb(i); // Equivalently, i -= lsb(i);
        }
        return sum;
    }

public:

    /**
     * @brief @return sum of the interval [left, right], O(log(n)).
     *
     * @param  left index.
     * @param right index.
     */
    Ti sum(const std::size_t left, const std::size_t right) const
    {
        if (right < left) throw std::invalid_argument("Make sure right >= left");
        return prefixSum(right) - prefixSum(left - 1);
    }

    /**
     * @brief add 'v' to index 'i', O(log(n)).
     *
     * @param i index.
     * @param v value.
     */
    void add(std::size_t i, const Ti v)
    {
        while (i < m_sz) {
            m_tree.at(i) += v;
            i += lsb(i);
        }
    }

    /**
     * @brief @return get the value at index i.
     *
     * @param i index.
     */
    Ti get(const std::size_t i) const
    {
        return sum(i, i);
    }

    /**
     * @brief set index i to be equal to v, O(log(n)).
     *
     * @param i index.
     * @param v value.
     */
    void set(const std::size_t i, const Ti v)
    {
        add(i, v - sum(i, i));
    }

    /**
     * @brief print Binary Indexed Tree.
     */
    void print() const
    {
        std::cout << "{";
        for (std::size_t i = 0; i < m_sz - 1; i++)
            std::cout << m_tree.at(i) << ", ";
        std::cout << m_tree.at(m_sz - 1) << "}" << std::endl;
    }

};

} // namespace wndx
