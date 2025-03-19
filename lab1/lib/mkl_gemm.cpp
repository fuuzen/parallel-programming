#include <mkl.h>
#include "lib.h"

/**
 * MPI 结合 Intel MKL 的实现
 */
void mkl_gemm(
  const double *A,
  const double *B,
  double *C
) {
  double alpha = 1.0f, beta = 0.0f;
  cblas_dgemm(
    CblasRowMajor, // 行优先存储
    CblasNoTrans,  // 不转置 A
    CblasNoTrans,  // 不转置 B
    N,             // A 的行数
    N,             // B 的列数
    N,             // A 的列数 / B 的行数
    alpha,         // 缩放因子 alpha
    A,             // 矩阵 A
    N ,             // A 的 leading dimension
    B,             // 矩阵 B
    N,             // B 的 leading dimension
    beta,          // 缩放因子 beta
    C,             // 结果矩阵 C
    N              // C 的 leading dimension
  );
}
