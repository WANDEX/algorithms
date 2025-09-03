#pragma once
/**
 * Doubly Linked List implementation written using the raw pointers.
 */

#include <concepts>             // std::convertible_to
#include <cstddef>              // std::size_t, std::ptrdiff_t
#include <initializer_list>
#include <iterator>
#include <sstream>              // std::ostream
#include <stdexcept>            // std::runtime_error, std::out_of_range
#include <string>
#include <vector>
// #include <new>                  // std::nothrow

namespace wndx::algo {
namespace ds {

template<typename T>
class DLLraw {
protected:
#if 1 // trivially constructible aggregate type
    struct Node
    {
        T     m_data{};
        Node* m_prev{ nullptr };
        Node* m_next{ nullptr };
    };
#else
    class Node
    {
    public:
        Node() = delete;
        Node(Node &&) = default;
        Node(const Node &) = default;
        Node &operator=(Node &&) = default;
        Node &operator=(const Node &) = default;
        virtual ~Node() = default;
        explicit Node(T const& data, Node* const prev, Node* const next)
            : m_data{data}, m_prev{prev}, m_next{next}
        {}
        T     m_data{};
        Node* m_prev{ nullptr };
        Node* m_next{ nullptr };
    };
#endif // trivially constructible aggregate type

    // aliases
    using data_t   = T;
    using node_t   = Node;
    using node_ptr = node_t *;

public:
    template <typename IT>
    class DLLraw_iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = std::remove_cv_t<IT>;
        using reference         = node_t&;
        using pointer           = node_t*;

        using self_t            = DLLraw_iterator;
    protected:
        pointer m_ptr{ nullptr }; // current node
    public:

        ~DLLraw_iterator() = default;
        DLLraw_iterator()  = delete;

        constexpr explicit DLLraw_iterator(pointer ptr) noexcept : m_ptr(ptr) {}

        // allow convert of iterator to bool
        constexpr operator bool() const { return m_ptr != nullptr; }

        constexpr value_type operator* () const { return m_ptr->m_data; }
        constexpr pointer    operator->() const { return m_ptr; }

        // pre/post-increment (prefix/postfix)
        constexpr self_t&    operator++()    { m_ptr = m_ptr->m_next; return *this; }
        constexpr self_t     operator++(int) { self_t res{*this}; ++(*this); return res; }

        // pre/post-decrement (prefix/postfix)
        constexpr self_t&    operator--()    { m_ptr = m_ptr->m_prev; return *this; }
        constexpr self_t     operator--(int) { self_t res{*this}; --(*this); return res; }

        // eq/inequality.
        constexpr bool operator== (const self_t&) const = default;

        // comparison operators: { <, <=, >, >= }.
        constexpr auto operator<=>(const self_t&) const = default;
    };

public:
    template<typename IT>
    class DLLraw_reverse_iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = std::remove_cv_t<IT>;
        using reference         = node_t&;
        using pointer           = node_t*;

        using self_t            = DLLraw_reverse_iterator;
    protected:
        pointer m_ptr{ nullptr }; // current node
    public:

        ~DLLraw_reverse_iterator() = default;
        DLLraw_reverse_iterator()  = delete;

        constexpr explicit DLLraw_reverse_iterator(pointer ptr) noexcept : m_ptr(ptr) {}

        // allow convert of iterator to bool
        constexpr operator bool() const { return m_ptr != nullptr; }

        constexpr value_type operator* () const { return m_ptr->m_data; }
        constexpr pointer    operator->() const { return m_ptr; }

        // pre/post-increment (prefix/postfix)
        constexpr self_t&    operator++()    { m_ptr = m_ptr->m_prev; return *this; }
        constexpr self_t     operator++(int) { self_t res{*this}; ++(*this); return res; }

        // pre/post-decrement (prefix/postfix)
        constexpr self_t&    operator--()    { m_ptr = m_ptr->m_next; return *this; }
        constexpr self_t     operator--(int) { self_t res{*this}; --(*this); return res; }

        // eq/inequality.
        constexpr bool operator== (const self_t&) const = default;

        // comparison operators: { <, <=, >, >= }.
        constexpr auto operator<=>(const self_t&) const = default;
    };

public:
    using iterator               = DLLraw_iterator<T>;
    using reverse_iterator       = DLLraw_reverse_iterator<T>;

