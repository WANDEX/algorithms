#pragma once
/**
 * @brief Binary Search Tree (BST) using std::unique_ptr.
 * Any comparable data is allowed within this tree.
 * Supported operations include adding, removing, height, and containment checks.
 * Multiple tree traversal orders are provided including non-recursive traversal via iterator:
 * 1) Preorder
 * 2) Inorder
 * 3) Postorder
 * 4) Levelorder
 */

#include "BSTuptrNode.hpp"
#include "BSTuptrIterator.hpp"

#include <algorithm>        // std::max
#include <cstddef>          // std::size_t
#include <cstdint>          // std::uint8_t
#include <initializer_list>
#include <iostream>
#include <memory>           // std::unique_ptr
#include <sstream>
#include <stdexcept>        // std::invalid_argument
#include <utility>          // std::move

namespace wndx {
namespace ds {

template<typename T>
class BSTuptr
{
public:
    using enum TreeTravOrder;

    // alias to the node pointer
    using node_ptr = std::unique_ptr<Node<T>>;

    using const_iterator = BSTuptrIterator<T>;

protected:
    enum TreeTravOrder trav_order{ TreeTravOrder::IN_ORDER };

    // tracks the number of nodes in this BST
    std::size_t m_nodeCount {0};

    // this BST is a rooted tree so we maintain a handle on the root node
    node_ptr m_root;

public:
    /**
     * @brief use specific tree traversal order.
     */
    BSTuptr(const TreeTravOrder order)
        : trav_order{ order }
    {
    }

    /**
     * @brief support list initialization, optionally use specified tree traversal order.
     */
    BSTuptr(const std::initializer_list<T> il, const TreeTravOrder order=IN_ORDER)
        : trav_order{ order }
    {
        for (const T n : il) add(n);
    }

    BSTuptr() = default;
    BSTuptr(BSTuptr &&) = default;
    BSTuptr(const BSTuptr &) = default;
    BSTuptr &operator=(BSTuptr &&) = default;
    BSTuptr &operator=(const BSTuptr &) = default;
    virtual ~BSTuptr() = default;

public:
    const_iterator cbegin() const noexcept { return const_iterator(m_root.get(), trav_order); }
    const_iterator cend()   const noexcept { return const_iterator(nullptr); }
    const_iterator begin()  const noexcept { return const_iterator(m_root.get(), trav_order); }
    const_iterator end()    const noexcept { return const_iterator(nullptr); }

    /**
     * @brief the number of nodes in binary tree
     */
    std::size_t size() const
    {
        return m_nodeCount;
    }

    /**
     * @brief check if binary tree is empty
     */
    bool isEmpty() const
    {
        return size() == 0;
    }

public:
    /**
     * @brief add an element to this binary tree.
     *
     * @param data The data/value to add.
     * @return true If the insertion operation was successful.
     */
    bool add(const T& data)
    {
        // if already exists in the binary tree => ignore adding it
        if (contains(data)) return false;
        m_root = add(m_root, data);
        m_nodeCount++;
        return true;
    }
private:
    /**
     * @brief private method to recursively add a value in the binary tree.
     *
     * @param node The node to search from.
     * @param data The data/value to add.
     */
    node_ptr add(node_ptr& node, const T& data)
    {
        // base case: found a leaf node
        if (!node) return std::make_unique<Node<T>>(data);
        // pick a subtree to insert element
        if (data < node->m_data) {
            node->l = add(node->l, data);
        } else {
            node->r = add(node->r, data);
        }
        return std::move(node);
    }

public:
    /**
     * @brief Remove a value from binary tree if it exists, O(n).
     *
     * @param rm_data The data/value to remove.
     */
    bool remove(const T& rm_data)
    {
        // check that node is actually exist
        if (contains(rm_data)) {
            if (true) { // to use a different removal algorithm.
                m_root = std::move(remove(m_root, rm_data));
            } else {
                if (!remove(m_root, m_root, rm_data)) return false;
            }
            m_nodeCount--;
            return true;
        }
        return false;
    }

private:
    /**
     * @brief private recursive method to find & remove an element from the tree, O(n).
     *
     * @param node The node to search from.
     * @param rm_data The data/value to remove.
     * @return reference of the node pointer to update the tree.
     * usage: m_root = std::move(remove(m_root, rm_data));
     */
    node_ptr& remove(node_ptr& node, const T& rm_data)
    {
        if (!node) return node; // nullptr node_ptr
        if (rm_data < node->m_data) {
            // dig into the left subtree
            node->l = std::move(remove(node->l, rm_data));
        } else if (rm_data > node->m_data) {
            // dig into the right subtree
            node->r = std::move(remove(node->r, rm_data));
        } else { // found the node we want to remove

            // This is the case with only a right subtree or no subtree at all.
            // swap the node we want to remove with its right child.
            if (!node->l) return node->r;

            // This is the case with only a left subtree or no subtree at all.
            // swap the node we want to remove with its left child.
            if (!node->r) return node->l;

            /* When removing a node from a binary tree with two links the
             * successor of the node being removed can either be the largest
             * value in the left subtree or the smallest value in the right
             * subtree. In this implementation I have decided to find the
             * smallest value in the right subtree which can be found by
             * traversing as far left as possible in the right subtree. */

            // find the leftmost node in the right subtree
            node_ptr& successor{ findMin(node->r) };
            T sndata{ successor->m_data }; // successor node data

            // swap / set successor node data
            node->m_data = sndata;

            /* go into the right subtree and remove the leftmost node we
             * found and swapped data with. This prevents us from having
             * two nodes in our tree with the same value. */
            node->r = std::move(remove(node->r, sndata));
        }
        return node;
    }

