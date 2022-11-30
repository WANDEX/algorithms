[![CI linux][ci_linux_bdg]]([ci_linux])
[![windows (msvc)][ci_windows_bdg]]([ci_windows])

# Algorithms
A collection of **0BSD**-licensed algorithms that I wrote for self-education and reuse in my works.

## Tests
**Require:** [GoogleTest (gtest)](https://github.com/google/googletest)

**To make clean build & run tests:**
```
./scripts/build.sh clean ctest
```

[Ninja (build system)](https://github.com/ninja-build/ninja)
is optional: change Generator **-G Ninja** in the above build script.

## Tree of Implemented DSA
include/\
└── cpp/\
    ├── datastructures/\
    │   ├── binary_indexed_tree/\
    │   │   └── [BITreeRangeQueryPointUpdate.hpp] ( *[BITreeRangeQueryPointUpdateTest.cpp]* )\
    │   ├── binary_search_tree/\
    │   │   ├── [BSTuptr.hpp] ( *[BSTuptrTest.cpp]* )\
    │   │   ├── [BSTuptrIterator.hpp]\
    │   │   └── [BSTuptrNode.hpp]\
    │   ├── linked_list/\
    │   │   └── [DoublyLinkedList.hpp] ( *[DoublyLinkedListTest.cpp]* )\
    │   ├── queue/\
    │   │   ├── [IQueue.hpp]\
    │   │   └── [ListQueue.hpp] ( *[ListQueueTest.cpp]* )\
    │   ├── queue_priority/\
    │   │   └── [BinaryHeapQ.hpp] ( *[BinaryHeapQTest.cpp]* )\
    │   ├── stack/\
    │   │   ├── [IStack.hpp]\
    │   │   └── [ListStack.hpp] ( *[ListStackTest.cpp]* )\
    │   └── union_find/\
    │       └── [UnionFind.hpp] ( *[UnionFindTest.cpp]* )\
    └── sort/\
        ├── [bubble_sort.hpp] ( *[SortTest.cpp]* )\
        ├── [insertion_sort.hpp] ( *[SortTest.cpp]* )\
        ├── [merge_sort.hpp] ( *[SortTest.cpp]* )\
        ├── [quick_sort.hpp] ( *[SortTest.cpp]* )\
        └── [selection_sort.hpp] ( *[SortTest.cpp]* )

[BITreeRangeQueryPointUpdate.hpp]: ./include/cpp/datastructures/binary_indexed_tree/BITreeRangeQueryPointUpdate.hpp
[BITreeRangeQueryPointUpdateTest.cpp]: ./tests/unit/datastructures/binary_indexed_tree/BITreeRangeQueryPointUpdateTest.cpp
[BSTuptr.hpp                 ]: ./include/cpp/datastructures/binary_search_tree/BSTuptr.hpp
[BSTuptrIterator.hpp         ]: ./include/cpp/datastructures/binary_search_tree/BSTuptrIterator.hpp
[BSTuptrNode.hpp             ]: ./include/cpp/datastructures/binary_search_tree/BSTuptrNode.hpp
[BSTuptrTest.cpp             ]: ./tests/unit/datastructures/binary_search_tree/BSTuptrTest.cpp
[BinaryHeapQ.hpp             ]: ./include/cpp/datastructures/queue_priority/BinaryHeapQ.hpp
[BinaryHeapQTest.cpp         ]: ./tests/unit/datastructures/queue_priority/BinaryHeapQTest.cpp
[DoublyLinkedList.hpp        ]: ./include/cpp/datastructures/linked_list/DoublyLinkedList.hpp
[DoublyLinkedListTest.cpp    ]: ./tests/unit/datastructures/linked_list/DoublyLinkedListTest.cpp
[IQueue.hpp                  ]: ./include/cpp/datastructures/queue/IQueue.hpp
[IStack.hpp                  ]: ./include/cpp/datastructures/stack/IStack.hpp
[ListQueue.hpp               ]: ./include/cpp/datastructures/queue/ListQueue.hpp
[ListQueueTest.cpp           ]: ./tests/unit/datastructures/queue/ListQueueTest.cpp
[ListStack.hpp               ]: ./include/cpp/datastructures/stack/ListStack.hpp
[ListStackTest.cpp           ]: ./tests/unit/datastructures/stack/ListStackTest.cpp
[SortTest.cpp                ]: ./tests/unit/sort/SortTest.cpp
[UnionFind.hpp               ]: ./include/cpp/datastructures/union_find/UnionFind.hpp
[UnionFindTest.cpp           ]: ./tests/unit/datastructures/union_find/UnionFindTest.cpp
[bubble_sort.hpp             ]: ./include/cpp/sort/bubble_sort.hpp
[insertion_sort.hpp          ]: ./include/cpp/sort/insertion_sort.hpp
[merge_sort.hpp              ]: ./include/cpp/sort/merge_sort.hpp
[quick_sort.hpp              ]: ./include/cpp/sort/quick_sort.hpp
[selection_sort.hpp          ]: ./include/cpp/sort/selection_sort.hpp

## Hall of Fame
DSA implemented in [Java](https://github.com/williamfiset/Algorithms),
repository contains many useful things: code, tests, slides, references,
[videos](https://www.youtube.com/c/WilliamFiset-videos/playlists).\
Huge respect to [William Fiset](https://github.com/williamfiset)
for the gratuitous dissemination of knowledge. :godmode:

## License
[0BSD](https://choosealicense.com/licenses/0bsd/)

[ci_linux       ]: https://github.com/WANDEX/algorithms/actions/workflows/ci_linux.yml
[ci_linux_bdg   ]: https://github.com/WANDEX/algorithms/actions/workflows/ci_linux.yml/badge.svg
[ci_windows     ]: https://github.com/WANDEX/algorithms/actions/workflows/ci_windows.yml
[ci_windows_bdg ]: https://github.com/WANDEX/algorithms/actions/workflows/ci_windows.yml/badge.svg]
