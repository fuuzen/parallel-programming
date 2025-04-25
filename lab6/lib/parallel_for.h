#include <stdlib.h>
#include <pthread.h>


const int MAX_NUM_THREADS = 16;  // 最大线程数


class ArgInterface {
public:
  // 这个函数在 join 阶段调用,保证是线程安全的
  virtual void converge() = 0;

  virtual ArgInterface * clone() = 0;
};


// 线程参数结构体
typedef struct {
  int start;
  int end;
  int inc;
  void (*functor)(int, void*);
  void *arg;
  int chunk_size;  // 用于动态调度的块大小
  int schedule_type;  //调度方式, 0:静态, 1:动态
  pthread_mutex_t *mutex;  // 用于动态调度的互斥锁
  int *next_iter;  // 用于动态调度的下一个迭代
} thread_arg_t;


// 线程工作函数 - 静态调度
static void* thread_work_static(void *arg);


// 线程工作函数 - 动态调度
static void* thread_work_dynamic(void *arg);


// 并行for循环函数
void parallel_for(
  int start,
  int end,
  int inc,
  void (*functor)(int, void*),
  void *arg,
  int num_threads = 1,
  bool accumulate = false,  // 是否需要累加
  int schedule_type = 0,
  int chunk_size = 1
);