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
  
  int rows_per_thread = M / num_threads;
  int extra_rows = M % num_threads;
  
  int start_row = 0;
  for (int i = 0; i < num_threads; i++) {
    thread_args[i].A = A;
    thread_args[i].B = B;
    thread_args[i].C = C;
    thread_args[i].K = K;
    thread_args[i].N = N;
    thread_args[i].start_row = start_row;
    
    int end_row = start_row + rows_per_thread;
    if (i < extra_rows) {
      end_row++;
    }
    thread_args[i].end_row = end_row;
    
    if(i != 0) {
      pthread_create(&threads[i], NULL, gemm, &thread_args[i]);
      bind_thread_to_cpu(threads[i], i);
    }
    start_row = end_row;
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
