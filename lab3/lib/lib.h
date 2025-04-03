#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include <pthread.h>

using namespace std;

/**
 * @brief 输入一个合法的整数
 * @param min 最小值
 * @param max 最大值
 * @return int
 */
int input (int min, int max);

/**
 * @brief 输入一个合法的整数，必须整除一个给定的 mod
 * @param min 最小值
 * @param max 最大值
 * @param mod mod
 * @return int
 */
int input (int min, int max, int mod);

// 线程参数结构体
typedef struct {
  double *A;
  double *B;
  double *C;
  int start_row;
  int end_row;
  int N;
  int K;
} ThreadArgs;

/**
 * @brief pthread 线程执行调整循环顺序并且循环展开的单线程 GEMM 实现
 */
void* gemm_thread(void *arg);

/**
 * @brief 调整循环顺序并且循环展开的单线程 GEMM 实现
 */
void gemm_thread(ThreadArgs *arg);
