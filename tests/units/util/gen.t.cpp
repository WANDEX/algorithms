#include "wndx/algo/util/gen.hpp"

#include <gtest/gtest.h>

#include <cstddef>              // std::size_t
#include <cstdint>              // other types, *_MIN, etc
#include <stdexcept>            // std::invalid_argument
#include <string>
#include <unordered_set>

using namespace wndx::algo;

class genTest : public ::testing::Test
{
protected:
    static const std::size_t LOOPS{ 5 };
    const std::string range_err{ "Insufficient range" };

    void SetUp()
    {}

    template<typename T>
    void make_rndm_uset(const std::size_t n, const T fr, const T to) const
    {
        // std::unordered_set<T> ruset{ gen::random_uset<T>(n, static_cast<T>(0), static_cast<T>(n)) };
        std::unordered_set<T> ruset{ gen::random_uset<T>(n, fr, to) };
        ASSERT_EQ(ruset.size(), n);
    }

    template<typename T>
    void make_rndm_uset_test() const
    {
        for (std::size_t n = 0; n < LOOPS; n++) {
            // make_rndm_uset<T>(n, 0, n);
            make_rndm_uset<T>(n, static_cast<T>(0), static_cast<T>(n));
        }
    }

    template<typename T>
    void make_rndm_uset_test_floating(const T fr, const T to) const
    {
        for (std::size_t n = 0; n < LOOPS; n++) {
            make_rndm_uset<T>(n, fr, to);
        }
    }

    void try_catch_rndm_uset(const std::size_t n, const int fr, const int to) const
    {
        try {
            std::unordered_set ruset{ gen::random_uset<int>(n, fr, to) };
        } catch(std::invalid_argument const &err) {
            ASSERT_EQ(err.what(), range_err) << err.what();
            throw err.what(); // re-throw for gtest ASSERT_*_THROW, FAIL() throws!
        } catch(...) {
            FAIL() << "Expected std::invalid_argument " + range_err;
        }
    }
};

TEST_F(genTest, random_uset_signed)
{
    make_rndm_uset_test<std::int8_t>();
    make_rndm_uset_test<std::int16_t>();
    make_rndm_uset_test<std::int32_t>();
    make_rndm_uset_test<std::int64_t>();
    make_rndm_uset_test<int>();
}
#if 1
TEST_F(genTest, random_uset_unsigned)
{
    make_rndm_uset_test<std::uint8_t>();
    make_rndm_uset_test<std::uint16_t>();
    make_rndm_uset_test<std::uint32_t>(); // FIXME
    make_rndm_uset_test<std::uint64_t>(); // FIXME
    make_rndm_uset_test<std::size_t>(); // FIXME
}
#endif

TEST_F(genTest, random_uset_floating)
{
    make_rndm_uset_test_floating<float>(0.f, 1.f);
    make_rndm_uset_test_floating<double>(-1., .1);
}

TEST_F(genTest, random_uset_insufficient_range)
{
    constexpr std::size_t N{ 42 };
    constexpr int Ni{ static_cast<int>(N) * -1 }; // N -> (int) -N
    ASSERT_NO_THROW (try_catch_rndm_uset(N,  0, N));
    ASSERT_NO_THROW (try_catch_rndm_uset(N, Ni, 0));
    ASSERT_NO_THROW (try_catch_rndm_uset(N, Ni, N));
    ASSERT_ANY_THROW(try_catch_rndm_uset(N,  0, N-1));
    ASSERT_ANY_THROW(try_catch_rndm_uset(N, Ni+1, 0));
    ASSERT_ANY_THROW(try_catch_rndm_uset(N, Ni,  -1));
}