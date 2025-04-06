#include "main.h"
#include "lib/lib.h"



int main(int argc, char *argv[]){
  chrono::time_point<chrono::high_resolution_clock> start_time, end_time;

  int rank, size;

  MPI_Init(nullptr, nullptr);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  // 创建自定义数据类型
  MPI_Datatype mpi_matrix_params_type = create_matrix_params_type();
  MatrixParams params;

  // 检查进程数是否为完全平方数
  int grid_size = sqrt(size);
  if (grid_size * grid_size != size) {
    if (rank == 0) {
      cerr << "进程数必须是完全平方数！" << endl;
    }
    MPI_Finalize();
    return 1;
  }

  if (rank == 0) {
    if (argc != 4) {
      printf("用法: %s m n k\n", argv[0]);
      return 1;
    }
    params.m = atoi(argv[1]);
    params.n = atoi(argv[2]);
    params.k = atoi(argv[3]);
    
    // 检查矩阵维度是否可以被grid_size整除
    if (params.m % grid_size != 0 || params.n % grid_size != 0 || params.k % grid_size != 0) {
      cerr << "矩阵维度必须能被" << grid_size << "整除" << endl;
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
  }

  // 广播参数
  MPI_Bcast(&params, 1, mpi_matrix_params_type, 0, MPI_COMM_WORLD);

  // 计算每个块的大小
  params.block_m = params.m / grid_size;
  params.block_n = params.n / grid_size;
  params.block_k = params.k / grid_size;

  // 创建2D笛卡尔拓扑
  int dims[2] = {grid_size, grid_size};
  int periods[2] = {1, 1}; // 启用循环边界
  MPI_Comm grid_comm;
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &grid_comm);

  // 获取进程在网格中的坐标
  int coords[2];
  MPI_Cart_coords(grid_comm, rank, 2, coords);
  int my_row = coords[0], my_col = coords[1];

  // 分配本地矩阵块
  double* local_A = (double*)malloc(params.block_m * params.block_k * sizeof(double));
  double* local_B = (double*)malloc(params.block_k * params.block_n * sizeof(double));
  double* local_C = (double*)calloc(params.block_m * params.block_n, sizeof(double));

  // 初始化数据 (作为缓冲区发收)
  double* A = (double*)malloc(params.m * params.k * sizeof(double));;
  double* B = (double*)malloc(params.k * params.n * sizeof(double));;
  if (rank == 0) {    
    // 初始化矩阵 A 和 B
    for (int i = 0; i < params.m * params.k; i++) A[i] = static_cast<double>(rand()) / RAND_MAX;
    for (int i = 0; i < params.k * params.n; i++) B[i] = static_cast<double>(rand()) / RAND_MAX;

    // 开始计时
    start_time = chrono::high_resolution_clock::now();

    // 分发矩阵块
    for (int m = 0; m < params.block_m; m++) {
      memcpy(local_A + m * params.block_k, A + m * params.k, params.block_k * sizeof(double));
    }
    for (int k = 0; k < params.block_k; k++) {
      memcpy(local_B + k * params.block_n, B + k * params.n, params.block_k * sizeof(double));
    }
    for (int i = 0; i < grid_size; i++) {
      for (int j = 0; j < grid_size; j++) {
        if (i == 0 && j == 0) continue;
        int proc_rank;
        int proc_coords[2] = {i, j};
        MPI_Cart_rank(grid_comm, proc_coords, &proc_rank);
        for (int m = 0; m < params.block_m; m++) {
          MPI_Send(A + (i * params.block_m + m) * params.k + j * params.block_k, params.block_k, MPI_DOUBLE, proc_rank, 0, grid_comm);
        }
        for (int k = 0; k < params.block_k; k++) {
          MPI_Send(B + (i * params.block_k + k) * params.n + j * params.block_n, params.block_n, MPI_DOUBLE, proc_rank, 1, grid_comm);
        }
      }
    }
  } else {
    for (int m = 0; m < params.block_m; m++) {
      MPI_Recv(local_A + m * params.block_k, params.block_k, MPI_DOUBLE, 0, 0, grid_comm, MPI_STATUS_IGNORE);
    }
    for (int k = 0; k < params.block_k; k++) {
      MPI_Recv(local_B + k * params.block_n, params.block_n, MPI_DOUBLE, 0, 1, grid_comm, MPI_STATUS_IGNORE);
    }
  }

  // 初始对齐
  int left, right, up, down;
  MPI_Cart_shift(grid_comm, 1, -my_row, &left, &right);
  MPI_Sendrecv_replace(local_A, params.block_m * params.block_k, MPI_DOUBLE, left, 0, right, 0, grid_comm, MPI_STATUS_IGNORE);
  MPI_Cart_shift(grid_comm, 0, -my_col, &up, &down);
  MPI_Sendrecv_replace(local_B, params.block_k * params.block_n, MPI_DOUBLE, up, 0, down, 0, grid_comm, MPI_STATUS_IGNORE);

  // 主循环
  for (int step = 0; step < grid_size; step++) {
    // 本地矩阵乘法
    gemm_cannon(local_A, local_B, local_C, &params);
    // 循环移位 A (向左)
    MPI_Cart_shift(grid_comm, 1, -1, &left, &right);
    MPI_Sendrecv_replace(local_A, params.block_m * params.block_k, MPI_DOUBLE,left, 0, right, 0, grid_comm, MPI_STATUS_IGNORE);
    // 循环移位 B (向上)
    MPI_Cart_shift(grid_comm, 0, -1, &up, &down);
    MPI_Sendrecv_replace(local_B, params.block_k * params.block_n, MPI_DOUBLE,up, 0, down, 0, grid_comm, MPI_STATUS_IGNORE);
  }

  // 收集结果
  if (rank == 0) {
    double* C = (double*)malloc(params.m * params.n * sizeof(double));

    for (int m = 0; m < params.block_m; m++) {
      memcpy(C + m * params.n, local_C + m * params.block_n, params.block_n * sizeof(double));
    }
    for (int i = 0; i < grid_size; i++) {
      for (int j = 0; j < grid_size; j++) {
        if (i == 0 && j == 0) continue;
        int proc_rank;
        int proc_coords[2] = {i, j};
        MPI_Cart_rank(grid_comm, proc_coords, &proc_rank);
        for (int m = 0; m < params.block_m; m++) {
          MPI_Recv(C + (i * params.block_m + m) * params.n + j * params.block_n, params.block_n, MPI_DOUBLE, proc_rank, 0, grid_comm, MPI_STATUS_IGNORE);
        }
      }
    }

    // 结束计时并输出结果
    end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
    
    cout << scientific << setprecision(5) << elapsed.count() << endl;

    #ifdef VERIFY
    double* C_serial = (double*)malloc(params.m * params.n * sizeof(double));
    serial_matrix_mult(A, B, C_serial, params.m, params.k, params.n);
    cout << verify_results(C, C_serial, params.m, params.n) << endl;
    free(C_serial);
    #endif

    free(A);
    free(B);
    free(C);
  } else {
    for (int m = 0; m < params.block_m; m++) {
      MPI_Send(local_C + m * params.block_n, params.block_n, MPI_DOUBLE, 0, 0, grid_comm);
    }
  }

  // 释放资源
  free(local_A);
  free(local_B);
  free(local_C);
  MPI_Comm_free(&grid_comm);
  MPI_Finalize();
  return 0;
}
