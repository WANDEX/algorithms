#pragma once
/**
 * A min priority queue implementation using a binary heap.
 * This implementation tracks each element inside the binary heap
 * with a hash table for quick removals.
 */

#include <cstddef>              // std::size_t
#include <initializer_list>
#include <iostream>
#include <set>
#include <sstream>              // std::ostream
#include <stdexcept>            // invalid_argument, runtime_error, out_of_range
#include <unordered_map>
#include <vector>

namespace wndx {
namespace ds {

template<typename T>
class BinaryHeapQ
{
protected:
    // A dynamic vector to track the elements inside the heap
    std::vector<T> heap;

    /**
     * This map keeps track of the possible indices a particular
     * node value is found in the heap. Having this mapping lets
     * us have O(log(n)) removals and O(1) element containment check
     * at the cost of some additional space and minor overhead
     */
    std::unordered_map<T, std::set<std::size_t>> umap;

public:
    // Construct an initially empty priority queue
    BinaryHeapQ()
    {}

    // Construct a priority queue with an initial capacity
    BinaryHeapQ(const std::size_t sz)
    {
        heap.resize(sz);
    }

    /**
     * Construct a priority queue using heapify in O(n) time, a great explanation can be found at:
     * https://www.cs.umd.edu/~meesh/351/mount/lectures/lect14-heapsort-analysis-part.pdf
     */
    BinaryHeapQ(const std::initializer_list<T> &il)
    {
        if (il.size() == 0)
            throw std::invalid_argument("Empty initializer list was provided.");
        heap.reserve(il.size());
        // place all element in the heap
        std::size_t i {0};
        for (const T &elem : il) {
            mapAdd(elem, i++);
            heap.push_back(elem);
        }
        // Heapify process, O(n)
        std::size_t j { (il.size() / 2) + 1 };
        do { sink(--j); } while (j > 0);
    }

    BinaryHeapQ(const std::vector<T> &v)
    {
        if (v.empty())
            throw std::invalid_argument("Empty std::vector was provided.");
        heap.reserve(v.size());
        // place all element in the heap
        std::size_t i {0};
        for (const T &elem : v) {
            mapAdd(elem, i++);
            heap.push_back(elem);
        }
        // Heapify process, O(n)
        std::size_t j { (v.size() / 2) + 1 };
        do { sink(--j); } while (j > 0);
    }

    virtual ~BinaryHeapQ() = default;

#if 0
    void print_set(const std::set<std::size_t> &s)
    {
        std::set<std::size_t>::iterator it { s.begin() };
        std::cout << '(';
        for (; it != std::prev(s.end()); it++)
            std::cout << *it << ", ";
        std::cout <<   *++it << ')';
    }

    // following should print all std::set elements
    /*
     * friend std::ostream& operator<<(std::ostream& os, const std::set<std::size_t>& s)
     * {
     *     std::set<std::size_t>::iterator it = s.begin();
     *     os << '(';
     *     for (; it != std::prev(s.end()); it++)
     *         os << *it << ", ";
     *     os <<   *++it << ')';
     *     return os;
     * }
     */

    void print_test()
    {
        std::cout << "heap\t= ";
        for (const auto &e : heap)
            std::cout << "[" << e << "] ";
        std::cout << '\n';

        for (const auto &e : umap) {
            // TODO: make printing std::set e.second with overloaded operator<< work!
            // std::cout << "umap value:[" << e.first << "] = " << e.second << '\n';

            std::cout << "umap[" << e.first << "] = ";
            print_set(e.second);
            std::cout << '\n';
        }
    }
#endif

    /**
     * clears everything inside the heap, O(n)
     */
    virtual void clear()
    {
        heap.clear();
        umap.clear();
    }

    /**
     * return the size of the heap, O(1)
     */
    virtual std::size_t size() const noexcept
    {
        return heap.size();
    }

    /**
     * check that priority queue is empty, O(1)
     */
    virtual bool isEmpty() const noexcept
    {
        return size() == 0;
    }

    /**
     * returns the value of the element with the lowest
     * priority in this priority queue, O(1).
     * throws error if the priority queue is empty.
     */
    virtual T peek() const
    {
        if (isEmpty())
            throw std::runtime_error("Empty Heap.");
        return heap.front();
    }

    /**
     * removes the root of the heap, O(log(n))
     */
    virtual T poll()
    {
        return removeAt(0);
    }

    /**
     * check that element is in heap, O(1)
     */
    virtual bool contains(const T &elem) const
    {
        return umap.contains(elem);
    }

    /**
     * Adds an element to the priority queue,
     * the element must not be null, O(log(n))
     */
    virtual void add(const T &elem)
    {
        heap.push_back(elem);
        const std::size_t index_last_elem { size() - 1 };
        mapAdd(elem, index_last_elem);
        swim(index_last_elem);
    }

    /**
     * removes a particular element from the heap, O(log(n))
     */
    virtual bool remove(const T &elem)
    {
        const std::size_t idx { mapGet(elem) };
        if (idx >= size()) return false; // not found => nothing to remove
        removeAt(idx);
        return true;
    }

