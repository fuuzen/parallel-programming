#include "main.h"
#include <cmath>
#include <string>
#include <cstdio>

void * calc_by_double(void* arg) {
  Args* args = (Args*)arg;
  double x, y;
  for (int i = 0; i < args->count_all; i++) {
    x = static_cast<double>(rand());
    y = static_cast<double>(rand());
    if (x * x + y * y <= QUADRA_RAND_MAX_DOUBLE) {
      args->count_inside += 1;
    }
  }
  return NULL;
}

void * calc_by_int(void* arg) {
  Args* args = (Args*)arg;
  uint64_t x, y;
  for (int i = 0; i < args->count_all; i++) {
    x = rand();
    y = rand();
    if (x * x + y * y <= QUADRA_RAND_MAX_INT) {
      args->count_inside += 1;
    }
  }
  return NULL;
}

int main(int argc, char *argv[]){
  bind_thread_to_cpu(pthread_self(), 0);

  if (argc != 3) {
    printf("用法: %s n num_threads\n", argv[0]);
    return 1;
  }

  int count_all = std::stod(argv[1]);
  int count_inside = 0;
  int num_threads = atoi(argv[2]);
  int count_all_each = count_all / num_threads;

  Timer *timer = new Timer();  // 开始计时

  pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
  Args *thread_args = (Args *)malloc(num_threads * sizeof(Args));

  for (int i = 1; i < num_threads; i++) {
    thread_args[i].count_all = count_all_each;
    thread_args[i].count_inside = 0;
    pthread_create(&threads[i], NULL, calc_by_double, &thread_args[i]);
    bind_thread_to_cpu(threads[i], i);
  }

  thread_args[0].count_all = count_all_each;
  thread_args[0].count_inside = 0;
  calc_by_double(&thread_args[0]);
  count_inside = thread_args[0].count_inside;

  for (int i = 1; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
    count_inside += thread_args[i].count_inside;
  }

  // printf("%.9lf", 4 * static_cast<double>(count_inside) / count_all);
  delete timer;  // 结束计时
  return 0;
}
