#pragma once
/**
 * const iterator for Binary Search Tree (BST) specific for std::unique_ptr nodes.
 * Multiple non-recursive tree traversal orders are provided including:
 * 1) Preorder
 * 2) Inorder
 * 3) Postorder
 * 4) Levelorder
 */

#include "BSTuptrNode.hpp"

#include <cstddef>          // std::ptrdiff_t
#include <cstdint>          // std::uint8_t
#include <iterator>

#include <initializer_list>
#include <deque>
#include <queue>
#include <stack>

enum class TreeTravOrder : std::uint8_t
{
    PRE_ORDER,
    IN_ORDER,
    POST_ORDER,
    LEVEL_ORDER,
};

template <typename T>
class BSTuptrIterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = const std::ptrdiff_t;
    using value_type        = const T;
    using reference         = const T&;
    using pointer           = const T* const;

    using node_ptr          = const Node<T>*;
    using const_iterator    = BSTuptrIterator<T>;

    using enum TreeTravOrder;
    enum TreeTravOrder trav_order{ TreeTravOrder::IN_ORDER };

protected:
    std::deque<node_ptr> m_deque;
    node_ptr m_root;
    int current {0};

public:
    explicit BSTuptrIterator(const node_ptr ptr, const TreeTravOrder order=IN_ORDER)
        : m_root(ptr), trav_order{ order }
    {
        traverse();
        m_deque.push_back(nullptr); // end() of the tree
    }

    BSTuptrIterator() = default;
    virtual ~BSTuptrIterator() = default;

    // dereference
    reference       operator*() const { return m_deque[current]->m_data; }

    // pre/post-increment
    const_iterator& operator++()      { ++current; return *this; };
    const_iterator  operator++(int)   { const_iterator tmp = *this; ++current; return tmp; };

    // pre/post-decrement
    const_iterator& operator--()      { --current; return *this; };
    const_iterator  operator--(int)   { const_iterator tmp = *this; --current; return tmp; };

    // eq/inequality
    bool operator==(const_iterator const& rhs) const
    {
        return m_deque[current] == rhs.m_deque[rhs.current];
    }

    auto operator<=>(const_iterator const& rhs) const = default;

protected:
    // O(1)
    void ins_back(const node_ptr p)
    {
        m_deque.push_back(p);
    }

    // O(1)
    void ins_front(const node_ptr p)
    {
        m_deque.push_front(p);
    }

public:
    void preorder()
    {
        std::stack<node_ptr> s;
        node_ptr p{ m_root };

        while (p || !s.empty()) {
            while (p) {
                ins_back(p);
                s.push(p);
                p = p->l.get();
            }
            if (!s.empty()) {
                p = s.top();
                s.pop();
                p = p->r.get();
            }
        }
    }

    void inorder()
    {
        std::stack<node_ptr> s;
        node_ptr p{ m_root };

        while (p || !s.empty()) {
            while (p) {
                s.push(p);
                p = p->l.get();
            }
            if (!s.empty()) {
                p = s.top();
                ins_back(p);
                s.pop();
                p = p->r.get();
            }
        }
    }

    void postorder()
    {
        if (!m_root) return;
        std::stack<node_ptr> s;
        node_ptr p{ m_root };
        s.push(p);

        while (!s.empty()) {
            p = s.top();
            s.pop();
            ins_front(p);
            if (p->l) s.push(p->l.get());
            if (p->r) s.push(p->r.get());
        }
    }

    void levelorder()
    {
        if (!m_root) return;
        std::queue<node_ptr> q;
        node_ptr p{ m_root };
        q.push(p);

        while (!q.empty()) {
            p = q.front();
            q.pop();
            ins_back(p);
            if (p->l) q.push(p->l.get());
            if (p->r) q.push(p->r.get());
        }
    }

    void traverse()
    {
        switch (trav_order) {
            case PRE_ORDER:
                preorder();
                break;
            case IN_ORDER:
                inorder();
                break;
            case POST_ORDER:
                postorder();
                break;
            case LEVEL_ORDER:
                levelorder();
                break;
            default:
                throw std::invalid_argument("No such Tree Traversal Order.");
        }
    }

};