    /**
     * Recursively checks if this heap is a min heap.
     * This method is just for testing purposes to make
     * sure the heap invariant is still being maintained.
     * Call this method with k=0 to start at the root.
     */
    bool isMinHeap(const std::size_t k) const
    {
        // If we are outside the bounds of the heap return true
        const std::size_t hsz { size() };
        if (k >= hsz) return true;

        const std::size_t l { 2 * k + 1 }; // left
        const std::size_t r { 2 * k + 2 }; // right

        /* Make sure that the current node k is less than
         * both of its children left, and right if they exist
         * return false otherwise to indicate an invalid heap */

        // LCOV_EXCL_BR_START
        if (l < hsz) {
            if (!less(k, l))
                return false; // LCOV_EXCL_LINE (0 hit = OK)
        }
        if (r < hsz) {
            if (!less(k, r))
                return false; // LCOV_EXCL_LINE (0 hit = OK)
        }

        // Recurse on both children to make sure they're also valid heaps
        return isMinHeap(l) && isMinHeap(r);
        // LCOV_EXCL_BR_STOP
    }

private:
    /**
     * Tests if the value of node i <= node j
     * This method assumes i & j are valid indices, O(1)
     */
    bool less(const std::size_t i, const std::size_t j) const
    {
        return heap.at(i) <= heap.at(j);
    }

    /**
     * bottom up node swim, O(log(n))
     */
    void swim(std::size_t k)
    {
        // grab the index of the next parent node WRT to k
        int parent = (k - 1) / 2;
        // Keep swimming while we have not reached the
        // root and while we are less than our parent.
        while (k > 0 && less(k, parent)) {
            // Exchange k with the parent
            swap(parent, k);
            k = parent;
            // Grab the index of the next parent node WRT to k
            parent = (k - 1) / 2;
        }
    }

    /**
     * top down node sink, O(log(n))
     */
    void sink(std::size_t k)
    {
        const std::size_t hsz { size() }; // heap size
        while (true) {
            const std::size_t l { 2 * k + 1 }; // left  node
            const std::size_t r { 2 * k + 2 }; // right node
            // assume left is the smallest node of the two children
            std::size_t smallest { l };

            // Find which is smaller left or right
            // If right is smaller set smallest to be right
            if (r  < hsz && less(r, l)) smallest = r;

            // Stop if we are outside the bounds of the tree
            // or stop early if we cannot sink k anymore
            if (l >= hsz || less(k, smallest)) break;

            // Move down the tree following the smallest node
            swap(smallest, k);
            k = smallest;
        }
    }

    /**
     * swap two nodes. Assumes i & j are valid, O(1)
     */
    void swap(const std::size_t i, const std::size_t j)
    {
        const T i_elem { heap.at(i) };
        const T j_elem { heap.at(j) };

        heap.at(i) = j_elem;
        heap.at(j) = i_elem;

        mapSwap(i_elem, j_elem, i, j);
    }

    /**
     * removes a node at particular index, O(log(n))
     */
    T removeAt(const std::size_t i)
    {
        if (isEmpty())
            throw std::runtime_error("Empty Heap.");

        const std::size_t index_last_elem { size() - 1 };
        const T removed_data { heap.at(i) };
        swap(i, index_last_elem);

        heap.erase(heap.begin()+index_last_elem);
        mapRemove(removed_data, index_last_elem);

        // Removed last element
        if (i == index_last_elem) return removed_data;
        const T elem { heap.at(i) };

        // Try sinking element
        sink(i);

        // If sinking did not work try swimming
        if (heap.at(i) == elem) swim(i);

        return removed_data;
    }

    /**
     * add a node value and its index to the map, O(log(n)).
     * key: val, value: set(of indexes)
     */
    void mapAdd(const T val, const std::size_t idx)
    {
        std::set<std::size_t> set { umap[val] };
        set.insert(idx);
        umap.insert_or_assign(val, set);
    }

    /**
     * Removes the index at a given value, O(log(n)).
     */
    void mapRemove(const T val, const std::size_t idx)
    {
        std::set<std::size_t> set { umap.at(val) };
        set.erase(idx);
        if (set.size() == 0) umap.erase(val);
        else umap.at(val) = set;
    }

    /**
     * Extract an index position for the given value.
     * if not found => returns heap size (one past the last).
     * NOTE: If a value exists multiple times in the heap
     * the highest index is returned (this was chosen arbitrarily)
     */
    std::size_t mapGet(const T val) const
    {
        std::set<std::size_t> set;
        try {
            set = umap.at(val);
        // FIXME: what is the 0 branch here?
        } catch(std::out_of_range const&) {
            return size();
        }
        return *set.rbegin();
    }

    /**
     * Exchange the index of two nodes internally within the map
     */
    void mapSwap(const T val1, const T val2, const std::size_t iv1, const std::size_t iv2)
    {
        std::set<std::size_t> set1 { umap.at(val1) };
        std::set<std::size_t> set2 { umap.at(val2) };

        set1.erase(iv1);
        set2.erase(iv2);

        set1.insert(iv2);
        set2.insert(iv1);

        umap.at(val1) = set1;
        umap.at(val2) = set2;
    }

};

} // namespace ds
} // namespace wndx