    // XXX: broken
    // using const_iterator         = DLLraw_iterator<std::add_const_t<T>>;
    // using const_reverse_iterator = DLLraw_reverse_iterator<std::add_const_t<T>>;

    // TODO: figure out how to make this work! (to avoid duplicating code)
    // 'https://en.cppreference.com/w/cpp/iterator/reverse_iterator'
    // It should be simple, but for some reason it is not! (spent too much time on this...)
    // XXX:
    // using reverse_iterator       = std::reverse_iterator<iterator>;
    // using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    constexpr iterator               begin()   const noexcept { return iterator(m_head); }
    constexpr iterator               end()     const noexcept { return iterator(m_tail->m_next); }

    constexpr reverse_iterator       rbegin()  const noexcept { return reverse_iterator(m_tail); }
    constexpr reverse_iterator       rend()    const noexcept { return reverse_iterator(m_head->m_prev); }

    // constexpr const_iterator         cbegin()  const noexcept { return const_iterator(m_head); }
    // constexpr const_iterator         cend()    const noexcept { return const_iterator(m_tail->m_next); }

    // constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(m_tail); }
    // constexpr const_reverse_iterator crend()   const noexcept { return const_reverse_iterator(m_head->m_prev); }

private:
    std::size_t m_size{ 0 };
    node_ptr    m_head{ nullptr };
    node_ptr    m_tail{ nullptr };

public:
    DLLraw() = default;
    DLLraw(DLLraw &&) = default;
    DLLraw(const DLLraw &) = default;
    DLLraw &operator=(DLLraw &&) = default;
    DLLraw &operator=(const DLLraw &) = default;

    virtual ~DLLraw() noexcept
    {
        clear();
    }

    /**
     * @brief Construct list with an initial set of values, O(n).
     */
    explicit DLLraw(const std::initializer_list<T> &il) noexcept
    {
        for (const T &e : il) {
            add(e);
        }
    }

    /**
     * @brief Construct list with an initial set of values, O(n).
     */
    explicit DLLraw(const std::vector<T> &v) noexcept
    {
        for (const T &e : v) {
            add(e);
        }
    }

    /**
     * Empty this linked list, O(n)
     */
    void clear() noexcept
    {
        node_ptr trav{ m_head };
        while (trav != nullptr) {
            node_ptr next = trav->m_next;
            // XXX: DOUBTS: 'branch 1 of delete operator' in the current context is irrelevant, right?
            // How to resolve/cover this without ignoring the both branches?
            delete trav; // LCOV_EXCL_BR_LINE
            trav = next;
        }
        m_head = m_tail = trav = nullptr;
        m_size = 0;
    }

    /**
     * return the size of the linked list
     */
    std::size_t size() const noexcept
    {
        return m_size;
    }

    /**
     * Is linked list empty?
     */
    bool empty() const noexcept
    {
        return size() == 0;
    }

    /**
     * Add an element to the tail of the linked list, O(1)
     */
    void add(const T &elem) noexcept
    {
        addLast(elem);
    }

    /**
     * Add an element to the beginning of the linked list, O(1)
     */
    void addFirst(const T &elem) noexcept
    {
        if (empty()) {
            m_head = m_tail = new node_t(elem, nullptr, nullptr);
        } else {
            m_head->m_prev  = new node_t(elem, nullptr,  m_head);
            m_head = m_head->m_prev;
        }
        m_size++;
    }

    /**
     * Add an element to the tail of the linked list, O(1)
     */
    void addLast(const T &elem) noexcept
    {
        if (empty()) {
            m_tail = m_head = new node_t(elem, nullptr, nullptr);
        } else {
            m_tail->m_next  = new node_t(elem,  m_tail, nullptr);
            m_tail = m_tail->m_next;
        }
        m_size++;
    }

    /**
     * Add an element at a specified index, O(n)
     */
    void addAt(const std::size_t index, const T &elem)
    {
        if (index > m_size) {
            throw std::out_of_range("Index > size.");
        }
        if (index == 0) {
            addFirst(elem);
            return;
        }
        if (index == m_size) {
            addLast(elem);
            return;
        }
        node_ptr temp{ m_head };
        for (std::size_t i = 0; i < index - 1; i++) {
            temp = temp->m_next;
        }
        node_ptr newNode{ new node_t(elem, temp, temp->m_next) };
        temp->m_next->m_prev = newNode;
        temp->m_next = newNode;
        m_size++;
    }

