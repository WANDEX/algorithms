![coverage ](https://img.shields.io/badge/coverage:-gray)
![lines    ](https://img.shields.io/badge/L-92%25-green)
![functions](https://img.shields.io/badge/F-98%25-brightgreen)
![branches ](https://img.shields.io/badge/B-90%25-green)
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
    └── algo/\
        ├── ds/\
        │   ├── [BinaryHeapQ.hpp] ( *[BinaryHeapQ.t.cpp]* )\
        │   ├── [BITreeRQPU.hpp] ( *[BITreeRQPU.t.cpp]* )\
        │   ├── [BSTuptr.hpp] ( *[BSTuptr.t.cpp]* )\
        │   ├── [BSTuptrIterator.hpp]\
        │   ├── [BSTuptrNode.hpp]\
        │   ├── [DLLraw.hpp] ( *[DLLraw.t.cpp]* )\
        │   ├── [IQueue.hpp]\
        │   ├── [IStack.hpp]\
        │   ├── [ListQueue.hpp] ( *[ListQueue.t.cpp]* )\
        │   ├── [ListStack.hpp] ( *[ListStack.t.cpp]* )\
        │   └── [UnionFind.hpp] ( *[UnionFind.t.cpp]* )\
        └── sort/\
            ├── [bubble_sort.hpp] ( *[sort.t.cpp]* )\
            ├── [insertion_sort.hpp] ( *[sort.t.cpp]* )\
            ├── [merge_sort.hpp] ( *[sort.t.cpp]* )\
            ├── [quick_sort.hpp] ( *[sort.t.cpp]* )\
            └── [selection_sort.hpp] ( *[sort.t.cpp]* )

[BITreeRQPU.hpp              ]: ./include/wndx/algo/ds/BITreeRQPU.hpp
[BITreeRQPU.t.cpp            ]: ./tests/units/ds/BITreeRQPU.t.cpp
[BSTuptr.hpp                 ]: ./include/wndx/algo/ds/BSTuptr.hpp
[BSTuptr.t.cpp               ]: ./tests/units/ds/BSTuptr.t.cpp
[BSTuptrIterator.hpp         ]: ./include/wndx/algo/ds/BSTuptrIterator.hpp
[BSTuptrNode.hpp             ]: ./include/wndx/algo/ds/BSTuptrNode.hpp
[BinaryHeapQ.hpp             ]: ./include/wndx/algo/ds/BinaryHeapQ.hpp
[BinaryHeapQ.t.cpp           ]: ./tests/units/ds/BinaryHeapQ.t.cpp
[DLLraw.hpp                  ]: ./include/wndx/algo/ds/DLLraw.hpp
[DLLraw.t.cpp                ]: ./tests/units/ds/DLLraw.t.cpp
[IQueue.hpp                  ]: ./include/wndx/algo/ds/IQueue.hpp
[IStack.hpp                  ]: ./include/wndx/algo/ds/IStack.hpp
[ListQueue.hpp               ]: ./include/wndx/algo/ds/ListQueue.hpp
[ListQueue.t.cpp             ]: ./tests/units/ds/ListQueue.t.cpp
[ListStack.hpp               ]: ./include/wndx/algo/ds/ListStack.hpp
[ListStack.t.cpp             ]: ./tests/units/ds/ListStack.t.cpp
[UnionFind.hpp               ]: ./include/wndx/algo/ds/UnionFind.hpp
[UnionFind.t.cpp             ]: ./tests/units/ds/UnionFind.t.cpp
[bubble_sort.hpp             ]: ./include/wndx/algo/sort/bubble_sort.hpp
[insertion_sort.hpp          ]: ./include/wndx/algo/sort/insertion_sort.hpp
[merge_sort.hpp              ]: ./include/wndx/algo/sort/merge_sort.hpp
[quick_sort.hpp              ]: ./include/wndx/algo/sort/quick_sort.hpp
[selection_sort.hpp          ]: ./include/wndx/algo/sort/selection_sort.hpp
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
