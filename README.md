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
│   ├── datastructures/\
│   ├── search/\
│   │   └── [binary_search.h](./include/c/search/binary_search.h)\
│   └── sort/\
│       ├── [bubble_sort.h](./include/c/sort/bubble_sort.h)\
│       ├── [insertion_sort.h](./include/c/sort/insertion_sort.h)\
│       ├── [quick_sort.h](./include/c/sort/quick_sort.h)\
│       └── [selection_sort.h](./include/c/sort/selection_sort.h)\
└── cpp/\
    ├── common/\
    │   ├── [gen.hpp](./include/cpp/common/gen.hpp)\
    │   ├── [macro.hpp](./include/cpp/common/macro.hpp)\
    │   ├── [pprint.hpp](./include/cpp/common/pprint.hpp)\
    │   └── [timer.hpp](./include/cpp/common/timer.hpp)\
    ├── datastructures/\
    │   ├── binarysearchtree/\
    │   │   ├── [BSTuptr.hpp](./include/cpp/datastructures/binarysearchtree/BSTuptr.hpp)\
    │   │   ├── [BSTuptrIterator.hpp](./include/cpp/datastructures/binarysearchtree/BSTuptrIterator.hpp)\
    │   │   └── [BSTuptrNode.hpp](./include/cpp/datastructures/binarysearchtree/BSTuptrNode.hpp)\
    │   ├── linkedlist/\
    │   │   └── [DoublyLinkedList.hpp](./include/cpp/datastructures/linkedlist/DoublyLinkedList.hpp)\
    │   ├── queue/\
    │   │   ├── [IQueue.hpp](./include/cpp/datastructures/queue/IQueue.hpp)\
    │   │   └── [ListQueue.hpp](./include/cpp/datastructures/queue/ListQueue.hpp)\
    │   ├── queue_priority/\
    │   │   └── [BinaryHeapQ.hpp](./include/cpp/datastructures/queue_priority/BinaryHeapQ.hpp)\
    │   ├── stack/\
    │   │   ├── [IStack.hpp](./include/cpp/datastructures/stack/IStack.hpp)\
    │   │   └── [ListStack.hpp](./include/cpp/datastructures/stack/ListStack.hpp)\
    │   └── unionfind/\
    │       └── [UnionFind.hpp](./include/cpp/datastructures/unionfind/UnionFind.hpp)\
    └── sort/\
        ├── [bubble_sort.hpp](./include/cpp/sort/bubble_sort.hpp)\
        ├── [insertion_sort.hpp](./include/cpp/sort/insertion_sort.hpp)\
        ├── [merge_sort.hpp](./include/cpp/sort/merge_sort.hpp)\
        ├── [quick_sort.hpp](./include/cpp/sort/quick_sort.hpp)\
        └── [selection_sort.hpp](./include/cpp/sort/selection_sort.hpp)\

## Hall of Fame
DSA implemented in [Java](https://github.com/williamfiset/Algorithms),
repository contains many useful things: code, tests, slides, references,
[videos](https://www.youtube.com/c/WilliamFiset-videos/playlists).\
Huge respect to [William Fiset](https://github.com/williamfiset)
for the gratuitous dissemination of knowledge. :godmode:

## License
[0BSD](https://choosealicense.com/licenses/0bsd/)
