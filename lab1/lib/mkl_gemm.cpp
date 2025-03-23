#include <mkl.h>
#include "lib.h"

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
    local_M,             // A 的行数
    N,             // B 的列数
    K,             // A 的列数 / B 的行数
    alpha,         // 缩放因子 alpha
    A,             // 矩阵 A
    K,             // A 的列数
    B,             // 矩阵 B
    N,             // B 的列数
    beta,          // 缩放因子 beta
    C,             // 结果矩阵 C
    N              // C 的列数
  );
}
