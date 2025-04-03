#include "main.h"

void* sum(void* arg) {
  SumArgs* args = (SumArgs*)arg;
  double local_sum = 0.0;
  
  // 计算本地部分和
  for (int i = args->start; i < args->end; i++) {
    local_sum += args->A[i];
  }
  
  // 使用互斥锁保护全局总和更新
  pthread_mutex_lock(&global_sum.lock);
  global_sum.total += local_sum;
  pthread_mutex_unlock(&global_sum.lock);
  
  return NULL;
}

void sum(SumArgs* args) {
  double local_sum = 0.0;
  
  // 计算本地部分和
  for (int i = args->start; i < args->end; i++) {
    local_sum += args->A[i];
  }
  
  // 使用互斥锁保护全局总和更新
  pthread_mutex_lock(&global_sum.lock);
  global_sum.total += local_sum;
  pthread_mutex_unlock(&global_sum.lock);
}