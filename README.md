# Simple and copyable condition_variable-based lock



```c++
#include <elelel/cv_lock>

// .....
  using namespace std::chrono_literals;
  std::mutex mutex;
  std::condition_variable cv;
  bool locked{false};
  elelel::cv_lock lock(mutex, cv, locked);
  
  int var = 2;
  std::atomic<bool> terminated{false};
  auto mutate_var = [&var, &lock, &terminated] () {
    auto local_lock = elelel::cv_lock(lock);
    while (!terminated) {
      try {
        local_lock.wait_and_lock();
        var -= 1;
        std::this_thread::sleep_for(30ms);
        var += var;
        local_lock.unlock();
      } catch (...) {
        local_lock.unlock();
        throw;
      }
    }
  };

  std::thread terminate_thread([&terminated] () {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(10s);
      terminated = true;
    });

  std::vector<std::thread> worker_threads;
  for (auto i = 0; i < 8; ++i) {
    worker_threads.push_back(std::thread(mutate_var));
  }
  terminate_thread.join();
  for (auto& t : worker_threads) t.join();
  
  REQUIRE(var == 2);

```
