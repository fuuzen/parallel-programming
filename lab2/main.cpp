#include "main.h"
#include "lib/lib.h"

int main(){
  chrono::time_point<chrono::high_resolution_clock> start_time, end_time;

  int rank, size;

  MPI_Init(nullptr, nullptr);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  // 创建自定义数据类型
  MPI_Datatype mpi_matrix_params_type = create_matrix_params_type();
  MatrixParams params;

  if (rank == 0) {
    cout << "[输入矩阵参数 M, 必须为当前进程数量的倍数]" << endl;
    params.m = input(128, 2048, size);
    cout << "[输入矩阵参数 params.n]" << endl;
    params.n = input(128, 2048);
    cout << "[输入矩阵参数 params.k]" << endl;
    params.k = input(128, 2048);
    params.local_m = params.m / size;  // 每个进程处理的行数，按 A 的行数 M 均等分
    
    // 同步参数
    MPI_Bcast(&params, 1, mpi_matrix_params_type, 0, MPI_COMM_WORLD);

    double *local_A = (double *)malloc(params.local_m * params.k * sizeof(double));
    double *local_B = (double *)malloc(params.k * params.n * sizeof(double));
    double *local_C = (double *)malloc(params.local_m * params.n * sizeof(double));
    double *A = (double *)malloc(params.m * params.k * sizeof(double));
    double *C = (double *)malloc(params.m * params.n * sizeof(double));
    
    // 初始化矩阵 A, B
    for (int i = 0; i < params.m * params.k; i++) A[i] = static_cast<double>(rand()) / RAND_MAX;
    for (int i = 0; i < params.k * params.n; i++) local_B[i] = static_cast<double>(rand()) / RAND_MAX;
    // 初始化矩阵 local_C
    for (int i = 0; i < params.local_m * params.n; i++) local_C[i] = 0.0;

    start_time = chrono::high_resolution_clock::now(); // 将进程间通信也算在计时里面

    // 分发 A 矩阵
    MPI_Scatter(A, params.local_m * params.k, MPI_DOUBLE, local_A, params.local_m * params.k, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // 同步 B 矩阵
    MPI_Bcast(local_B, params.k * params.n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    #ifdef METHOD1
    gemm(local_A, local_B, local_C, &params);
    #endif
    #ifdef METHOD2
    mkl_gemm(local_A, local_B, local_C, &params);
    #endif

    // 收集 C 矩阵
    MPI_Gather(local_C, params.n * params.local_m, MPI_DOUBLE, C, params.n * params.local_m, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
  
    double flops = 2.0 * params.m * params.n * params.k / 1e9;
    double gflops = flops / elapsed.count();
  
    cout << "Time: " << elapsed.count() << " s" << endl << "GFLOPS: " << gflops << endl;

    free(A);
    free(C);
    free(local_A);
    free(local_B);
    free(local_C);
  } else {
    // 同步参数
    MPI_Bcast(&params, 1, mpi_matrix_params_type, 0, MPI_COMM_WORLD);

    double *local_A = (double *)malloc(params.local_m * params.k * sizeof(double));
    double *local_B = (double *)malloc(params.k * params.n * sizeof(double));
    double *local_C = (double *)malloc(params.local_m * params.n * sizeof(double));
    
    // 初始化矩阵 local_C
    for (int i = 0; i < params.local_m * params.n; i++) local_C[i] = 0.0;
    
    // 接收分发的 A 矩阵
    MPI_Scatter(NULL, params.local_m * params.k, MPI_DOUBLE, local_A, params.local_m * params.k, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    // 同步 B 矩阵
    MPI_Bcast(local_B, params.k * params.n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    #ifdef METHOD1
    gemm(local_A, local_B, local_C, &params);
    #endif
    #ifdef METHOD2
    mkl_gemm(local_A, local_B, local_C, &params);
    #endif
    
    // 收集 C 矩阵
    MPI_Gather(local_C, params.n * params.local_m, MPI_DOUBLE, NULL, params.n * params.local_m, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    free(local_A);
    free(local_B);
    free(local_C);
  }
  MPI_Finalize();
  return 0;
}
