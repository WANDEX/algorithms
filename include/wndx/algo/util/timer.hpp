#pragma once

#include <chrono>

namespace wndx {

class Timer
{
private:
    // Type aliases to make accessing nested type easier
    using clock_type = std::chrono::steady_clock;
    using second_type = std::chrono::duration<double, std::ratio<1>>;

    std::chrono::time_point<clock_type> m_beg { clock_type::now() };

public:
    inline void reset()
    {
        m_beg = clock_type::now();
    }

    inline double elapsed() const
    {
        return std::chrono::duration_cast<second_type>(clock_type::now() - m_beg).count();
    }
};

} // namespace wndx