    /**
     * Check the value of the first node if it exists, O(1)
     */
    T peekFirst() const
    {
        if (empty()) throw std::runtime_error("Empty list.");
        return m_head->m_data;
    }

    /**
     * Check the value of the last node if it exists, O(1)
     */
    T peekLast() const
    {
        if (empty()) throw std::runtime_error("Empty list.");
        return m_tail->m_data;
    }

    /**
     * Remove the first value at the m_head of the linked list, O(1)
     */
    T removeFirst()
    {
        if (empty()) throw std::runtime_error("Empty list.");
        node_t const& h_old{ *m_head };
        T const data{ h_old.m_data }; // extract the data at the head
        m_head = h_old.m_next; // move the head pointer forwards one node
        delete  &h_old;
        --m_size;
        if (empty()) m_tail = nullptr;
        else m_head->m_prev = nullptr; // memory cleanup
        return data;
    }

    /**
     * Remove the last value at the tail of the linked list, O(1)
     */
    T removeLast()
    {
        if (empty()) throw std::runtime_error("Empty list.");
        node_t const& t_old{ *m_tail };
        T const data{ t_old.m_data }; // extract the data at the tail
        m_tail = t_old.m_prev; // move the tail pointer backwards one node
        delete  &t_old;
        --m_size;
        if (empty()) m_head = nullptr;
        else m_tail->m_next = nullptr; // memory cleanup
        return data;
    }

    /**
     * Remove an arbitrary node from the linked list, O(1)
     */
    T remove(node_ptr node)
    {
        if (node->m_prev == nullptr) return removeFirst();
        if (node->m_next == nullptr) return removeLast();
        // Make the pointers of adjacent nodes skip over 'node'
        node->m_next->m_prev = node->m_prev;
        node->m_prev->m_next = node->m_next;
        T const data = node->m_data; // tmp store the data
        delete node;
        node = nullptr; // memory cleanup
        --m_size;
        return data;
    }

    /**
     * remove a node at a particular index, O(n)
     */
    T removeAt(const std::size_t index)
    {
        if (index >= m_size) {
            throw std::out_of_range("Index >= size.");
        }
        node_ptr trav{ nullptr };
        if (index < m_size / 2) {
            // search from the front
            trav = m_head;
            std::size_t i{ 0 };
            for (; i < index; i++) {
                trav = trav->m_next;
            }
        } else {
            // search from the back
            trav = m_tail;
            std::size_t i{ m_size - 1 };
            for (; i > index; i--) {
                trav = trav->m_prev;
            }
        }
        return remove(trav);
    }

    /**
     * Remove a particular value in the linked list, O(n)
     */
    bool remove(const T &obj)
    {
        node_ptr trav{ m_head };
        for (; trav != nullptr; trav = trav->m_next) {
            if (obj == trav->m_data) {
                remove(trav);
                return true;
            }
        }
        return false;
    }

    /**
     * Find the index of a particular value in the linked list, O(n).
     * if not found => returns size() + 1 (one past the last).
     */
    std::size_t indexOf(const T &obj) const noexcept
    {
        std::size_t index {0};
        node_ptr trav{ m_head };
        for (; trav != nullptr; trav = trav->m_next, index++) {
            if (obj == trav->m_data) {
                return index;
            }
        }
        return size() + 1;
    }

    /**
     * Check that value is contained within the linked list, O(n).
     */
    bool contains(const T &obj) const noexcept
    {
        return indexOf(obj) != size() + 1;
    }

    std::string toString() const
    {
        std::ostringstream oss;
        oss << "{ ";
        node_ptr trav{ m_head };
        while (trav != nullptr) {
            oss << trav->m_data;
            trav = trav->m_next;
            if (trav) oss << ", ";
        }
        oss << " }";
        return oss.str();
    }

    friend std::ostream& operator<<(std::ostream &strm, const DLLraw<T> &a) {
        return strm << a.toString();
    }
};

} // namespace ds
} // namespace wndx::algo