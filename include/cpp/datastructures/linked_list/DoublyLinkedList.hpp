#pragma once

#include <cstddef>      // size_t
#include <sstream>      // ostream
#include <stdexcept>    // runtime_error
#include <string>

namespace wndx {
namespace ds {

template<typename T>
class DoublyLinkedList
{
private: /* nested/internal Node class to represent data
'https://en.cppreference.com/w/cpp/language/template_specialization' */
    template <typename U> // Unused (required ^)
    class Node
    {
    private:
        T m_data {};
        Node<T> *m_prev{ nullptr }, *m_next{ nullptr };
        // DoublyLinkedList need an access to the Node information
        friend class DoublyLinkedList<T>;

    public:
        Node(const T &data, Node<T> *prev, Node<T> *next)
            : m_data{ data }, m_prev{ prev }, m_next{ next }
        {}
        ~Node() = default;

        std::string toString() const
        {
            std::ostringstream oss;
            oss << "Node(" << m_data << ")";
            return oss.str();
        }

        friend std::ostream& operator<<(std::ostream &strm, const Node<T> &a)
        {
            return strm << a.toString();
        }
    };
private:
    std::size_t m_size {0};
    Node<T> *head { nullptr };
    Node<T> *tail { nullptr };

public:
    DoublyLinkedList()
    {}

    virtual ~DoublyLinkedList()
    {
        clear();
    }

    /**
     * Empty this linked list, O(n)
     */
    void clear()
    {
        Node<T> *trav = head;
        while (trav != nullptr) {
            Node<T> *next = trav->m_next;
            delete trav;
            trav = nullptr;
            trav = next;
        }
        head = tail = trav = nullptr;
        m_size = 0;
    }

    /**
     * return the size of the linked list
     */
    std::size_t size() const
    {
        return m_size;
    }

    /**
     * Is linked list empty?
     */
    bool isEmpty() const
    {
        return size() == 0;
    }

    /**
     * Add an element to the tail of the linked list, O(1)
     */
    void add(const T &elem)
    {
        addLast(elem);
    }

    /**
     * Add an element to the beginning of the linked list, O(1)
     */
    void addFirst(const T &elem)
    {
        if (isEmpty()) {
            head = tail = new Node<T>(elem, nullptr, nullptr);
        } else {
            head->m_prev = new Node<T>(elem, nullptr, head);
            head = head->m_prev;
        }
        m_size++;
    }

    /**
     * Add an element to the tail of the linked list, O(1)
     */
    void addLast(const T &elem)
    {
        if (isEmpty()) {
            head = tail = new Node<T>(elem, nullptr, nullptr);
        } else {
            tail->m_next = new Node<T>(elem, tail, nullptr);
            tail = tail->m_next;
        }
        m_size++;
    }

    /**
     * Add an element at a specified index, O(n)
     */
    void addAt(const std::size_t index, const T &elem)
    {
        if (index > m_size) {
            throw ("Illegal Index");
        }
        if (index == 0) {
            addFirst(elem);
            return;
        }
        if (index == m_size) {
            addLast(elem);
            return;
        }
        Node<T> *temp = head;
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
        if (isEmpty()) throw std::runtime_error("Empty list");
        return head->m_data;
    }

    /**
     * Check the value of the last node if it exists, O(1)
     */
    T peekLast() const
    {
        if (isEmpty()) throw std::runtime_error("Empty list");
        return tail->m_data;
    }

    /**
     * Remove the first value at the head of the linked list, O(1)
     */
    T removeFirst()
    {
        if (isEmpty()) throw std::runtime_error("Empty list");
        T data = head->m_data; // Extract the data at the head
        head = head->m_next;   // move the head pointer forwards one node
        --m_size;
        if (isEmpty()) tail = nullptr;
        else head->m_prev = nullptr; // memory cleanup
        return data; // Return the data of the node we just removed
    }

    /**
     * Remove the last value at the tail of the linked list, O(1)
     */
    T removeLast()
    {
        if (isEmpty()) throw std::runtime_error("Empty list");
        T data = tail->m_data; // Extract the data at the tail
        tail = tail->m_prev;   // move the tail pointer backwards one node
        --m_size;
        if (isEmpty()) head = nullptr;
        else tail->m_next = nullptr; // memory cleanup
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
        if (index >= m_size)
            throw std::invalid_argument("Invalid Index");
        std::size_t i;
        Node<T> *trav;
        if (index < m_size / 2) { // search from the front
            trav = head; i = 0;
            for (; i != index; i++) trav = trav->m_next;
        } else { // search from the back
            trav = tail; i = m_size - 1;
            for (; i != index; i--) trav = trav->m_prev;
        }
        return remove(trav);
    }

    /**
     * Remove a particular value in the linked list, O(n)
     */
    bool remove(const T &obj)
    {
        Node<T> *trav = head;
        // TODO: support searching for nullptr obj
        for (; trav != nullptr; trav = trav->m_next) {
            if (obj == trav->m_data) {
                remove(trav);
                return true;
            }
        }
        return false;
    }

    /**
     * Find the index of a particular value in the linked list, O(n)
     */
    std::size_t indexOf(const T &obj) const
    {
        std::size_t index {0};
        Node<T> *trav = head;
        // TODO: support searching for nullptr obj
        for (; trav != nullptr; trav = trav->m_next, index++) {
            if (obj == trav->m_data) {
                return index;
            }
        }
        return -1;
    }

    /**
     * Check that value is contained within the linked list, O(n)
     */
    bool contains(const T &obj) const
    {
        return indexOf(obj) != -1;
    }

    std::string toString() const
    {
        std::ostringstream oss;
        oss << "{ ";
        Node<T> *trav = head;
        while (trav != nullptr) {
            oss << trav->m_data;
            trav = trav->m_next;
            if (trav) oss << ", ";
        }
        oss << " }";
        return oss.str();
    }

    friend std::ostream& operator<<(std::ostream &strm, const DoublyLinkedList<T> &a)
    {
        return strm << a.toString();
    }
};

} // namespace ds
} // namespace wndx
