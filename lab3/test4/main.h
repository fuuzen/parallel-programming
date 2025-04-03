#include "../util/util.h"

// 线程参数结构体
typedef struct {
  double *A;
  int start;
  int end;
  double sum;
} SumArgs;

/**
 * @brief pthread 线程执行数组求和
 */
void* sum(void *arg);

/**
 * @brief 执行数组求和
 */
void sum(SumArgs *args);

/**
 * @brief 用于存储总和和互斥锁
 */
typedef struct {
  double total;
  pthread_mutex_t lock;
} GlobalSum;

extern GlobalSum global_sum;