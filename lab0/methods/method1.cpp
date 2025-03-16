#include "methods.h"

/**
 * 调整循环顺序的实现
 */
void method1(
  double *A,
  double *B,
  double *C
) {
  for(int i = 0; i < N; i++) {
    for(int l = 0; l < N; l++) {
      // 将 A[i][l] 存储在寄存器中，减少内存访问
      double a_il = A[i * K + l];
      for(int j = 0; j < N; j++) {
        C[i * N + j] += a_il * B[l * N + j];
      }
    }
  }
}