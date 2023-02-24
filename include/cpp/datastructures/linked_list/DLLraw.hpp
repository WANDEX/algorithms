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

namespace wndx {
namespace ds {

template<typename T>
class DLLraw {
protected: /* nested/internal Node class to represent data
'https://en.cppreference.com/w/cpp/language/template_specialization' */
    template <typename NT> // (required ^)
    class Node
    {
    private:
        NT m_data {};
        Node<NT> *m_prev{ nullptr }, *m_next{ nullptr };
        // DLLraw need an access to the Node information
        friend class DLLraw<NT>;

    public:
        Node() = delete;
        Node(Node &&) = delete;
        Node(const Node &) = delete;
        Node &operator=(Node &&) = delete;
        Node &operator=(const Node &) = delete;

        ~Node() = default;

        Node(const NT &data, Node<NT> *prev, Node<NT> *next) noexcept
            : m_data{ data }, m_prev{ prev }, m_next{ next }
        {}

        std::string toString() const
        {
            std::ostringstream oss;
            oss << "Node(" << m_data << ")";
            return oss.str();
        }

        // dereference XXX is this right?
        // NT operator*() const { return m_data; }

        friend std::ostream& operator<<(std::ostream &strm, const Node<NT> &a)
        {
            return strm << a.toString();
        }
    };

public:
    template <typename IT>
    class DLLraw_iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = std::remove_cv_t<IT>;
        using reference         = Node<IT>&;
        using pointer           = Node<IT>*;

        using self_t            = DLLraw_iterator;
    protected:
        pointer m_ptr{ nullptr }; // current node
    public:

        ~DLLraw_iterator() = default;
        DLLraw_iterator()  = delete;

        constexpr explicit DLLraw_iterator(pointer ptr) noexcept : m_ptr(ptr) {}

        // allow explicit convert to bool like: static_cast<bool>(iter)
        // constexpr explicit operator bool() const { return m_ptr != nullptr; }

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
        using reference         = Node<IT>&;
        using pointer           = Node<IT>*;

        using self_t            = DLLraw_reverse_iterator;
    protected:
        pointer m_ptr{ nullptr }; // current node
    public:

        ~DLLraw_reverse_iterator() = default;
        DLLraw_reverse_iterator()  = delete;

        constexpr explicit DLLraw_reverse_iterator(pointer ptr) noexcept : m_ptr(ptr) {}

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
    std::size_t m_size {0};
    Node<T> *m_head{ nullptr };
    Node<T> *m_tail{ nullptr };

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
        Node<T> *trav = m_head;
        while (trav != nullptr) {
            Node<T> *next = trav->m_next;
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
    bool isEmpty() const noexcept
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
        if (isEmpty()) {
            m_head = m_tail = new Node<T>(elem, nullptr, nullptr);
        } else {
            m_head->m_prev  = new Node<T>(elem, nullptr, m_head);
            m_head = m_head->m_prev;
        }
        m_size++;
    }

    /**
     * Add an element to the tail of the linked list, O(1)
     */
    void addLast(const T &elem) noexcept
    {
        if (isEmpty()) {
            m_head = m_tail = new Node<T>(elem, nullptr, nullptr);
        } else {
            m_tail->m_next  = new Node<T>(elem, m_tail, nullptr);
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
        Node<T> *temp = m_head;
        for (std::size_t i = 0; i < index - 1; i++) {
            temp = temp->m_next;
        }
        Node<T> *newNode = new Node<T>(elem, temp, temp->m_next);
        temp->m_next->m_prev = newNode;
        temp->m_next = newNode;
        m_size++;
    }

    /**
     * Check the value of the first node if it exists, O(1)
     */
    T peekFirst() const
    {
        if (isEmpty()) throw std::runtime_error("Empty list.");
        return m_head->m_data;
    }

    /**
     * Check the value of the last node if it exists, O(1)
     */
    T peekLast() const
    {
        if (isEmpty()) throw std::runtime_error("Empty list.");
        return m_tail->m_data;
    }

    /**
     * Remove the first value at the m_head of the linked list, O(1)
     */
    T removeFirst()
    {
        if (isEmpty()) throw std::runtime_error("Empty list.");
        T data = m_head->m_data; // Extract the data at the m_head
        m_head = m_head->m_next; // move the m_head pointer forwards one node
        --m_size;
        if (isEmpty()) m_tail = nullptr;
        else m_head->m_prev   = nullptr; // memory cleanup
        return data; // Return the data of the node we just removed
    }

    /**
     * Remove the last value at the tail of the linked list, O(1)
     */
    T removeLast()
    {
        if (isEmpty()) throw std::runtime_error("Empty list.");
        T data = m_tail->m_data; // Extract the data at the tail
        m_tail = m_tail->m_prev; // move the tail pointer backwards one node
        --m_size;
        if (isEmpty()) m_head = nullptr;
        else m_tail->m_next   = nullptr; // memory cleanup
        return data;
    }

    /**
     * Remove an arbitrary node from the linked list, O(1)
     */
    T remove(Node<T> *node)
    {
        if (node->m_prev == nullptr) return removeFirst();
        if (node->m_next == nullptr) return removeLast();
        // Make the pointers of adjacent nodes skip over 'node'
        node->m_next->m_prev = node->m_prev;
        node->m_prev->m_next = node->m_next;
        T data = node->m_data;  // tmp store the data
        // memory cleanup
        node = node->m_prev = node->m_next = nullptr;
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
        Node<T> *trav{ nullptr };
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
        Node<T> *trav = m_head;
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
        Node<T> *trav = m_head;
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
        Node<T> *trav = m_head;
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
} // namespace wndx
