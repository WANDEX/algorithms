#pragma once
/**
 * UnionFind/Disjoint Set data structure implementation.
 */

#include <cstddef>              // std::size_t
#include <vector>

namespace wndx::algo {
namespace ds {

class UnionFind
{
protected:
    // the number of elements in this union find
    std::size_t m_size;

    // tracks the number of components in the union find
    std::size_t m_ncomp;

    // used to track the size of each of the component
    std::vector<std::size_t> m_sz;

    // id[i] points to the parent of i, if id[i] = i then i is a root node
    std::vector<std::size_t> m_id;

public:
    UnionFind() = delete;

    UnionFind(const std::size_t size)
        : m_size{ size }, m_ncomp{ size }
    {
        m_id.resize(size);
        m_sz.resize(size);
        for (std::size_t i = 0; i < size; i++) {
            m_id.at(i) = i; // link to itself (self root)
            m_sz.at(i) = 1; // each component is originally of size one
        }
    }

    UnionFind(UnionFind &&) = default;
    UnionFind(const UnionFind &) = default;
    UnionFind &operator=(UnionFind &&) = default;
    UnionFind &operator=(const UnionFind &) = default;
    virtual ~UnionFind() = default;

    /**
     * find which component/set 'p' belongs to, takes amortized constant time.
     */
    std::size_t find(std::size_t p)
    {
        // find the root of the component/set
        std::size_t root { p };
        while (root != m_id[root]) root = m_id[root];
        /* Compress the path leading back to the root.
         * Doing this operation is called "path compression"
         * and is what gives us amortized time complexity. */
        while (p != root) {
            std::size_t next { m_id[p] };
            m_id[p] = root;
            p = next;
        }
        return root;
    }

    /**
     * return whether or not the elements 'p' and
     * 'q' are in the same components/set.
     */
    bool connected(const std::size_t p, const std::size_t q)
    {
        return find(p) == find(q);
    }

    /**
     * return the size of the components/set 'p' belongs to
     */
    std::size_t componentSize(const std::size_t p)
    {
        return m_sz[find(p)];
    }

    /**
     * return the number of elements in this UnionFind/Disjoint set
     */
    std::size_t size() const noexcept
    {
        return m_size;
    }

    /**
     * return the number of remaining components/sets
     */
    std::size_t components() const noexcept
    {
        return m_ncomp;
    }

    /**
     * unify the components/sets containing elements 'p' and 'q'
     */
    void unify(const std::size_t p, const std::size_t q)
    {
        // These elements are already in the same group!
        if (connected(p, q)) return;

        std::size_t root1 { find(p) };
        std::size_t root2 { find(q) };

        // Merge smaller component/set into the larger one.
        if (m_sz[root1] <  m_sz[root2]) {
            m_sz[root2] += m_sz[root1];
            m_id[root1]  = root2;
            m_sz[root1]  = 0;
        } else {
            m_sz[root1] += m_sz[root2];
            m_id[root2]  = root1;
            m_sz[root2]  = 0;
        }

        // since the roots found are different we know that the
        // number of components/sets has decreased by one
        m_ncomp--;
    }
};

} // namespace ds
} // namespace wndx::algo
