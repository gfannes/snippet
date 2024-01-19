#ifndef HEADER_thread_pool_common_hpp_ALREAD_INCLUDED
#define HEADER_thread_pool_common_hpp_ALREAD_INCLUDED

#include <cmath>
#include <functional>
#include <mutex>
#include <vector>

using Mutex = std::mutex;
using Lock = std::unique_lock<Mutex>;

using Job = std::function<void()>;
using Jobs = std::vector<Job>;

using Size = std::size_t;

inline void burn(std::size_t difficulty)
{
    volatile float sum = 0;
    for (auto i = 0; i < difficulty; ++i)
        sum += std::sin(i);
}

#endif
