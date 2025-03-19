#include <mpi.h>
#include "lib.h"

/**
 * MPI 的矩阵乘法实现
 */
void mpi_gemm(
  int argc,
  char **argv,
  const double *A,
  const double *B,
  double *C
) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int local_M = M / size;  // 每个进程处理的行数，按 A 的行数 M 均等分
  double *A_ = (double *)malloc(N * local_M * sizeof(double));
  double *B_ = (double *)malloc(N * N * sizeof(double));
  double *C_ = (double *)malloc(N * local_M * sizeof(double));

  // 初始化矩阵 A 和 B
  if (rank == 0) {
    double *global_A = (double *)malloc(N * N * sizeof(double));
    double *global_B = (double *)malloc(N * N * sizeof(double));

    for (int i = 0; i < N * N; i++) {
      global_A[i] = (double)(i + 1);
      global_B[i] = (double)(i + 1);
    }

    // 分发矩阵 A 的行块给各个进程
    MPI_Scatter(global_A, N * local_M, MPI_DOUBLE, A_, N * local_M, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // 广播矩阵 B 给所有进程
    MPI_Bcast(global_B, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    free(global_A);
    free(global_B);
  } else {
    MPI_Scatter(NULL, N * local_M, MPI_DOUBLE, A_, N * local_M, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(B_, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  // 每个进程计算自己的部分
  gemm(A_, B_, C_, local_M);

  // 收集结果到 rank 0
  if (rank == 0) {
    double *global_C = (double *)malloc(N * N * sizeof(double));
    MPI_Gather(C, N * local_M, MPI_DOUBLE, global_C, N * local_M, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    free(global_C);
  } else {
    MPI_Gather(C, N * local_M, MPI_DOUBLE, NULL, N * local_M, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  free(A_);
  free(B_);
  free(C_);

  MPI_Finalize();
}