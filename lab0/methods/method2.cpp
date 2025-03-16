#include "methods.h"

/**
 * 循环展开的实现
 */
void method2(
  double *A,
  double *B,
  double *C
) {
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < N; ++j) {
      double sum = 0.0;

      // 展开最内层循环，每次处理 4 个元素
      int k = 0;
      for (; k <= K - 4; k += 4) {
        sum += A[i * K + k] * B[k * N + j];
        sum += A[i * K + k + 1] * B[(k + 1) * N + j];
        sum += A[i * K + k + 2] * B[(k + 2) * N + j];
        sum += A[i * K + k + 3] * B[(k + 3) * N + j];
      }

      // 处理剩余不足 4 个的元素
      for (; k < K; ++k) {
        sum += A[i * K + k] *B[k * N + j];
      }

      C[i * N + j] = sum;
    }
  }
}