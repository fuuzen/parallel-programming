#include <pthread.h>

/**
 * @brief 将线程绑定到指定 CPU 核上
 * @param thread 线程
 * @param cpu_id CPU 核 ID
 */
void bind_thread_to_cpu(pthread_t thread, int cpu_id);
