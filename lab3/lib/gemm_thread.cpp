#include "lib.h"

void* gemm_thread(void *arg) {
  ThreadArgs *args = (ThreadArgs*)arg;
  for (int i = args->start_row; i < args->end_row; ++i) {
    int k = 0;
    // 循环展开：每次处理4个k值
    for (; k <= args->K - 4; k += 4) {
      double a_ik0 = args->A[i * args->K + k];
      double a_ik1 = args->A[i * args->K + k + 1];
      double a_ik2 = args->A[i * args->K + k + 2];
      double a_ik3 = args->A[i * args->K + k + 3];

      int j = 0;
      // 循环展开：每次处理4个j值
      for (; j <= args->N - 4; j += 4) {
        args->C[i * args->N + j]     += a_ik0 * args->B[k * args->N + j]     + a_ik1 * args->B[(k + 1) * args->N + j]     + a_ik2 * args->B[(k + 2) * args->N + j]     + a_ik3 * args->B[(k + 3) * args->N + j];
        args->C[i * args->N + j + 1] += a_ik0 * args->B[k * args->N + j + 1] + a_ik1 * args->B[(k + 1) * args->N + j + 1] + a_ik2 * args->B[(k + 2) * args->N + j + 1] + a_ik3 * args->B[(k + 3) * args->N + j + 1];
        args->C[i * args->N + j + 2] += a_ik0 * args->B[k * args->N + j + 2] + a_ik1 * args->B[(k + 1) * args->N + j + 2] + a_ik2 * args->B[(k + 2) * args->N + j + 2] + a_ik3 * args->B[(k + 3) * args->N + j + 2];
        args->C[i * args->N + j + 3] += a_ik0 * args->B[k * args->N + j + 3] + a_ik1 * args->B[(k + 1) * args->N + j + 3] + a_ik2 * args->B[(k + 2) * args->N + j + 3] + a_ik3 * args->B[(k + 3) * args->N + j + 3];
      }
      // 处理剩余的j值（尾循环）
      for (; j < args->N; j++) {
        args->C[i * args->N + j] += a_ik0 * args->B[k * args->N + j] + a_ik1 * args->B[(k + 1) * args->N + j] + a_ik2 * args->B[(k + 2) * args->N + j] + a_ik3 * args->B[(k + 3) * args->N + j];
      }
    }
    // 处理剩余的k值（尾循环）
    for (; k < args->K; k++) {
      double a_ik = args->A[i * args->K + k];
      int j = 0;
      // 循环展开：每次处理4个j值
      for (; j <= args->N - 4; j += 4) {
        args->C[i * args->N + j]     += a_ik * args->B[k * args->N + j];
        args->C[i * args->N + j + 1] += a_ik * args->B[k * args->N + j + 1];
        args->C[i * args->N + j + 2] += a_ik * args->B[k * args->N + j + 2];
        args->C[i * args->N + j + 3] += a_ik * args->B[k * args->N + j + 3];
      }
      // 处理剩余的j值（尾循环）
      for (; j < args->N; j++) {
        args->C[i * args->N + j] += a_ik * args->B[k * args->N + j];
      }
    }
  }
  pthread_exit(NULL);
}


void gemm_thread(ThreadArgs *args) {
  for (int i = args->start_row; i < args->end_row; ++i) {
    int k = 0;
    // 循环展开：每次处理4个k值
    for (; k <= args->K - 4; k += 4) {
      double a_ik0 = args->A[i * args->K + k];
      double a_ik1 = args->A[i * args->K + k + 1];
      double a_ik2 = args->A[i * args->K + k + 2];
      double a_ik3 = args->A[i * args->K + k + 3];

      int j = 0;
      // 循环展开：每次处理4个j值
      for (; j <= args->N - 4; j += 4) {
        args->C[i * args->N + j]     += a_ik0 * args->B[k * args->N + j]     + a_ik1 * args->B[(k + 1) * args->N + j]     + a_ik2 * args->B[(k + 2) * args->N + j]     + a_ik3 * args->B[(k + 3) * args->N + j];
        args->C[i * args->N + j + 1] += a_ik0 * args->B[k * args->N + j + 1] + a_ik1 * args->B[(k + 1) * args->N + j + 1] + a_ik2 * args->B[(k + 2) * args->N + j + 1] + a_ik3 * args->B[(k + 3) * args->N + j + 1];
        args->C[i * args->N + j + 2] += a_ik0 * args->B[k * args->N + j + 2] + a_ik1 * args->B[(k + 1) * args->N + j + 2] + a_ik2 * args->B[(k + 2) * args->N + j + 2] + a_ik3 * args->B[(k + 3) * args->N + j + 2];
        args->C[i * args->N + j + 3] += a_ik0 * args->B[k * args->N + j + 3] + a_ik1 * args->B[(k + 1) * args->N + j + 3] + a_ik2 * args->B[(k + 2) * args->N + j + 3] + a_ik3 * args->B[(k + 3) * args->N + j + 3];
      }
      // 处理剩余的j值（尾循环）
      for (; j < args->N; j++) {
        args->C[i * args->N + j] += a_ik0 * args->B[k * args->N + j] + a_ik1 * args->B[(k + 1) * args->N + j] + a_ik2 * args->B[(k + 2) * args->N + j] + a_ik3 * args->B[(k + 3) * args->N + j];
      }
    }
    // 处理剩余的k值（尾循环）
    for (; k < args->K; k++) {
      double a_ik = args->A[i * args->K + k];
      int j = 0;
      // 循环展开：每次处理4个j值
      for (; j <= args->N - 4; j += 4) {
        args->C[i * args->N + j]     += a_ik * args->B[k * args->N + j];
        args->C[i * args->N + j + 1] += a_ik * args->B[k * args->N + j + 1];
        args->C[i * args->N + j + 2] += a_ik * args->B[k * args->N + j + 2];
        args->C[i * args->N + j + 3] += a_ik * args->B[k * args->N + j + 3];
      }
      // 处理剩余的j值（尾循环）
      for (; j < args->N; j++) {
        args->C[i * args->N + j] += a_ik * args->B[k * args->N + j];
      }
    }
  }
}