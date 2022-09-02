#pragma once
/**
 * A min priority queue implementation using a binary heap.
 * This implementation tracks each element inside the binary heap
 * with a hash table for quick removals.
 */

// #include "pprint.hpp"   // XXX
#include <iostream>     // XXX

#include <algorithm>    // std::max
#include <cstddef>      // size_t
#include <map>
#include <set>
#include <sstream>      // ostream
#include <stdexcept>    // runtime_error, out_of_range
#include <vector>

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
    std::map<T, std::set<std::size_t>> map;

public:
    // Construct and initially empty priority queue
    BinaryHeapQ()
    {}
    // Construct a priority queue with an initial capacity
    BinaryHeapQ(const std::size_t sz)
    {
        heap.reserve(sz);
    }

    // FIXME: why not prints set?
    // XXX breaks std::string <<
    // friend std::ostream& operator<<(std::ostream &os, const std::set<std::size_t> &s)
    // {
    //     for (auto const &i : s)
    //         os << i << " ";
    //     return os;
    // }

    /**
     * Construct a priority queue using heapify in O(n) time, a great explanation can be found at:
     * http://www.cs.umd.edu/~meesh/351/mount/lectures/lect14-heapsort-analysis-part.pdf
     */
    BinaryHeapQ(const std::vector<T> &v)
    {
        const std::size_t hsz = v.size();
        heap.reserve(hsz);
        // place all element in the heap
        for (std::size_t i = 0; i < hsz; i++) {
            mapAdd(v[i], i);
            heap.push_back(v[i]);
        }
        // Heapify process, O(n)
        const int calc = (hsz / 2) - 1;
        for (int i = std::max(0, calc); i >= 0; i--) sink(i);
    }


    virtual ~BinaryHeapQ()
    {
        clear();
    }

    void print_set(const std::set<std::size_t> &s)
    {
        std::set<std::size_t>::iterator it { s.begin() };
        std::cout << '(';
        for (; it != std::prev(s.end()); it++)
            std::cout << *it << ", ";
        std::cout <<   *++it << ')';
    }

    void print_test()
    {
        for (const auto &e : map) {
            // TODO: make printing set work!
            // std::cout << '[' << e.first << "=" << e.second << "]\n";
            std::cout << "map value:[" << e.first << "] ";
            print_set(e.second);
            std::cout << '\n';
        }
        for (const auto &e : heap) {
            std::cout << "heap value:[" << e << "]\n";
        }
    }

    /**
     * clears everything inside the heap, O(n)
     */
    virtual void clear()
    {
        heap.clear();
        map.clear();
    }

    /**
     * return the size of the heap, O(1)
     */
    virtual std::size_t size() const
    {
        return heap.size();
    }

    /**
     * check that priority queue is empty, O(1)
     */
    virtual bool isEmpty() const
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
        if (isEmpty()) throw std::runtime_error("Empty Heap");
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
     * check that element is in heap
     */
    virtual bool contains(const T &elem) const
    {
        // FIXME: should be O(1)
        return map.contains(elem); // log(n)
    }

    /**
     * Adds an element to the priority queue,
     * the element must not be null, O(log(n))
     */
    virtual void add(const T &elem)
    {
        // if (elem == nullptr) throw std::invalid_argument("Invalid Index");

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
        // Logarithmic removal with map, O(log(n))
        std::size_t idx{ mapGet(elem) };
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
    bool isMinHeap(const std::size_t k)
    {
        // If we are outside the bounds of the heap return true
        const std::size_t hsz { size() };
        if (k >= hsz) return true;

        const std::size_t l { 2 * k + 1 }; // left
        const std::size_t r { 2 * k + 2 }; // right

        /* Make sure that the current node k is less than
         * both of its children left, and right if they exist
         * return false otherwise to indicate an invalid heap */
        if (l < hsz && !less(k, l)) return false;
        if (r < hsz && !less(k, r)) return false;

        // Recurse on both children to make sure they're also valid heaps
        return isMinHeap(l) && isMinHeap(r);
    }

private:
    /**
     * Tests if the value of node i <= node j
     * This method assumes i & j are valid indices, O(1)
     */
    bool less(const std::size_t i, const std::size_t j) const
    {
        return heap[i] <= heap[j];
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
        const T i_elem { heap[i] };
        const T j_elem { heap[j] };

        heap[i] = j_elem;
        heap[j] = i_elem;

        mapSwap(i_elem, j_elem, i, j);
    }

    /**
     * removes a node at particular index, O(log(n))
     */
    T removeAt(const std::size_t i)
    {
        if (isEmpty()) throw std::runtime_error("Empty Heap");

        const std::size_t index_last_elem { size() - 1 };
        T removed_data { heap[i] };
        swap(i, index_last_elem);

        heap.erase(heap.begin()+index_last_elem);
        mapRemove(removed_data, index_last_elem);

        // Removed last element
        if (i == index_last_elem) return removed_data;
        T elem { heap[i] };

        // Try sinking element
        sink(i);

        // If sinking did not work try swimming
        if (heap[i] == elem) swim(i);

        return removed_data;
    }

    /**
     * add a node value and its index to the map.
     * key: val, value: set(of indexes)
     */
    void mapAdd(const T val, const std::size_t idx)
    {
        std::set<std::size_t> set { map[val] };
        set.insert(idx);
        map.insert_or_assign(val, set);
    }

    /**
     * Removes the index at a given value, O(log(n)).
     */
    void mapRemove(const T val, const std::size_t idx)
    {
        std::set<std::size_t> set { map[val] };
        set.erase(idx); // O(log(c.size()) + c.count(key))
        if (set.size() == 0) map.erase(val);
        else map[val] = set;
    }

    /**
     * Extract an index position for the given value.
     * if not found => returns heap size (one past the last).
     * NOTE: If a value exists multiple times in the heap
     * the highest index is returned (this has arbitrarily been chosen)
     */
    std::size_t mapGet(const T val)
    {
        std::set<std::size_t> set;
        try {
            set = map.at(val);
        } catch(std::out_of_range) {
            return size();
        }
        if (set.empty())
            return size();
        return *set.rbegin();
    }

    /**
     * Exchange the index of two nodes internally within the map
     */
    void mapSwap(const T val1, const T val2, const std::size_t iv1, const std::size_t iv2)
    {
        std::set<std::size_t> set1 { map[val1] };
        std::set<std::size_t> set2 { map[val2] };

        set1.erase(iv1);
        set2.erase(iv2);

        set1.insert(iv2);
        set2.insert(iv1);

        map[val1] = set1;
        map[val2] = set2;
    }

};
