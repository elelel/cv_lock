#pragma once

#include <condition_variable>
#include <mutex>

namespace ares {
  struct cv_lock {
    cv_lock(std::mutex& mutex,
            std::condition_variable& cv,
            bool& locked);
    cv_lock(const cv_lock& other);
    
    void wait_and_lock();
    void unlock();
    template <typename F>
    void with_lock(F f);
    template <typename Result, typename F>
    Result with_lock_as(F f);
  private:
    std::mutex& mutex_;
    std::condition_variable& cv_;

    std::unique_lock<std::mutex> lock_;
    bool locked_;

    std::string name_;
  };
}
