#include "../util/util.h"

// 线程参数结构体
typedef struct {
  double *A;
  double *B;
  double *C;
  int start_row;
  int end_row;
  int N;
  int K;
} GemmArgs;

/**
 * @brief pthread 线程执行调整循环顺序并且循环展开的单线程 GEMM 实现
 */
void* gemm(void *arg);

/**
 * @brief 调整循环顺序并且循环展开的单线程 GEMM 实现
 */
void gemm(GemmArgs *args);
