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
