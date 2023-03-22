#include "gen.hpp"

#include <gtest/gtest.h>

#include <cstddef>              // std::size_t
#include <cstdint>              // other types, *_MIN, etc
#include <stdexcept>            // std::invalid_argument
#include <string>
#include <unordered_set>

using namespace wndx;

class genTest : public ::testing::Test
{
protected:
    const std::size_t LOOPS { 25 };

    virtual void SetUp()
    {}

    template<typename T>
    void doRandomUsetOfType(const std::size_t n) const
    {
        std::unordered_set<T> ruset{ gen::random_uset<T>(n, static_cast<T>(0), static_cast<T>(n)) };
        ASSERT_EQ(ruset.size(), n);
    }

};

TEST_F(genTest, random_uset_types)
{
    for (std::size_t i = 0; i < LOOPS; i++) {
        doRandomUsetOfType<int>(i);
        doRandomUsetOfType<double>(i);
        doRandomUsetOfType<std::int_fast8_t>(i);
        doRandomUsetOfType<std::int_least16_t>(i);
    }
}

TEST_F(genTest, random_uset_insufficient_range)
{
    const std::size_t N {42};
    const int Ni { static_cast<int>(N) * -1 }; // N inverted
    const std::string exp_err{ "Insufficient range" };

    try {
        std::unordered_set ruset{ gen::random_uset<int>(N, 0, N - 1) };
    } catch(std::invalid_argument const &err) {
        ASSERT_EQ(err.what(), std::string(exp_err));
    } catch(...) {
        FAIL() << "Expected std::invalid_argument " + exp_err ;
    }

    ASSERT_ANY_THROW(gen::random_uset<int>(N, 0, N - 1)); // the same as above
    ASSERT_ANY_THROW(gen::random_uset<int>(N, Ni + 1, 0));
    // ASSERT_ANY_THROW(gen::random_uset<int>(N, Ni, -1)); // FIXME: hangs

    ASSERT_NO_THROW (gen::random_uset<int>(N, 0, N));
    ASSERT_NO_THROW (gen::random_uset<int>(N, Ni, 0));
    ASSERT_NO_THROW (gen::random_uset<int>(N, Ni, N));
}
