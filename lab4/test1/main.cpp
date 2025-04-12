#include "main.h"
#include <cmath>
#include <cstddef>

double a;
double b;
double c;
double delta;
double root1;
double root2;
bool delta_ready = false;
bool has_real_roots;

pthread_cond_t cond_delta_ready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void * calc_delta(void * _) {
  pthread_mutex_lock(&mutex1);
  pthread_mutex_lock(&mutex2);
  double d = b * b - 4 * a * c;
  delta = sqrt(d);
  has_real_roots = d >= 0;
  delta_ready = true;
  pthread_cond_signal (&cond_delta_ready);
  pthread_mutex_unlock(&mutex1);
  pthread_cond_signal (&cond_delta_ready);
  pthread_mutex_unlock(&mutex2);
  pthread_exit(NULL);
}

void * calc_root1(void * _) {
  pthread_mutex_lock(&mutex1);
  while(!delta_ready)
    pthread_cond_wait(&cond_delta_ready, &mutex1);
  root1 = (- b - delta) / (a * 2);
  pthread_exit(NULL);
}

void * calc_root2(void * _) {
  pthread_mutex_lock(&mutex2);
  while(!delta_ready)
    pthread_cond_wait(&cond_delta_ready, &mutex2);
  root2 = (- b + delta) / (a * 2);
  pthread_exit(NULL);
}

int main(int argc, char *argv[]){
  bind_thread_to_cpu(pthread_self(), 0);

  if (argc != 4) {
    printf("用法: %s a b c\n", argv[0]);
    return 1;
  }

  a = atof(argv[1]);
  b = atof(argv[2]);
  c = atof(argv[3]);

  Timer *timer = new Timer();  // 开始计时

  pthread_t *threads = (pthread_t *)malloc(3 * sizeof(pthread_t));
  
  pthread_create(&threads[0], NULL, calc_delta, NULL);
  bind_thread_to_cpu(*threads, 1);
  pthread_create(&threads[1], NULL, calc_root1, NULL);
  bind_thread_to_cpu(*threads, 2);
  pthread_create(&threads[2], NULL, calc_root2, NULL);
  bind_thread_to_cpu(*threads, 3);

  for(int i = 0; i < 3; i++)
    pthread_join(threads[i], NULL);

  if (has_real_roots) {
    printf("x1 = %lf\n", root1);
    printf("x2 = %lf\n", root2);
  } else {
    printf("无实数根\n");
  }

  printf("用时:(秒) ");
  delete timer;  // 结束计时
  return 0;
}
