#include "util.h"

void bind_thread_to_cpu(pthread_t thread, int cpu_id) {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);          // 清空 CPU 集合
  CPU_SET(cpu_id, &cpuset);   // 绑定到指定 CPU

  // 设置线程的 CPU 亲和性
  int rc = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
  if (rc != 0) {
    std::perror("pthread_setaffinity_np failed");
  }
}