#include "parallel_for.h"
#include "../util/util.h"


static void* thread_work_static(void *arg) {
  thread_arg_t *t_arg = (thread_arg_t*)arg;
  
  for (int i = t_arg->start; i < t_arg->end; i += t_arg->inc) {
    t_arg->functor(i, t_arg->arg);
  }
  
  return NULL;
}


static void* thread_work_dynamic(void *arg) {
  thread_arg_t *t_arg = (thread_arg_t*)arg;
  int chunk = t_arg->chunk_size;
  int current_iter;
  
  while (1) {
    // 获取下一个迭代块
    pthread_mutex_lock(t_arg->mutex);
    current_iter = *(t_arg->next_iter);
    *(t_arg->next_iter) += chunk * t_arg->inc;
    pthread_mutex_unlock(t_arg->mutex);
    
    // 检查是否超出范围
    if (current_iter >= t_arg->end) {
      break;
    }
    
    // 执行当前块
    int end_iter = current_iter + chunk * t_arg->inc;
    if (end_iter > t_arg->end) {
      end_iter = t_arg->end;
    }
    
    for (int i = current_iter; i < end_iter; i += t_arg->inc) {
      t_arg->functor(i, t_arg->arg);
    }
  }
  
  return NULL;
}


void parallel_for(
  int start,
  int end,
  int inc,
  void *(*functor)(int, void*),
  void *arg,
  int num_threads,
  int schedule_type,
  int chunk_size
) {
  bind_thread_to_cpu(pthread_self(), 0);

  if (num_threads <= 0 || num_threads > MAX_NUM_THREADS) {
    num_threads = 1;
  }
  
  pthread_t *threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
  thread_arg_t *thread_args = (thread_arg_t*)malloc(num_threads * sizeof(thread_arg_t));
  
  if (schedule_type == 0) {  // 静态调度
    int total_iters = (end - start) / inc;
    if ((end - start) % inc != 0) {
      total_iters++;
    }
    
    int iters_per_thread = total_iters / num_threads;
    int remaining_iters = total_iters % num_threads;
    
    int current_start = start;
    for (int i = num_threads - 1; i >= 0; i--) {
      int iters = iters_per_thread;
      if (i < remaining_iters) {
        iters++;
      }
      
      thread_args[i].start = current_start;
      thread_args[i].end = current_start + iters * inc;
      if (thread_args[i].end > end) {
        thread_args[i].end = end;
      }
      thread_args[i].inc = inc;
      thread_args[i].functor = functor;
      thread_args[i].arg = arg;
      
      current_start = thread_args[i].end;
      
      if(i != 0) {
        pthread_create(&threads[i], NULL, thread_work_static, &thread_args[i]);  
        bind_thread_to_cpu(threads[i], i);
      } else {
        thread_work_static(&thread_args[i]);
      }
    }
  } else {  // 动态调度
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    int next_iter = start;
    
    for (int i = num_threads - 1; i >= 0; i--) {
      thread_args[i].start = start;
      thread_args[i].end = end;
      thread_args[i].inc = inc;
      thread_args[i].functor = functor;
      thread_args[i].arg = arg;
      thread_args[i].chunk_size = chunk_size;
      thread_args[i].schedule_type = schedule_type;
      thread_args[i].mutex = &mutex;
      thread_args[i].next_iter = &next_iter;
      
      if(i != 0) {
        pthread_create(&threads[i], NULL, thread_work_dynamic, &thread_args[i]);
        bind_thread_to_cpu(threads[i], i);
      } else {
        thread_work_dynamic(&thread_args[i]);
      }
    }
  }
  
  // 等待所有线程完成
  for (int i = 1; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }
  
  free(threads);
  free(thread_args);
}