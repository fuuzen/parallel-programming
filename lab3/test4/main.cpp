#include "main.h"

// 全局变量用于存储总和和互斥锁
GlobalSum global_sum;

int main(int argc, char *argv[]) {
  bind_thread_to_cpu(pthread_self(), 0);

  int N, num_threads;

  if (argc != 3) {
    printf("用法: %s n num_threads \n", argv[0]);
    return 1;
  }
  N = atoi(argv[1]);
  num_threads = atoi(argv[2]);

  // 初始化互斥锁
  pthread_mutex_init(&global_sum.lock, NULL);
  global_sum.total = 0.0;

  double *A = (double *)malloc(N * sizeof(double));
  
  // 初始化数组 A
  for (int i = 0; i < N; i++) A[i] = static_cast<double>(rand()) / RAND_MAX;

  Timer *timer = new Timer();  // 开始计时

  pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
  SumArgs *thread_args = (SumArgs *)malloc(num_threads * sizeof(SumArgs));
  
  int nums_per_thread = N / num_threads;
  int extra_nums = N % num_threads;
  
  int start = 0;
  for (int i = 0; i < num_threads; i++) {
    thread_args[i].A = A;
    thread_args[i].start = start;
    
    int end = start + nums_per_thread;
    if (i < extra_nums) {
      end++;
    }
    thread_args[i].end = end;
    
    if(i != 0) {
      pthread_create(&threads[i], NULL, sum, &thread_args[i]);
      bind_thread_to_cpu(threads[i], i);
    }
    start = end;
  }

  // 主线程参与计算
  sum(&thread_args[0]);
  
  // 等待所有线程完成
  for (int i = 1; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  delete timer;  // 结束计时
  free(A);
  pthread_mutex_destroy(&global_sum.lock);
  return 0;
}