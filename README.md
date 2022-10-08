# Algorithms

A collection of **0BSD**-licensed algorithms that I wrote for self-education and reuse in my works.

## Tests
##### Requires: [GoogleTest (gtest)](https://github.com/google/googletest)
to make build & run tests:
```
./scripts/build.sh tests
```
to make clean build & run tests:
```
./scripts/build.sh clean tests
```

## Tree Of Implemented DSA
include/\
├── c/\
│   ├── search/\
│   │   └── [binary_search.h] ( *[binary_search.cc]* )\
│   └── sort/\
│       ├── [bubble_sort.h] ( *[SortTest.cpp]* )\
│       ├── [insertion_sort.h] ( *[SortTest.cpp]* )\
│       ├── [quick_sort.h] ( *[SortTest.cpp]* )\
│       └── [selection_sort.h] ( *[SortTest.cpp]* )\
└── cpp/\
    ├── common/\
    │   ├── [gen.hpp] ( *[BSTuptrTest.cpp] | [BinaryHeapQTest.cpp] | [SortTest.cpp]* )\
    │   ├── [macro.hpp]\
    │   ├── [pprint.hpp] ( *[BSTuptrTest.cpp]* )\
    │   └── [timer.hpp]\
    ├── datastructures/\
    │   ├── binarysearchtree/\
    │   │   ├── [BSTuptr.hpp] ( *[BSTuptrTest.cpp]* )\
    │   │   ├── [BSTuptrIterator.hpp]\
    │   │   └── [BSTuptrNode.hpp]\
    │   ├── linkedlist/\
    │   │   └── [DoublyLinkedList.hpp] ( *[DoublyLinkedListTest.cpp]* )\
    │   ├── queue/\
    │   │   ├── [IQueue.hpp]\
    │   │   └── [ListQueue.hpp] ( *[ListQueueTest.cpp]* )\
    │   ├── queue_priority/\
    │   │   └── [BinaryHeapQ.hpp] ( *[BinaryHeapQTest.cpp]* )\
    │   ├── stack/\
    │   │   ├── [IStack.hpp]\
    │   │   └── [ListStack.hpp] ( *[ListStackTest.cpp]* )\
    │   └── unionfind/\
    │       └── [UnionFind.hpp] ( *[UnionFindTest.cpp]* )\
    └── sort/\
        ├── [bubble_sort.hpp] ( *[SortTest.cpp]* )\
        ├── [insertion_sort.hpp] ( *[SortTest.cpp]* )\
        ├── [merge_sort.hpp] ( *[SortTest.cpp]* )\
        ├── [quick_sort.hpp] ( *[SortTest.cpp]* )\
        └── [selection_sort.hpp] ( *[SortTest.cpp]* )

[BSTuptr.hpp                 ]: ./include/cpp/datastructures/binarysearchtree/BSTuptr.hpp
[BSTuptrIterator.hpp         ]: ./include/cpp/datastructures/binarysearchtree/BSTuptrIterator.hpp
[BSTuptrNode.hpp             ]: ./include/cpp/datastructures/binarysearchtree/BSTuptrNode.hpp
[BSTuptrTest.cpp             ]: ./tests/unit/datastructures/binarysearchtree/BSTuptrTest.cpp
[BinaryHeapQ.hpp             ]: ./include/cpp/datastructures/queue_priority/BinaryHeapQ.hpp
[BinaryHeapQTest.cpp         ]: ./tests/unit/datastructures/queue_priority/BinaryHeapQTest.cpp
[DoublyLinkedList.hpp        ]: ./include/cpp/datastructures/linkedlist/DoublyLinkedList.hpp
[DoublyLinkedListTest.cpp    ]: ./tests/unit/datastructures/linkedlist/DoublyLinkedListTest.cpp
[IQueue.hpp                  ]: ./include/cpp/datastructures/queue/IQueue.hpp
[IStack.hpp                  ]: ./include/cpp/datastructures/stack/IStack.hpp
[ListQueue.hpp               ]: ./include/cpp/datastructures/queue/ListQueue.hpp
[ListQueueTest.cpp           ]: ./tests/unit/datastructures/queue/ListQueueTest.cpp
[ListStack.hpp               ]: ./include/cpp/datastructures/stack/ListStack.hpp
[ListStackTest.cpp           ]: ./tests/unit/datastructures/stack/ListStackTest.cpp
[SortTest.cpp                ]: ./tests/unit/sort/SortTest.cpp
[UnionFind.hpp               ]: ./include/cpp/datastructures/unionfind/UnionFind.hpp
[UnionFindTest.cpp           ]: ./tests/unit/datastructures/unionfind/UnionFindTest.cpp
[binary_search.cc            ]: ./tests/unit/search/binary_search.cc
[binary_search.h             ]: ./include/c/search/binary_search.h
[bubble_sort.h               ]: ./include/c/sort/bubble_sort.h
[bubble_sort.hpp             ]: ./include/cpp/sort/bubble_sort.hpp
[gen.hpp                     ]: ./include/cpp/common/gen.hpp
[insertion_sort.h            ]: ./include/c/sort/insertion_sort.h
[insertion_sort.hpp          ]: ./include/cpp/sort/insertion_sort.hpp
[macro.hpp                   ]: ./include/cpp/common/macro.hpp
[merge_sort.hpp              ]: ./include/cpp/sort/merge_sort.hpp
[pprint.hpp                  ]: ./include/cpp/common/pprint.hpp
[quick_sort.h                ]: ./include/c/sort/quick_sort.h
[quick_sort.hpp              ]: ./include/cpp/sort/quick_sort.hpp
[selection_sort.h            ]: ./include/c/sort/selection_sort.h
[selection_sort.hpp          ]: ./include/cpp/sort/selection_sort.hpp
[timer.hpp                   ]: ./include/cpp/common/timer.hpp

## Hall of Fame
DSA implemented in [Java](https://github.com/williamfiset/Algorithms),
repository contains many useful things: code, tests, slides, references,
[videos](https://www.youtube.com/c/WilliamFiset-videos/playlists).\
Huge respect to [William Fiset](https://github.com/williamfiset)
for the gratuitous dissemination of knowledge. :godmode:

## License
[0BSD](https://choosealicense.com/licenses/0bsd/)
