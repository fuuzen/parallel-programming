#include <pthread.h>
#include <iostream>
#include <chrono>
#include <iomanip>

/**
 * @brief 将线程绑定到指定 CPU 核上
 * @param thread 线程
 * @param cpu_id CPU 核 ID
 */
void bind_thread_to_cpu(pthread_t thread, int cpu_id);

/**
 * @brief 计时器，创建时开始计时，销毁时结束计时并输出时间。
 */
class Timer {
private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
  std::chrono::time_point<std::chrono::high_resolution_clock> end_time;

public:
  Timer() {
    start_time = std::chrono::high_resolution_clock::now();
  }

  ~Timer() {
    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << std::scientific << std::setprecision(5) << elapsed.count() << std::endl;
  }
};
