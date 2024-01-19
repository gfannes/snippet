#ifndef HEADER_util_log_hpp_ALREAD_INCLUDED
#define HEADER_util_log_hpp_ALREAD_INCLUDED

#ifndef UTIL_LOG
#error Expected UTIL_LOG to be defined
#endif

#include <iostream>
#include <sstream>
#include <thread>

#if UTIL_LOG

thread_local static std::ostringstream l_oss;
#define L(...)                                                                 \
  do {                                                                         \
    l_oss.str("");                                                             \
    l_oss << std::this_thread::get_id() << ": " << __VA_ARGS__ << std::endl;   \
    std::cout << l_oss.str();                                                  \
  } while (0)

#else

#define L(...)                                                                 \
  do {                                                                         \
  } while (0)

#endif

#endif
