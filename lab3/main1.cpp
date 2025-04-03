#include "lib/lib.h"

int main(int argc, char *argv[]){
  int M, N, K, num_threads;
  #ifdef INTERACTIVE
  cout << "[输入线程数量 num_threads]" << endl;
  num_threads = input(1, 16);
  cout << "[输入矩阵参数 M]" << endl;
  M = input(128, 2048, num_threads);
  cout << "[输入矩阵参数 N]" << endl;
  N = input(128, 2048);
  cout << "[输入矩阵参数 K]" << endl;
  K = input(128, 2048);
  #else
  if (argc != 5) {
    printf("用法: %s m k n num_threads\n", argv[0]);
    return 1;
  }
  M = atoi(argv[1]);
  K = atoi(argv[2]);
  N = atoi(argv[3]);
  num_threads = atoi(argv[4]);
  #endif

  double *A = (double *)malloc(M * K * sizeof(double));
  double *B = (double *)malloc(K * N * sizeof(double));
  double *C = (double *)malloc(M * N * sizeof(double));
  
  // 初始化矩阵 A, B, C
  for (int i = 0; i < M * K; i++) A[i] = static_cast<double>(rand()) / RAND_MAX;
  for (int i = 0; i < K * N; i++) B[i] = static_cast<double>(rand()) / RAND_MAX;
  for (int i = 0; i < M * N; i++) C[i] = 0.0;

  pthread_t threads[num_threads];
  ThreadArgs thread_args[num_threads];
  
  int rows_per_thread = M / num_threads;
  int extra_rows = M % num_threads;
  
  int start_row = 0;
  for (int i = 0; i < num_threads; i++) {
    thread_args[i].A = A;
    thread_args[i].B = B;
    thread_args[i].C = C;
    thread_args[i].start_row = start_row;
    
    int end_row = start_row + rows_per_thread;
    if (i < extra_rows) {
      end_row++;
    }
    thread_args[i].end_row = end_row;
    
    if(i != 0) {
      pthread_create(&threads[i], NULL, gemm_thread, &thread_args[i]);
    }
    start_row = end_row;
  }
  
  gemm_thread(&thread_args[0]);

  for (int i = 1; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  free(A);
  free(B);
  free(C);
  return 0;
}
