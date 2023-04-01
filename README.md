![coverage ](https://img.shields.io/badge/coverage:-gray)
![lines    ](https://img.shields.io/badge/L-100%25-brightgreen)
![functions](https://img.shields.io/badge/F-99%25-brightgreen)
![branches ](https://img.shields.io/badge/B-98%25-brightgreen)
![license..](https://img.shields.io/github/license/WANDEX/algorithms?color=black)\
[![CI linux][ci_linux_bdg]]([ci_linux])
[![windows (MSVC)][ci_windows_bdg]]([ci_windows])
[![macos (AppleClang)][ci_macos_bdg]]([ci_macos])

# wndxlib
Header only C++20 library of Data Structures & Algorithms written mainly for self-education.

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

## Integration
`CMakeLists.txt` that uses `wndxlib` can look like this:
```cmake
project(project_1337)

include(FetchContent)
FetchContent_Declare(
  wndxlib # v0.0.1.0-rc1
  URL https://github.com/WANDEX/algorithms/releases/download/v0.0.1.0-rc1/wndxlib-src.zip
)
FetchContent_MakeAvailable(wndxlib)

add_executable(project_1337)
target_sources(project_1337 PRIVATE main.cpp)
target_link_libraries(project_1337 PRIVATE wandex::wndx)
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

[ci_linux       ]: https://github.com/WANDEX/algorithms/actions/workflows/ci_linux.yml
[ci_linux_bdg   ]: https://github.com/WANDEX/algorithms/actions/workflows/ci_linux.yml/badge.svg
[ci_windows     ]: https://github.com/WANDEX/algorithms/actions/workflows/ci_windows.yml
[ci_windows_bdg ]: https://github.com/WANDEX/algorithms/actions/workflows/ci_windows.yml/badge.svg
[ci_macos       ]: https://github.com/WANDEX/algorithms/actions/workflows/ci_macos.yml
[ci_macos_bdg   ]: https://github.com/WANDEX/algorithms/actions/workflows/ci_macos.yml/badge.svg
