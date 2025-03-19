#include "../main.h"

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

/**
 * @brief Intel MKL 的单线程 GEMM 实现
 * @param A 左乘矩阵，M x K
 * @param B 右乘矩阵，K x N
 * @param C 返回矩阵，M x N
 * @param local_M 本线程矩阵 A 的行数 M
 */
void mkl_gemm (
  const double *A,
  const double *B,
  double *C
);

/**
 * 调整循环顺序并且循环展开的单线程 GEMM 实现
 * @param A 左乘矩阵，M x K
 * @param B 右乘矩阵，K x N
 * @param C 返回矩阵，M x N
 * @param local_M 本线程矩阵 A 的行数 M
 */
 void gemm (
  const double *A,
  const double *B,
  double *C
);