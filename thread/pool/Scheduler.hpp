#ifndef HEADER_thread_pool_Scheduler_hpp_ALREAD_INCLUDED
#define HEADER_thread_pool_Scheduler_hpp_ALREAD_INCLUDED

#include <thread/pool/common.hpp>

class Scheduler {
public:
  Scheduler(Size job_count, Size schedule_count, Size difficulty,
            Size schedule_cost)
      : job_count_(job_count), schedule_count_(schedule_count),
        difficulty_(difficulty), schedule_cost_(schedule_cost) {}

  bool done() const { return schedule_count_ == 0; }

  void operator()(Jobs &jobs) {
    if (!done()) {
      L("Scheduler.call() " << schedule_count_);
      burn(schedule_cost_ * difficulty_);

      for (auto i = 0; i < job_count_; ++i)
        jobs.emplace_back([&]() { burn(difficulty_); });

      --schedule_count_;
    }
  }

private:
  const Size job_count_;
  const Size difficulty_;
  const Size schedule_cost_;
  Size schedule_count_;
};

#endif
