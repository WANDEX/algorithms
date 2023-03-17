![coverage ](https://img.shields.io/badge/coverage:-gray)
![lines    ](https://img.shields.io/badge/L-99%25-brightgreen)
![functions](https://img.shields.io/badge/F-98%25-brightgreen)
![branches ](https://img.shields.io/badge/B-97%25-brightgreen)
![license..](https://img.shields.io/github/license/WANDEX/algorithms?color=black)\
[![CI linux][ci_linux_bdg]]([ci_linux])
[![windows (MSVC)][ci_windows_bdg]]([ci_windows])
[![macos (AppleClang)][ci_macos_bdg]]([ci_macos])

# Algorithms
A collection of **0BSD**-licensed algorithms that I wrote for self-education and reuse in my works.

## Tests
**Require:** [GoogleTest (gtest)](https://github.com/google/googletest)

**To make clean build & run tests:**
```
./scripts/build.sh clean ctest
```

**To make cleaner build (for edge cases) & run tests:**
```
./scripts/build.sh cleaner ctest
```

[Ninja (build system)](https://github.com/ninja-build/ninja)
is optional: change Generator **-G Ninja** in the above build script.

## Tree of Implemented DSA
include/\
└── wndx/\
    ├── ds/\
    │   ├── binary_indexed_tree/\
    │   │   └── [BITreeRQPU.hpp] ( *[BITreeRQPUTest.cpp]* )\
    │   ├── binary_search_tree/\
    │   │   ├── [BSTuptr.hpp] ( *[BSTuptrTest.cpp]* )\
    │   │   ├── [BSTuptrIterator.hpp]\
    │   │   └── [BSTuptrNode.hpp]\
    │   ├── linked_list/\
    │   │   └── [DLLraw.hpp] ( *[DLLrawTest.cpp]* )\
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

[BITreeRQPU.hpp              ]: ./include/wndx/ds/binary_indexed_tree/BITreeRQPU.hpp
[BITreeRQPUTest.cpp          ]: ./tests/units/datastructures/binary_indexed_tree/BITreeRQPUTest.cpp
[BSTuptr.hpp                 ]: ./include/wndx/ds/binary_search_tree/BSTuptr.hpp
[BSTuptrIterator.hpp         ]: ./include/wndx/ds/binary_search_tree/BSTuptrIterator.hpp
[BSTuptrNode.hpp             ]: ./include/wndx/ds/binary_search_tree/BSTuptrNode.hpp
[BSTuptrTest.cpp             ]: ./tests/units/datastructures/binary_search_tree/BSTuptrTest.cpp
[BinaryHeapQ.hpp             ]: ./include/wndx/ds/queue_priority/BinaryHeapQ.hpp
[BinaryHeapQTest.cpp         ]: ./tests/units/datastructures/queue_priority/BinaryHeapQTest.cpp
[DLLraw.hpp                  ]: ./include/wndx/ds/linked_list/DLLraw.hpp
[DLLrawTest.cpp              ]: ./tests/units/datastructures/linked_list/DLLrawTest.cpp
[IQueue.hpp                  ]: ./include/wndx/ds/queue/IQueue.hpp
[IStack.hpp                  ]: ./include/wndx/ds/stack/IStack.hpp
[ListQueue.hpp               ]: ./include/wndx/ds/queue/ListQueue.hpp
[ListQueueTest.cpp           ]: ./tests/units/datastructures/queue/ListQueueTest.cpp
[ListStack.hpp               ]: ./include/wndx/ds/stack/ListStack.hpp
[ListStackTest.cpp           ]: ./tests/units/datastructures/stack/ListStackTest.cpp
[SortTest.cpp                ]: ./tests/units/sort/SortTest.cpp
[UnionFind.hpp               ]: ./include/wndx/ds/union_find/UnionFind.hpp
[UnionFindTest.cpp           ]: ./tests/units/datastructures/union_find/UnionFindTest.cpp
[bubble_sort.hpp             ]: ./include/wndx/sort/bubble_sort.hpp
[insertion_sort.hpp          ]: ./include/wndx/sort/insertion_sort.hpp
[merge_sort.hpp              ]: ./include/wndx/sort/merge_sort.hpp
[quick_sort.hpp              ]: ./include/wndx/sort/quick_sort.hpp
[selection_sort.hpp          ]: ./include/wndx/sort/selection_sort.hpp

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
[ci_windows_bdg ]: https://github.com/WANDEX/algorithms/actions/workflows/ci_windows.yml/badge.svg
[ci_macos       ]: https://github.com/WANDEX/algorithms/actions/workflows/ci_macos.yml
[ci_macos_bdg   ]: https://github.com/WANDEX/algorithms/actions/workflows/ci_macos.yml/badge.svg
