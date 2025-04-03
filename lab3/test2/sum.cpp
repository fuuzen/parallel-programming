#include "main.h"

void* sum(void *arg) {
  SumArgs* args = (SumArgs*)arg;
  double sum = 0;
  for (long i = args->start; i < args->end; i++) {
    sum += args->A[i];
  }
  args->sum = sum;
  pthread_exit(NULL);
}

void sum(SumArgs *args) {
  double sum = 0;
  for (long i = args->start; i < args->end; i++) {
    sum += args->A[i];
  }
  args->sum = sum;
}
