#pragma once

#include <condition_variable>
#include <mutex>

namespace elelel {
  struct cv_lock {
    // Construct the lock with an external mutex and condition_variable
    cv_lock(std::mutex& mutex,
            std::condition_variable& cv,
            bool& locked);
    // Copy-construct the lock while retaining the shared mutex and condition_variable
    cv_lock(const cv_lock& other);

    // Wait till unlock and then lock
    void wait_and_lock();
    // Unlock
    void unlock();
    // Wait and lock, execute void function f and unlock. Unlocks even if an exception is thrown
    template <typename F>
    void with_lock(F f);
    // Wait and lock, execute function f returning Result and unlock.
    // Return the result afterwards, Result has to be default-constructible.
    // Unlocks even if an exception is thrown
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
