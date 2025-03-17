#include "methods.h"

/**
 * 最 naive 的实现方式
 */
void method0(
  double *A,
  double *B,
  double *C
) {
  for(int i = 0; i < M; i++) {
    for(int j = 0; j < N; j++) {
      for(int k = 0; k < K; k++) {
        C[i * N + j] += A[i * K + k] * B[k * N + j];
      }
    }
  }
}