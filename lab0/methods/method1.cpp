#include "methods.h"

/**
 * 调整循环顺序的实现
 */
void method1(
  double *A,
  double *B,
  double *C
) {
  for(int i = 0; i < M; i++) {
    for(int k = 0; k < K; k++) {
      // 将 A[i][k] 存储在寄存器中，减少内存访问
      double a_ik = A[i * K + k];
      for(int j = 0; j < N; j++) {
        C[i * N + j] += a_ik * B[k * N + j];
      }
    }
  }
}