/**
 * old test assignment:
 * A function that truncates spaces at the end of the string passed to it.
 * The function must be written to work with very long lines
 * with a very large number of spaces, optimized for the number of memory accesses.
 * signature: void TrimRight( char *s );
 */

// 0 -> use STL for our implementation of trim right function - rtrim.
// || use boost::trim_right for comparison result with our function
#define USE_BOOST 0
#if USE_BOOST
#include <boost/algorithm/string/trim.hpp>
#endif // USE_BOOST

#include <gtest/gtest.h>

#include <algorithm>            // std::find_if
#include <locale>               // std::isspace(std::locale)
#include <string>
#include <string_view>


/**
 * @brief trim ws chars from the end of C-style string by placing null terminator.
 *
 * ws - whitespace.
 *
 * to make function more robust we check for all types of ws characters.
 * we also use overloaded isspace with the global locale arg for safety.
 *
 * https://en.cppreference.com/w/cpp/locale/locale/locale
 *
 * origin source: https://stackoverflow.com/a/69036727
 */
void TrimRight( char *s )
{
    const std::locale loc{ };
    for (;; ++s) {
        // Loop for as long as the string has not ended and non-ws chars are found.
        for (; !std::isspace(*s, loc); ++s) {
            // Return if the end of the string is found. If the end of the string
            // is found here, it did not end with ws.
            if (*s == '\0') return;
        }

        // A ws position is found, store it.
        char *space_pos = s;

        // Loop for as long as there are ws.
        do ++s; while (std::isspace(*s, loc));

        // Return if the end of the string is found. If the end of the string
        // is found at this stage, it ended with the ws so trim the string.
        if (*s == '\0') {
            *space_pos = '\0';
            return;
        }
    }
}


class str_trim_right : public ::testing::Test
{
protected:
    /*
     * @brief trim string from the end using STL (in place).
     * origin source: https://stackoverflow.com/a/217605
     */
    void rtrim(std::string &s) const
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](char ch) {
                    return !std::isspace(ch, std::locale{});
                }).base(), s.end());
    }

    /**
     * copy non modifiable input string into two distinct modifiable strings
     * for passing into two different trim right functions to compare results later.
     *
     * while using USE_BOOST we assume that the boost::trim_right() is well tested and reliable.
     * => NOTE: our rtrim function should give the same result!
     *
     * https://google.github.io/googletest/reference/assertions.html#EXPECT_STREQ
     */
    void do_trim_right_test(std::string_view const& s) const
    {
        std::string lhs{ s };
        std::string rhs{ s };

#if USE_BOOST
        boost::trim_right(lhs);
#else
        rtrim(lhs);
#endif // USE_BOOST

        TrimRight(rhs.data());

        // verify that the two C-style strings have the same contents.
        ASSERT_STREQ(lhs.data(), rhs.data());
    }

};


TEST_F(str_trim_right, no_ws)
{
    do_trim_right_test("aaa...");
}

TEST_F(str_trim_right, no_ws_end)
{
    do_trim_right_test("aaa... also!");
}

TEST_F(str_trim_right, ws_leading)
{
    do_trim_right_test(" a 1?");
}

TEST_F(str_trim_right, ws_end_1)
{
    do_trim_right_test("a 1? ");
}

TEST_F(str_trim_right, ws_end_2)
{
    do_trim_right_test("a  2?  ");
}

TEST_F(str_trim_right, ws_end_3)
{
    do_trim_right_test("a   3?   ");
}

TEST_F(str_trim_right, ws_big_1)
{
    do_trim_right_test(
"aaaaaa BOOM? \t\n\t"
    );
}

TEST_F(str_trim_right, ws_big_2)
{
    do_trim_right_test(
"\t\t\t\t\t\t\t\t\t\t\t"
"aaaAAA \t\n\t BOOM!   "
"\t\t\t\t\t\t\t\t\t\t\t"
    );
}

TEST_F(str_trim_right, ws_big_3)
{
    do_trim_right_test(
"\t\t\t\t\t\t\t\t\t\t\t"
"Tars! Taaars! TAAAARS!"
"\t\t\t\t\t\t\t\t\t\t\t"
"aaaAAA \t\n\t BOOM! \t"
"\t\t\t\t\t\t\t\t\t\t\t"
    );
}

