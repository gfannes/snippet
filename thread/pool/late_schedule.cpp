#define UTIL_LOG 0
#include <util/log.hpp>
#include <util/Stopwatch.hpp>

#include <thread/pool/common.hpp>
#include <thread/pool/Scheduler.hpp>

class Pool
{
public:
    Pool(Size thread_count, Scheduler &scheduler)
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
        const Size threshold = (threads_.size() == 1 ? 0 : 1 * threads_.size());

        while (!quit_)
        {
            L("loop");

            Job job;
            bool do_schedule = false;

            {
                Lock lock{jobs_m_};

                if (jobs_.size() > threshold)
                {
                    job = jobs_.back();
                    jobs_.pop_back();
                }
                else
                {
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
                            {
                                quit_ = true;
                            }
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

int main()
{
    const Size job_count = 32;
    const Size schedule_count = 3000;
    const Size difficulty = 1000;
    const Size schedule_cost = 1;

    const double duration_ref = [&]() {
        // return 1.15;
        for (auto i = 0; i < 3; ++i)
        {
            Stopwatch sw;
            const Size count = job_count * schedule_count;
            for (auto i = 0; i < count; ++i)
                burn(difficulty);
            if (i == 2)
                return sw.elapse();
        }
        return 0.0;
    }();
    std::cout << duration_ref << std::endl;

    for (Size thread_count : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 100})
    {
        Stopwatch sw;
        {
            Scheduler scheduler{job_count, schedule_count, difficulty, schedule_cost};
            Pool pool{thread_count, scheduler};
        }
        const auto duration_act = sw.elapse();

        const auto duration_act_cmp = duration_act * thread_count;
        const auto efficiency = 100 * duration_ref / duration_act_cmp;

        const auto speedup = duration_ref / duration_act;
        std::cout << thread_count << " " << efficiency << "% " << speedup << " "
                  << duration_act << std::endl;
    }

    return 0;
}
