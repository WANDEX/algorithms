/* compile cmd g++/clang++ :
g++ ./merge_sort.cpp -std=c++11 -Wall -O3 -o ./mst.out
 */

#include <cstddef>  // std::size_t
#include <array>    // std::array
#include <iostream>

// preprocessor directive to toggle visualization of sorting algorithm steps
#define SHOW_STEPS false // NOTE: enable only temporary in educational purposes!

template <const std::size_t n>
void merge(std::array<int, n> &A, int p, int q, int r);

/**
 * print C array
 */
void print_array(const int *A, const std::size_t n, const std::string prefix="");

/**
 * print std::array
 */
template <const std::size_t n>
void print_array(const std::array<int, n> &A, const std::string prefix="")
{
    std::cerr << prefix << "{";
    for (std::size_t i = 0; i < n - 1; i++)
        std::cerr << A[i] << ", ";
    std::cerr << A[n - 1] << "}" << std::endl; // last element without additional ', '
}

/**
 * merge sort algorithm.
 * recursively calls itself to divide array until it becomes a single element,
 * then merges the sorted sub-arrays.
 * @p left_index, @r right_index
 */
template <const std::size_t n>
void merge_sort(std::array<int, n> &A, int p, int r)
{
    if (p >= r) return;
    int q = (p + r) / 2; // middle_index (round down implicitly)
    merge_sort(A, p, q);
    merge_sort(A, q + 1, r);
    merge(A, p, q, r);
}

/**
 * merge of merge sort algorithm.
 * sorts & merges the sorted sub-arrays.
 * (abbreviated as it is a well-known algorithm)
 * @p left_index, @q middle_index, @r right_index
 */
template <const std::size_t n>
void merge(std::array<int, n> &A, int p, int q, int r)
{
    int x{0}, y{0}, z{0};
    int lsub = q - p + 1;
    int rsub = r - q;
    int la[lsub], ra[rsub];

    // copy separate fragments of the input array into two temporary arrays
    for (x = 0; x < lsub; x++) la[x] = A[p + x];
    for (y = 0; y < rsub; y++) ra[y] = A[q + y + 1];

#if SHOW_STEPS
    print_array(A, "A (top)\t");
    print_array(la, x, "la\t");
    print_array(ra, y, "ra\t");
#endif

    x = 0; y = 0; z = p;
    while (x < lsub && y < rsub) {
        if (la[x] <= ra[y])
            A[z++] = la[x++];
        else
            A[z++] = ra[y++];
    }

#if SHOW_STEPS
    print_array(A, "A (mid)\t");
#endif

    while (x < lsub) A[z++] = la[x++];
    while (y < rsub) A[z++] = ra[y++];

#if SHOW_STEPS
    print_array(A, "A (bot)\t");
#endif
}

template <const std::size_t n>
bool merge_sort_test(std::array<int, n> &A, const std::array<int, n> &A_expected)
{
    merge_sort(A, 0, n - 1);
    if (A == A_expected) {
        print_array(A, "[PASS] ");
        return true;
    } else {
        std::cerr << "Produced array does not equal to the expected!" << std::endl;
        print_array(A, "[FAIL]  res: ");
        print_array(A_expected, "\texp: ");
        return false;
    }
}

bool merge_sort_test_0()
{
    const std::size_t n = 3;
    std::array<int, n> A = {7, 5, 6};
    std::array<int, n> E = {5, 6, 7}; // E - stands for Expected after sort
    return merge_sort_test(A, E);
}

bool merge_sort_test_1()
{
    const std::size_t n = 5;
    std::array<int, n> A = {0, 3, 5, 1, 4};
    std::array<int, n> E = {0, 1, 3, 4, 5};
    return merge_sort_test(A, E);
}

bool merge_sort_test_2()
{
    const std::size_t n = 8;
    std::array<int, n> A = {1, 2, 7, 0, 3, 2, 5, 6};
    std::array<int, n> E = {0, 1, 2, 2, 3, 5, 6, 7};
    return merge_sort_test(A, E);
}

bool merge_sort_test_3()
{
    const std::size_t n = 10;
    std::array<int, n> A = {7, 69, 10, 81, 54, 53, 99, 42, 85, 68};
    std::array<int, n> E = {7, 10, 42, 53, 54, 68, 69, 81, 85, 99};
    return merge_sort_test(A, E);
}

bool merge_sort_test_4()
{
    const std::size_t n = 11;
    std::array<int, n> A = {50, 84, 43, 4, 47, 45, 62, 72, 93, 6, 1};
    std::array<int, n> E = {1, 4, 6, 43, 45, 47, 50, 62, 72, 84, 93};
    return merge_sort_test(A, E);
}

bool merge_sort_test_5()
{
    const std::size_t n = 42; // the meaning of life :)
    std::array<int, n> A = {203, 730, 187, 609, 536, 480, 106, 2, 4, 605, 42, 384, 724, 691, 723, 0, 467, 820, 676, 818, 427, 821, 910, 8, 272, 338, 762, 819, 109, 144, 779, 990, 679, 838, 450, 141, 44, 580, 289, 471, 573, 823};
    std::array<int, n> E = {0, 2, 4, 8, 42, 44, 106, 109, 141, 144, 187, 203, 272, 289, 338, 384, 427, 450, 467, 471, 480, 536, 573, 580, 605, 609, 676, 679, 691, 723, 724, 730, 762, 779, 818, 819, 820, 821, 823, 838, 910, 990};
    return merge_sort_test(A, E);
}

/**
 * one general function for calling all tests
 */
int merge_sort_tests()
{
    std::size_t pass = 0, fail = 0;

    std::cerr << "*** TESTS (merge sort) ***" << std::endl;

    /* NOTE: if SHOW_STEPS is true - solely to increase readability,
     * temporary comment out all following tests,
     * except for the specific one whose steps you are interested in.
     */

    (merge_sort_test_0()) ? pass++ : fail++;
    (merge_sort_test_1()) ? pass++ : fail++;
    (merge_sort_test_2()) ? pass++ : fail++;
    (merge_sort_test_3()) ? pass++ : fail++;
    (merge_sort_test_4()) ? pass++ : fail++;

#if !SHOW_STEPS
    /* Disable following tests in SHOW_STEPS mode
     * (solely for readability -> the array is too big for a human to understand the steps)
     */
    (merge_sort_test_5()) ? pass++ : fail++;
#endif

    std::cerr << "[PASS] " << pass << "/" << pass + fail << std::endl;
    if (fail > 0) {
        std::cerr << "[FAIL] " << fail << std::endl;
        return 1;
    }
    return 0;
}

int main()
{
    return merge_sort_tests();
}
