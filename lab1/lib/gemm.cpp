#include "lib.h"

void gemm(
  const double *A,
  const double *B,
  double *C
) {
  for (int i = 0; i < local_M; ++i) {
    int k = 0;
    // 循环展开：每次处理4个k值
    for (; k <= K - 4; k += 4) {
      double a_ik0 = A[i * K + k];
      double a_ik1 = A[i * K + k + 1];
      double a_ik2 = A[i * K + k + 2];
      double a_ik3 = A[i * K + k + 3];

      int j = 0;
      // 循环展开：每次处理4个j值
      for (; j <= N - 4; j += 4) {
        C[i * N + j]     += a_ik0 * B[k * N + j]     + a_ik1 * B[(k + 1) * N + j]     + a_ik2 * B[(k + 2) * N + j]     + a_ik3 * B[(k + 3) * N + j];
        C[i * N + j + 1] += a_ik0 * B[k * N + j + 1] + a_ik1 * B[(k + 1) * N + j + 1] + a_ik2 * B[(k + 2) * N + j + 1] + a_ik3 * B[(k + 3) * N + j + 1];
        C[i * N + j + 2] += a_ik0 * B[k * N + j + 2] + a_ik1 * B[(k + 1) * N + j + 2] + a_ik2 * B[(k + 2) * N + j + 2] + a_ik3 * B[(k + 3) * N + j + 2];
        C[i * N + j + 3] += a_ik0 * B[k * N + j + 3] + a_ik1 * B[(k + 1) * N + j + 3] + a_ik2 * B[(k + 2) * N + j + 3] + a_ik3 * B[(k + 3) * N + j + 3];
      }
      // 处理剩余的j值（尾循环）
      for (; j < N; j++) {
        C[i * N + j] += a_ik0 * B[k * N + j] + a_ik1 * B[(k + 1) * N + j] + a_ik2 * B[(k + 2) * N + j] + a_ik3 * B[(k + 3) * N + j];
      }
    }
    // 处理剩余的k值（尾循环）
    for (; k < K; k++) {
      double a_ik = A[i * K + k];
      int j = 0;
      // 循环展开：每次处理4个j值
      for (; j <= N - 4; j += 4) {
        C[i * N + j]     += a_ik * B[k * N + j];
        C[i * N + j + 1] += a_ik * B[k * N + j + 1];
        C[i * N + j + 2] += a_ik * B[k * N + j + 2];
        C[i * N + j + 3] += a_ik * B[k * N + j + 3];
      }
      // 处理剩余的j值（尾循环）
      for (; j < N; j++) {
        C[i * N + j] += a_ik * B[k * N + j];
      }
    }
  }
}