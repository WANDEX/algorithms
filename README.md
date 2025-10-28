![coverage ](https://img.shields.io/badge/coverage:-gray)
![lines    ](https://img.shields.io/badge/L-99%25-brightgreen)
![functions](https://img.shields.io/badge/F-99%25-brightgreen)
![branches ](https://img.shields.io/badge/B-94%25-green)
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

#### Detailed Instruction
<details><summary><b>
MSVC -- Windows platform using Visual Studio compiler in git-bash shell -- MINGW
</b></summary>

<details><summary>
NOTE: Always preferred version of CMake is a modern stable release compiled from source!
</summary>

* Check project root `CMakeLists.txt` `cmake_minimum_required` VERSION.
* CMake version installed with your Visual Studio may be obsolete.
* Microsoft also patches CMake versions for their MSVC toolchain.
* Such patched and/or obsolete versions may be e.g. `cmake --version` *3.26.0-msvc3* etc.
* Proper work with patched versions of CMake is not guaranteed and may not be guaranteed!
</details>

##### prepare build environment:
```sh
## backup unmodified PATH
$ export PATH0="$PATH"

## add MSVC toolchain dir with cl compiler to the PATH
$ export PATH="$PATH:/c/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/14.36.32532/bin/Hostx64/x64"

## add DrMemory to the PATH
$ export PATH="$PATH:/c/wndx/src/top/DrMemory-Windows-2.6.0/bin64"

## make new build dir
$ bdir="./build/msvc"
$ [ -d "$bdir" ] &&   rm -rf "$bdir"
$ [ -d "$bdir" ] || mkdir -p "$bdir"
```

##### compile, test, memcheck:
```sh
## cmake configure
$ cmake -S . -B "$bdir" -G "Visual Studio 17 2022" -D WNDX_ALGO_BUILD_TESTS=ON -D WNDX_ALGO_MEMCHECK_ENABLE=ON

## cmake build
$ cmake --build "$bdir"

## execute unit tests
$ ctest --test-dir "$bdir/tests/units/"

## check onto memory leaks executing unit tests via DrMemory
$ cmake --build "$bdir" --target memcheck
```
</details>

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
