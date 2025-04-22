#include "main.h"

int main(int argc, char *argv[]) {
  if (argc != 5) {
    printf("Usage: %s m n k num_threads\n", argv[0]);
    return 1;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int k = atoi(argv[3]);
  int num_threads = atoi(argv[4]);

  // 设置OpenMP线程数
  omp_set_num_threads(num_threads);

  // 分配矩阵内存
  double *A = (double *)malloc(m * k * sizeof(double));
  double *B = (double *)malloc(k * n * sizeof(double));
  double *C = (double *)malloc(m * n * sizeof(double));

  #pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < m * k; i++) A[i] = static_cast<double>(rand()) / RAND_MAX;

  #pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < k * n; i++) B[i] = static_cast<double>(rand()) / RAND_MAX;

  Timer *timer = new Timer();  // 开始计时

  // 并行矩阵乘法
  #pragma omp parallel for collapse(2) schedule(dynamic)
  for(int i = 0; i < m; i++) {
    for(int j = 0; j < n; j++) {
      for(int l = 0; l < k; l++) {
        C[i * n + j] += A[i * k + l] * B[l * k + j];
      }
    }
  }
  // for(int i = 0; i < m; i++) {
  //   for(int l = 0; l < k; l++) {
  //     // 将 A[i][l] 存储在寄存器中，减少内存访问
  //     double a_il = A[i * k + l];
  //     for(int j = 0; j < n; j++) {
  //       C[i * n + j] += a_il * B[l * k + j];
  //     }
  //   }
  // }

  delete timer;  // 结束计时

  // 释放内存
  free(A);
  free(B);
  free(C);

  return 0;
}