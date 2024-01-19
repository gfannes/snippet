#define UTIL_LOG 1
#include <util/log.hpp>

#include <algorithm>
#include <bit>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <vector>

using Mutex = std::mutex;
using Lock = std::unique_lock<Mutex>;

using Job = std::function<void()>;
using Jobs = std::vector<Job>;

void burn(std::size_t difficulty)
{
    volatile float sum = 0;
    for (auto i = 0; i < difficulty; ++i)
        sum += std::sin(i);
}

class Scheduler
{
public:
    Scheduler(std::size_t job_count, std::size_t schedule_count, std::size_t difficulty, std::size_t schedule_cost)
        : job_count_(job_count), schedule_count_(schedule_count), difficulty_(difficulty), schedule_cost_(schedule_cost) {}

    bool done() const { return schedule_count_ == 0; }

    void operator()(Jobs &jobs)
    {
        if (!done())
        {
            burn(schedule_cost_ * difficulty_);

            for (auto i = 0; i < job_count_; ++i)
                jobs.emplace_back([&]() { burn(difficulty_); });

            --schedule_count_;
        }
    }

private:
    const std::size_t job_count_;
    const std::size_t difficulty_;
    const std::size_t schedule_cost_;
    std::size_t schedule_count_;
};

class Pool
{
public:
    Pool(std::size_t thread_count, Scheduler &scheduler)
        : scheduler_(scheduler)
    {
        for (auto i = 0; i < thread_count; ++i)
            threads_.emplace_back(std::ref(*this));
    }

    ~Pool()
    {
        for (auto &thread : threads_)
            thread.join();
    }

    void operator()()
    {
        const std::size_t threshold = (threads_.size() == 1 ? 0 : 3 * threads_.size());

        while (!quit_)
        {
            L("loop");

            Job job;
            bool do_schedule = false;

            {
                Lock lock{jobs_m_};

                if (stage_m_.try_lock())
                {
                    if (!stage_.empty())
                    {
                        L("Found " << stage_.size() << " jobs");
                        jobs_.insert(jobs_.end(), stage_.begin(), stage_.end());
                        stage_.resize(0);
                    }

                    if (scheduler_.done())
                    {
                        stage_m_.unlock();

                        if (!jobs_.empty())
                        {
                            job = jobs_.back();
                            jobs_.pop_back();
                        }
                        else
                            quit_ = true;
                    }
                    else if (jobs_.size() > threshold)
                    {
                        stage_m_.unlock();

                        job = jobs_.back();
                        jobs_.pop_back();
                    }
                    else
                    {
                        do_schedule = true;
                    }
                }
                else if (!jobs_.empty())
                {
                    job = jobs_.back();
                    jobs_.pop_back();
                }
            }

            if (job)
            {
                L("compute");
                job();
            }
            else if (do_schedule)
            {
                L("SCHEDULE");
                scheduler_(stage_);
                stage_m_.unlock();
            }
        }

        L("QUIT");
    }

private:
    Scheduler &scheduler_;

    Jobs jobs_;
    Mutex jobs_m_;

    Jobs stage_;
    Mutex stage_m_;

    volatile bool quit_ = false;

    std::vector<std::thread> threads_;
};

class Duration
{
public:
    Duration() { start(); }

    void start() { start_ = Clock::now(); }

    double stop() const
    {
        return std::chrono::duration_cast<std::chrono::duration<double>>(Clock::now() - start_).count();
    }

private:
    using Clock = std::chrono::steady_clock;
    Clock::time_point start_;
};

int main()
{
    const std::size_t job_count = 32;
    const std::size_t schedule_count = 3000;
    const std::size_t difficulty = 1000;
    const std::size_t schedule_cost = 1;

    const double duration_ref = [&]() {
        return 1.15;
        for (auto i = 0; i < 3; ++i)
        {
            Duration d;
            const std::size_t count = job_count * schedule_count;
            for (auto i = 0; i < count; ++i)
                burn(difficulty);
            if (i == 2)
                return d.stop();
        }
        return 0.0;
    }();
    std::cout << duration_ref << std::endl;

    for (std::size_t thread_count : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 100})
    {
        Duration duration;
        {
            Scheduler scheduler{job_count, schedule_count, difficulty, schedule_cost};
            Pool pool{thread_count, scheduler};
        }
        const auto duration_act = duration.stop();

        const auto duration_act_cmp = duration_act * thread_count;
        const auto efficiency = 100 * duration_ref / duration_act_cmp;

        const auto speedup = duration_ref / duration_act;
        std::cout << thread_count << " " << efficiency << "% " << speedup << " " << duration_act << std::endl;
    }

    return 0;
}
