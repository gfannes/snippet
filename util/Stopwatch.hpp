#ifndef HEADER_util_Stopwatch_hpp_ALREAD_INCLUDED
#define HEADER_util_Stopwatch_hpp_ALREAD_INCLUDED

#include <chrono>

class Stopwatch
{
public:
    Stopwatch() { start(); }

    void start() { start_ = Clock::now(); }

    double elapse() const
    {
        return std::chrono::duration_cast<std::chrono::duration<double>>(
                   Clock::now() - start_)
            .count();
    }

private:
    using Clock = std::chrono::steady_clock;
    Clock::time_point start_;
};

#endif