    /**
     * @brief private recursive method to find & remove an element from the tree, O(n).
     *
     * @param parent The parent node of node.
     * @param node The node to search from.
     * @param rm_data The data/value to remove.
     * @return true If the removal operation was successful.
     * usage: remove(m_root, m_root, data);
     * (another version of removing algorithm).
     */
    bool remove(node_ptr& parent, node_ptr& node, const T& rm_data)
    {
        // base cases
        if (!node) return false;
        if (rm_data <  node->m_data) return remove(node, node->l, rm_data);
        if (rm_data >  node->m_data) return remove(node, node->r, rm_data);
        if (rm_data != node->m_data) return false;
        // found the node we want to remove (rm_data == node->m_data)
        if (node->l && node->r) {
            node_ptr& successor{ findMin(node->r) };
            T sndata{ successor->m_data }; // successor node data
            remove(m_root, m_root, sndata);
            node->m_data = sndata;

        } else if (node->l || node->r) {
            node_ptr& non_null = (node->l ? node->l : node->r);
            if (m_root == node)
                m_root    = std::move(non_null);
            else if (rm_data < parent->m_data)
                parent->l = std::move(non_null);
            else
                parent->r = std::move(non_null);

        } else {
            if (node == m_root)
                m_root.reset(nullptr);
            else if (rm_data < parent->m_data)
                parent->l.reset(nullptr);
            else
                parent->r.reset(nullptr);
        }
        return true;
    }

    /**
     * @brief Helper method to find the leftmost node (which has the smallest value).
     * usage: node_ptr& successor{ findMin(node->r) };
     */
    node_ptr& findMin(node_ptr& node)
    {
        while (node->l) return findMin(node->l);
        return node;
    }

    /**
     * @brief Helper method to find the rightmost node (which has the largest value).
     * usage: node_ptr& successor{ findMax(node->l) };
     */
    node_ptr& findMax(node_ptr& node)
    {
        while (node->r) return findMax(node->r);
        return node;
    }

public:
    /**
     * @brief returns true if the element exists in the tree.
     */
    bool contains(const T& data)
    {
        return contains(m_root, data);
    }
private:
    /**
     * @brief private recursive method to find an element in the tree.
     */
    bool contains(node_ptr& node, const T& data)
    {
        // base case: reached bottom, value not found
        if (!node) return false;
        // dig into the left subtree
        if (data < node->m_data)
            return contains(node->l, data);
        // dig into the right subtree
        if (data > node->m_data)
            return contains(node->r, data);
        // found the value we were looking for
        return true;
    }

public:
    /**
     * @brief compute the height of the tree, O(n).
     */
    std::size_t height()
    {
        return height(m_root);
    }
private:
    /**
     * @brief recursive helper method to compute the height of the tree.
     */
    std::size_t height(const node_ptr& node)
    {
        if (!node) return 0;
        return std::max(height(node->l), height(node->r)) + 1;
    }

protected:
    /**
     * @brief helper method to pretty print tree.
     */
    void cout_tree_info(std::ostringstream& oss, const std::string& prefix)
    {
        std::string str { oss.str() };
        // erase extra ", " string (after last node data)
        if (str.length() > 1) str.erase(str.end() - 2, str.end());
        std::cout << prefix << "h:(" << height() << ")\t[" << str << "]\n";
    }

    /**
     * @brief recursive method to print tree in preorder.
     */
    void print_preorder(const node_ptr& node, std::ostream& oss = std::cout) const
    {
        if (!node) return;
        oss << node->m_data << ", ";
        print_preorder(node->l, oss);
        print_preorder(node->r, oss);
    }

    /**
     * @brief recursive method to print tree in inorder.
     */
    void print_inorder(const node_ptr& node, std::ostream& oss = std::cout) const
    {
        if (!node) return;
        print_inorder(node->l, oss);
        oss << node->m_data << ", ";
        print_inorder(node->r, oss);
    }

    /**
     * @brief recursive method to print tree in postorder.
     */
    void print_postorder(const node_ptr& node, std::ostream& oss = std::cout) const
    {
        if (!node) return;
        print_postorder(node->l, oss);
        print_postorder(node->r, oss);
        oss << node->m_data << ", ";
    }

    /**
     * @brief recursive method to print tree in levelorder.
     */
    void print_levelorder(const node_ptr& node, std::ostream& oss = std::cout)
    {
        for (std::size_t i = 0; i <= height(); i++) {
            lorder_cur_lvl(node, i, oss);
        }
    }

private:
    void lorder_cur_lvl(const node_ptr& node, const std::size_t& lvl, std::ostream& oss = std::cout) const
    {
        if (!node) return;
        if (lvl == 1) {
            oss << node->m_data << ", ";
        } else if (lvl > 1) {
            lorder_cur_lvl(node->l, lvl - 1, oss);
            lorder_cur_lvl(node->r, lvl - 1, oss);
        }
    }

public:
    /**
     * @brief pass enum to print tree in specific order i.e. tree.IN_ORDER
     */
    void print(const TreeTravOrder order, const std::string& prefix="")
    {
        std::ostringstream oss;
        switch (order) {
            case PRE_ORDER:
                print_preorder(m_root, oss);
                break;
            case IN_ORDER:
                print_inorder(m_root, oss);
                break;
            case POST_ORDER:
                print_postorder(m_root, oss);
                break;
            case LEVEL_ORDER:
                print_levelorder(m_root, oss);
                break;
            default:
                throw std::invalid_argument("No such Tree Traversal Order.");
        }
        cout_tree_info(oss, prefix);
    }

};

} // namespace ds
} // namespace wndx
