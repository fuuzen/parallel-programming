#include "main.h"

int main(int argc, char *argv[]){
  bind_thread_to_cpu(pthread_self(), 0);

  int M, N, K, num_threads;

  if (argc != 5) {
    printf("用法: %s m k n num_threads\n", argv[0]);
    return 1;
  }
  M = atoi(argv[1]);
  K = atoi(argv[2]);
  N = atoi(argv[3]);
  num_threads = atoi(argv[4]);

  double *A = (double *)malloc(M * K * sizeof(double));
  double *B = (double *)malloc(K * N * sizeof(double));
  double *C = (double *)malloc(M * N * sizeof(double));
  
  // 初始化矩阵 A, B, C
  for (int i = 0; i < M * K; i++) A[i] = static_cast<double>(rand()) / RAND_MAX;
  for (int i = 0; i < K * N; i++) B[i] = static_cast<double>(rand()) / RAND_MAX;
  for (int i = 0; i < M * N; i++) C[i] = 0.0;

  Timer *timer = new Timer();  // 开始计时

  pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
  GemmArgs *thread_args = (GemmArgs *)malloc(num_threads * sizeof(GemmArgs));
  
  auto [block_rows, block_cols] = get_block_distribution(num_threads, M, N);

  int *row_divisions = (int *)malloc((block_rows + 1) * sizeof(int));
  int *col_divisions = (int *)malloc((block_cols + 1) * sizeof(int));

  for (int i = 0; i <= block_rows; i++) {
    row_divisions[i] = i * M / block_rows;
  }
  for (int j = 0; j <= block_cols; j++) {
    col_divisions[j] = j * N / block_cols;
  }
  
  int thread_idx = 0;
  for (int i = 0; i < block_rows; i++) {
    for (int j = 0; j < block_cols; j++) {
      auto& arg = thread_args[thread_idx];
      arg.A = A;
      arg.B = B;
      arg.C = C;
      arg.K = K;
      arg.N = N;
      arg.start_row = row_divisions[i];
      arg.end_row = row_divisions[i+1];
      arg.start_col = col_divisions[j];
      arg.end_col = col_divisions[j+1];
      
      if (thread_idx != 0) {
        pthread_create(&threads[thread_idx], NULL, gemm, &arg);
        bind_thread_to_cpu(threads[thread_idx], thread_idx);
      }
      thread_idx++;
    }
  }

  gemm(&thread_args[0]);

  for (int i = 1; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  delete timer;  // 结束计时
  free(A);
  free(B);
  free(C);
  return 0;
}
