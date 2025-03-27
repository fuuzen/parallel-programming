#include "main.h"
#include "lib/lib.h"

int N = 2048;
int M = 2048;
int K = 2048;
int local_M = N;

int main(){
  chrono::time_point<chrono::high_resolution_clock> start_time, end_time;

  int rank, size;

  MPI_Init(nullptr, nullptr);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    #ifdef INTERACTIVE
    cout << "[输入矩阵参数 M, 必须为当前进程数量的倍数]" << endl;
    M = input(128, 2048, size);
    cout << "[输入矩阵参数 N]" << endl;
    N = input(128, 2048);
    cout << "[输入矩阵参数 K]" << endl;
    K = input(128, 2048);
    #else
    cin >> M >> N >> K;
    #endif
    local_M = M / size;  // 每个进程处理的行数，按 A 的行数 M 均等分
    
    // 同步参数
    for (int dest = 1; dest < size; dest++) {
      MPI_Send(&M, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
      MPI_Send(&N, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&K, 1, MPI_INT, dest, 2, MPI_COMM_WORLD);
      MPI_Send(&local_M, 1, MPI_INT, dest, 3, MPI_COMM_WORLD);
    }

    double *local_A = (double *)malloc(local_M * K * sizeof(double));
    double *local_B = (double *)malloc(K * N * sizeof(double));
    double *local_C = (double *)malloc(local_M * N * sizeof(double));
    double *A = (double *)malloc(M * K * sizeof(double));
    double *C = (double *)malloc(M * N * sizeof(double));

    // 初始化矩阵 A, B
    for (int i = 0; i < M * K; i++) A[i] = static_cast<double>(rand()) / RAND_MAX;
    for (int i = 0; i < K * N; i++) local_B[i] = static_cast<double>(rand()) / RAND_MAX;
    // 初始化矩阵 local_C
    for (int i = 0; i < local_M * N; i++) local_C[i] = 0.0;

    start_time = chrono::high_resolution_clock::now(); // 将进程间通信也算在计时里面

    // 分发 A 矩阵，首先保留rank 0的部分
    memcpy(local_A, A, local_M * K * sizeof(double));
    // 然后发送其他部分
    for (int dest = 1; dest < size; dest++) {
      MPI_Send(A + dest * local_M * K, local_M * K, MPI_DOUBLE, dest, 4, MPI_COMM_WORLD);
    }

    // 同步 B 矩阵
    for (int dest = 1; dest < size; dest++) {
      MPI_Send(local_B, K * N, MPI_DOUBLE, dest, 5, MPI_COMM_WORLD);
    }

    #ifdef METHOD1
    gemm(local_A, local_B, local_C);
    #endif
    #ifdef METHOD2
    mkl_gemm(local_A, local_B, local_C);
    #endif

    // 收集 C 矩阵，首先保留rank 0的部分
    memcpy(C, local_C, local_M * N * sizeof(double));
    // 然后接收其他部分
    for (int src = 1; src < size; src++) {
      MPI_Recv(C + src * local_M * N, local_M * N, MPI_DOUBLE, src, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
  
    double flops = 2.0 * M * N * K / 1e9;
  
    cout << scientific << setprecision(5) << elapsed.count() << endl;

    free(A);
    free(C);
    free(local_A);
    free(local_B);
    free(local_C);
  } else {
    // 同步参数
    MPI_Recv(&M, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&N, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&K, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&local_M, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    double *local_A = (double *)malloc(local_M * K * sizeof(double));
    double *local_B = (double *)malloc(K * N * sizeof(double));
    double *local_C = (double *)malloc(local_M * N * sizeof(double));
    
    // 初始化矩阵 local_C
    for (int i = 0; i < local_M * N; i++) local_C[i] = 0.0;
    
    // 接收分发的 A 矩阵
    MPI_Recv(local_A, local_M * K, MPI_DOUBLE, 0, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // 接收 B 矩阵
    MPI_Recv(local_B, K * N, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    #ifdef METHOD1
    gemm(local_A, local_B, local_C);
    #endif
    #ifdef METHOD2
    mkl_gemm(local_A, local_B, local_C);
    #endif
    
    // 发送 local_C 矩阵
    MPI_Send(local_C, local_M * N, MPI_DOUBLE, 0, 6, MPI_COMM_WORLD);

    free(local_A);
    free(local_B);
    free(local_C);
  }
  MPI_Finalize();
  return 0;
}
