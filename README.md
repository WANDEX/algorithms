![coverage ](https://img.shields.io/badge/coverage:-gray)
![lines    ](https://img.shields.io/badge/L-100%25-brightgreen)
![functions](https://img.shields.io/badge/F-99%25-brightgreen)
![branches ](https://img.shields.io/badge/B-98%25-brightgreen)
![license..](https://img.shields.io/github/license/WANDEX/algorithms?color=black)
[![CI/CD cmake-multi-platform][cicd_cmake_mp_bdg]][cicd_cmake_mp_url]

# wndx::algo
Header only C++20 lib of Data Structures Algorithms written mainly for self-education and exploration of C++.

## Requirements
* git submodule: [wndx::sane](https://github.com/WANDEX/wndx_sane)
* Tests Require: [GoogleTest (gtest)](https://github.com/google/googletest) (CMake fetch content at configure time)

## Build
```sh
$ git clone --recurse-submodules git@github.com:WANDEX/algorithms.git && cd algorithms
$ cmake -E make_directory build
$ cmake -S . -B build
$ cmake --build build
```

## Integration
`CMakeLists.txt` that uses `wndx::algo` can look like this:
```cmake
project(project_1337)

include(FetchContent)
FetchContent_Declare(
  wndx_algo
  GIT_REPOSITORY https://github.com/WANDEX/algorithms.git
  GIT_TAG        0.0.3 # latest tag commit hash is preferred
)
FetchContent_MakeAvailable(wndx_algo)

add_executable(project_1337)
target_sources(project_1337 PRIVATE main.cpp)
target_link_libraries(project_1337 PRIVATE wndx_algo::algo)
```

## Tree of Implemented DSA
include/\
└── wndx/\
    ├── ds/\
    │   ├── binary_indexed_tree/\
    │   │   └── [BITreeRQPU.hpp] ( *[BITreeRQPU.t.cpp]* )\
    │   ├── binary_search_tree/\
    │   │   ├── [BSTuptr.hpp] ( *[BSTuptr.t.cpp]* )\
    │   │   ├── [BSTuptrIterator.hpp]\
    │   │   └── [BSTuptrNode.hpp]\
    │   ├── linked_list/\
    │   │   └── [DLLraw.hpp] ( *[DLLraw.t.cpp]* )\
    │   ├── queue/\
    │   │   ├── [IQueue.hpp]\
    │   │   └── [ListQueue.hpp] ( *[ListQueue.t.cpp]* )\
    │   ├── queue_priority/\
    │   │   └── [BinaryHeapQ.hpp] ( *[BinaryHeapQ.t.cpp]* )\
    │   ├── stack/\
    │   │   ├── [IStack.hpp]\
    │   │   └── [ListStack.hpp] ( *[ListStack.t.cpp]* )\
    │   └── union_find/\
    │       └── [UnionFind.hpp] ( *[UnionFind.t.cpp]* )\
    └── sort/\
        ├── [bubble_sort.hpp] ( *[sort.t.cpp]* )\
        ├── [insertion_sort.hpp] ( *[sort.t.cpp]* )\
        ├── [merge_sort.hpp] ( *[sort.t.cpp]* )\
        ├── [quick_sort.hpp] ( *[sort.t.cpp]* )\
        └── [selection_sort.hpp] ( *[sort.t.cpp]* )

[BITreeRQPU.hpp              ]: ./include/wndx/ds/binary_indexed_tree/BITreeRQPU.hpp
[BITreeRQPU.t.cpp            ]: ./tests/units/ds/binary_indexed_tree/BITreeRQPU.t.cpp
[BSTuptr.hpp                 ]: ./include/wndx/ds/binary_search_tree/BSTuptr.hpp
[BSTuptr.t.cpp               ]: ./tests/units/ds/binary_search_tree/BSTuptr.t.cpp
[BSTuptrIterator.hpp         ]: ./include/wndx/ds/binary_search_tree/BSTuptrIterator.hpp
[BSTuptrNode.hpp             ]: ./include/wndx/ds/binary_search_tree/BSTuptrNode.hpp
[BinaryHeapQ.hpp             ]: ./include/wndx/ds/queue_priority/BinaryHeapQ.hpp
[BinaryHeapQ.t.cpp           ]: ./tests/units/ds/queue_priority/BinaryHeapQ.t.cpp
[DLLraw.hpp                  ]: ./include/wndx/ds/linked_list/DLLraw.hpp
[DLLraw.t.cpp                ]: ./tests/units/ds/linked_list/DLLraw.t.cpp
[IQueue.hpp                  ]: ./include/wndx/ds/queue/IQueue.hpp
[IStack.hpp                  ]: ./include/wndx/ds/stack/IStack.hpp
[ListQueue.hpp               ]: ./include/wndx/ds/queue/ListQueue.hpp
[ListQueue.t.cpp             ]: ./tests/units/ds/queue/ListQueue.t.cpp
[ListStack.hpp               ]: ./include/wndx/ds/stack/ListStack.hpp
[ListStack.t.cpp             ]: ./tests/units/ds/stack/ListStack.t.cpp
[UnionFind.hpp               ]: ./include/wndx/ds/union_find/UnionFind.hpp
[UnionFind.t.cpp             ]: ./tests/units/ds/union_find/UnionFind.t.cpp
[bubble_sort.hpp             ]: ./include/wndx/sort/bubble_sort.hpp
[insertion_sort.hpp          ]: ./include/wndx/sort/insertion_sort.hpp
[merge_sort.hpp              ]: ./include/wndx/sort/merge_sort.hpp
[quick_sort.hpp              ]: ./include/wndx/sort/quick_sort.hpp
[selection_sort.hpp          ]: ./include/wndx/sort/selection_sort.hpp
[sort.t.cpp                  ]: ./tests/units/sort/sort.t.cpp

## Hall of Fame
DSA implemented in [Java](https://github.com/williamfiset/Algorithms),
repository contains many useful things: code, tests, slides, references,
[videos](https://www.youtube.com/c/WilliamFiset-videos/playlists).\
Huge respect to [William Fiset](https://github.com/williamfiset)
for the gratuitous dissemination of knowledge. :godmode:

## License
[0BSD](https://choosealicense.com/licenses/0bsd/)

[cicd_cmake_mp_url]: https://github.com/WANDEX/algorithms/actions/workflows/cicd_cmake_multi_platform.yml
[cicd_cmake_mp_bdg]: https://github.com/WANDEX/algorithms/actions/workflows/cicd_cmake_multi_platform.yml/badge.svg?event=push
