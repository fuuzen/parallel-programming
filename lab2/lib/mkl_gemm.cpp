#include <mkl.h>
#include "lib.h"

void mkl_gemm(
  const double *A,
  const double *B,
  double *C,
  MatrixParams *params
) {
  double alpha = 1.0f, beta = 0.0f;
  cblas_dgemm(
    CblasRowMajor, // 行优先存储
    CblasNoTrans,  // 不转置 A
    CblasNoTrans,  // 不转置 B
    params->local_m,             // A 的行数
    params->n,             // B 的列数
    params->k,             // A 的列数 / B 的行数
    alpha,         // 缩放因子 alpha
    A,             // 矩阵 A
    params->k,             // A 的列数
    B,             // 矩阵 B
    params->n,             // B 的列数
    beta,          // 缩放因子 beta
    C,             // 结果矩阵 C
    params->n              // C 的列数
  );
}
