#include "main.h"
#include "lib/lib.h"

int N = 2048;
int M = 2048;
int K = 2048;
int local_M = N;

int main(){
  chrono::time_point<chrono::high_resolution_clock> start_time, end_time;

  int rank, size;

  double *local_A = (double *)malloc(local_M * K * sizeof(double));
  double *local_B = (double *)malloc(K * N * sizeof(double));
  double *local_C = (double *)malloc(local_M * N * sizeof(double));

  MPI_Init(nullptr, nullptr);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    cout << "[输入矩阵参数 M, 必须为当前进程数量的倍数]" << endl;
    M = input(128, 2048, size);
    cout << "[输入矩阵参数 N]" << endl;
    N = input(128, 2048);
    cout << "[输入矩阵参数 K]" << endl;
    K = input(128, 2048);
    local_M = M / size;  // 每个进程处理的行数，按 A 的行数 M 均等分
    
    // 同步参数
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&K, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&local_M, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double *A = (double *)malloc(M * K * sizeof(double));
    // 初始化矩阵 A, B
    for (int i = 0; i < M * K; i++) A[i] = static_cast<double>(rand()) / RAND_MAX;
    for (int i = 0; i < K * N; i++) local_B[i] = static_cast<double>(rand()) / RAND_MAX;
    // 初始化矩阵 local_C
    for (int i = 0; i < local_M * N; i++) local_C[i] = 0.0;

    start_time = chrono::high_resolution_clock::now(); // 将进程间通信也算在计时里面

    // 分发 A 矩阵
    MPI_Scatter(A, local_M * K, MPI_DOUBLE, local_A, local_M * K, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    // 释放 A 矩阵
    free(A);

    // 同步 B 矩阵
    MPI_Bcast(local_B, K * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  } else {
    // 同步参数
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&K, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&local_M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // 初始化矩阵 local_C
    for (int i = 0; i < local_M * N; i++) local_C[i] = 0.0;
    
    // 接收分发的 A 矩阵
    MPI_Scatter(NULL, local_M * K, MPI_DOUBLE, local_A, local_M * K, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    // 同步 B 矩阵
    MPI_Bcast(local_B, K * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  #ifdef METHOD1
  gemm(local_A, local_B, local_C);
  #endif
  #ifdef METHOD2
  mkl_gemm(local_A, local_B, local_C);
  #endif

  if (rank == 0) {
    double *C = (double *)malloc(M * N * sizeof(double));

    // 收集 C 矩阵
    MPI_Gather(local_C, N * local_M, MPI_DOUBLE, C, N * local_M, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
  
    double flops = 2.0 * M * N * K / 1e9;
    double gflops = flops / elapsed.count();
  
    cout << "Time: " << elapsed.count() << " s" << endl << "GFLOPS: " << gflops << endl;

    free(C);
  } else {
    
    // 收集 C 矩阵
    MPI_Gather(local_C, N * local_M, MPI_DOUBLE, NULL, N * local_M, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  free(local_A);
  free(local_B);
  free(local_C);
  MPI_Finalize();
  return 0;
}
