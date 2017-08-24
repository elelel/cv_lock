#pragma once

#include "../cv_lock.hpp"

inline ares::cv_lock::cv_lock(std::mutex& mutex,
                              std::condition_variable& cv,
                              bool& locked) :
  mutex_(mutex),
  cv_(cv),
  lock_(std::move(std::unique_lock<std::mutex>(mutex_, std::defer_lock))),
  locked_(locked) {
  if (locked_) {
    lock_.lock();
  }
}

// Copy constructor that shares mutex/cv with copies.
inline ares::cv_lock::cv_lock(const cv_lock& other) :
  mutex_(other.mutex_),
  cv_(other.cv_),
  lock_(std::move(std::unique_lock<std::mutex>(mutex_, std::defer_lock))),
  locked_(other.locked_) {
  if (locked_) {
    lock_.lock();
  }
}  

inline void ares::cv_lock::wait_and_lock() {
  cv_.wait(lock_, [this] { return !locked_; });
  lock_.lock();
  locked_ = true;
}

inline void ares::cv_lock::unlock() {
  locked_ = false;
  lock_.unlock();
  cv_.notify_one();
}

template <typename F>
inline void ares::cv_lock::with_lock(F f) {
  wait_and_lock();
  try {
    f();
  } catch (...) {
    unlock();
    throw;
  }
  unlock();
}

template <typename Result, typename F>
inline Result ares::cv_lock::with_lock_as(F f) {
  Result rslt;
  wait_and_lock();
  try {
    rslt = f();
  } catch (...) {
    unlock();
    throw;
  }
  unlock();
  return rslt;
}
