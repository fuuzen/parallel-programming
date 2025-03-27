#include "lib.h"

void gemm(
  const double *A,
  const double *B,
  double *C,
  MatrixParams *params
) {
  for (int i = 0; i < params->local_m; ++i) {
    int k = 0;
    // 循环展开：每次处理4个k值
    for (; k <= params->k - 4; k += 4) {
      double a_ik0 = A[i * params->k + k];
      double a_ik1 = A[i * params->k + k + 1];
      double a_ik2 = A[i * params->k + k + 2];
      double a_ik3 = A[i * params->k + k + 3];

      int j = 0;
      // 循环展开：每次处理4个j值
      for (; j <= params->n - 4; j += 4) {
        C[i * params->n + j]     += a_ik0 * B[k * params->n + j]     + a_ik1 * B[(k + 1) * params->n + j]     + a_ik2 * B[(k + 2) * params->n + j]     + a_ik3 * B[(k + 3) * params->n + j];
        C[i * params->n + j + 1] += a_ik0 * B[k * params->n + j + 1] + a_ik1 * B[(k + 1) * params->n + j + 1] + a_ik2 * B[(k + 2) * params->n + j + 1] + a_ik3 * B[(k + 3) * params->n + j + 1];
        C[i * params->n + j + 2] += a_ik0 * B[k * params->n + j + 2] + a_ik1 * B[(k + 1) * params->n + j + 2] + a_ik2 * B[(k + 2) * params->n + j + 2] + a_ik3 * B[(k + 3) * params->n + j + 2];
        C[i * params->n + j + 3] += a_ik0 * B[k * params->n + j + 3] + a_ik1 * B[(k + 1) * params->n + j + 3] + a_ik2 * B[(k + 2) * params->n + j + 3] + a_ik3 * B[(k + 3) * params->n + j + 3];
      }
      // 处理剩余的j值（尾循环）
      for (; j < params->n; j++) {
        C[i * params->n + j] += a_ik0 * B[k * params->n + j] + a_ik1 * B[(k + 1) * params->n + j] + a_ik2 * B[(k + 2) * params->n + j] + a_ik3 * B[(k + 3) * params->n + j];
      }
    }
    // 处理剩余的k值（尾循环）
    for (; k < params->k; k++) {
      double a_ik = A[i * params->k + k];
      int j = 0;
      // 循环展开：每次处理4个j值
      for (; j <= params->n - 4; j += 4) {
        C[i * params->n + j]     += a_ik * B[k * params->n + j];
        C[i * params->n + j + 1] += a_ik * B[k * params->n + j + 1];
        C[i * params->n + j + 2] += a_ik * B[k * params->n + j + 2];
        C[i * params->n + j + 3] += a_ik * B[k * params->n + j + 3];
      }
      // 处理剩余的j值（尾循环）
      for (; j < params->n; j++) {
        C[i * params->n + j] += a_ik * B[k * params->n + j];
      }
    }
  }
}

void gemm_cannon(
  const double *A,
  const double *B,
  double *C,
  MatrixParams *params
) {
  for (int i = 0; i < params->block_m; ++i) {
    int k = 0;
    // 循环展开：每次处理4个k值
    for (; k <= params->block_k - 4; k += 4) {
      double a_ik0 = A[i * params->block_k + k];
      double a_ik1 = A[i * params->block_k + k + 1];
      double a_ik2 = A[i * params->block_k + k + 2];
      double a_ik3 = A[i * params->block_k + k + 3];

      int j = 0;
      // 循环展开：每次处理4个j值
      for (; j <= params->block_n - 4; j += 4) {
        C[i * params->block_n + j]     += a_ik0 * B[k * params->block_n + j]     + a_ik1 * B[(k + 1) * params->block_n + j]     + a_ik2 * B[(k + 2) * params->block_n + j]     + a_ik3 * B[(k + 3) * params->block_n + j];
        C[i * params->block_n + j + 1] += a_ik0 * B[k * params->block_n + j + 1] + a_ik1 * B[(k + 1) * params->block_n + j + 1] + a_ik2 * B[(k + 2) * params->block_n + j + 1] + a_ik3 * B[(k + 3) * params->block_n + j + 1];
        C[i * params->block_n + j + 2] += a_ik0 * B[k * params->block_n + j + 2] + a_ik1 * B[(k + 1) * params->block_n + j + 2] + a_ik2 * B[(k + 2) * params->block_n + j + 2] + a_ik3 * B[(k + 3) * params->block_n + j + 2];
        C[i * params->block_n + j + 3] += a_ik0 * B[k * params->block_n + j + 3] + a_ik1 * B[(k + 1) * params->block_n + j + 3] + a_ik2 * B[(k + 2) * params->block_n + j + 3] + a_ik3 * B[(k + 3) * params->block_n + j + 3];
      }
      // 处理剩余的j值（尾循环）
      for (; j < params->block_n; j++) {
        C[i * params->block_n + j] += a_ik0 * B[k * params->block_n + j] + a_ik1 * B[(k + 1) * params->block_n + j] + a_ik2 * B[(k + 2) * params->block_n + j] + a_ik3 * B[(k + 3) * params->block_n + j];
      }
    }
    // 处理剩余的k值（尾循环）
    for (; k < params->block_k; k++) {
      double a_ik = A[i * params->block_k + k];
      int j = 0;
      // 循环展开：每次处理4个j值
      for (; j <= params->block_n - 4; j += 4) {
        C[i * params->block_n + j]     += a_ik * B[k * params->block_n + j];
        C[i * params->block_n + j + 1] += a_ik * B[k * params->block_n + j + 1];
        C[i * params->block_n + j + 2] += a_ik * B[k * params->block_n + j + 2];
        C[i * params->block_n + j + 3] += a_ik * B[k * params->block_n + j + 3];
      }
      // 处理剩余的j值（尾循环）
      for (; j < params->block_n; j++) {
        C[i * params->block_n + j] += a_ik * B[k * params->block_n + j];
      }
    }
  }
}