#pragma once

#include <memory>           // std::unique_ptr

namespace wndx {

template <typename T>
class Node
{
public:
    using node_ptr = std::unique_ptr<Node<T>>;
    T m_data;
    node_ptr l, r; // left/right node

    Node(const T& data, node_ptr& left, node_ptr& right)
        : m_data{ data }, l{ left }, r{ right }
    {}

    Node(const T& data)
        : m_data{ data }
    {}

    Node() = default;
    Node(Node &&) = default;
    Node(const Node &) = default;
    Node &operator=(Node &&) = default;
    Node &operator=(const Node &) = default;
    virtual ~Node() = default;
};

} // namespace wndx
