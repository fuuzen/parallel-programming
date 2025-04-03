#include "../util/util.h"

using namespace std;

// 线程参数结构体
typedef struct {
  double *A;
  double *B;
  double *C;
  int start_row;
  int end_row;
  int start_col;
  int end_col;
  int K;
  int N;
} GemmArgs;

/**
 * @brief pthread 线程执行调整循环顺序并且循环展开的单线程 GEMM 实现
 */
void* gemm(void *arg);

/**
 * @brief 调整循环顺序并且循环展开的单线程 GEMM 实现
 */
void gemm(GemmArgs *args);

/**
 * @brief 获取分块分配的块数
 */
pair<int, int> get_block_distribution(int num_threads, int M, int N);