<div class="cover" style="page-break-after:always;font-family:方正公文仿宋;width:100%;height:100%;border:none;margin: 0 auto;text-align:center;">
    <div style="width:50%;margin: 0 auto;height:0;padding-bottom:10%;">
        </br>
        <img src="../sysu-name.png" alt="校名" style="width:100%;"/>
    </div>
    </br></br>
    <div style="width:40%;margin: 0 auto;height:0;padding-bottom:40%;">
        <img src="../sysu.png" alt="校徽" style="width:100%;"/>
    </div>
		</br></br></br>
    <span style="font-family:华文黑体Bold;text-align:center;font-size:20pt;margin: 10pt auto;line-height:30pt;">本科生实验报告</span>
    </br>
    </br>
    <table style="border:none;text-align:center;width:72%;font-family:仿宋;font-size:14px; margin: 0 auto;">
    <tbody style="font-family:方正公文仿宋;font-size:12pt;">
        <tr style="font-weight:normal;"> 
            <td style="width:20%;text-align:center;">实验课程</td>
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">并行程序设计与算法实验</td>
      </tr>
        <tr style="font-weight:normal;"> 
            <td style="width:20%;text-align:center;">实验名称</td>
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">5-基于OpenMP的并行矩阵乘法</td>
      </tr>
        <tr style="font-weight:normal;"> 
            <td style="width:20%;text-align:center;">专业名称</td>
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">计算机科学与技术</td>
      </tr>
        <tr style="font-weight:normal;"> 
            <td style="width:20%;text-align:center;">学生姓名</td>
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">李世源</td>
      </tr>
        <tr style="font-weight:normal;"> 
            <td style="width:20%;text-align:center;">学生学号</td>
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">22342043</td>
      </tr>
        <tr style="font-weight:normal;"> 
            <td style="width:20%;text-align:center;">实验地点</td>
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋"></td>
      </tr>
        <tr style="font-weight:normal;"> 
            <td style="width:20%;text-align:center;">实验成绩</td>
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋"></td>
      </tr>
      <tr style="font-weight:normal;"> 
            <td style="width:20%;text-align:center;">报告时间</td>
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">2025年04月16日</td>
      </tr>
    </tbody>              
    </table>
</div>

<!-- 注释语句：导出PDF时会在这里分页，使用 Typora Newsprint 主题放大 125% -->



# 实验环境

我的测试平台处理器是 Intel Xeon E7 处理器，单槽 16 核，Intel 给出的性能信息如下：

| Processor Group                                              | GFLOPS | APP     |
| ------------------------------------------------------------ | ------ | ------- |
| Intel® Xeon® Processor E7-4830 v3 (30M Cache, 2.10 GHz) E7-4830V3 | 403.2  | 0.12096 |

# 代码介绍

- `test1` 目录下为   实现。
- `test2` 目录下为   实现。

`Makefile` 中定义了开发、构建、测试，使用如下：

```shell
# 生成 LSP 配置文件，本实验不需要链接所以这个不太需要
make dev

# 只构建不测试
make build

# 运行单次测试
./build/test1  # 
./build/test2  # 

# 清空已构建内容(build 目录)
make clean
```

使用 jupyter notebook 脚本 `draw.ipynb` 根据 `make test2` 输出的结果 (`build/result.md`) 画图，直观展示蒙特卡洛计算结果的变化情况。实验报告中的曲线图由该脚本生成。

# 1. OpenMP通用矩阵乘法

## 实验要求

使用 OpenMP 实现并行通用矩阵乘法，并通过实验分析不同进程数量、矩阵规模、调度机制时该实现的性能。

**输入**：$m,n,k$ 三个整数，每个整数的取值范围均为 $[128, 2048]$。

**问题描述**：随机生成 $m\times n$ 的矩阵 $A$ 及 $n\times k$ 的矩阵 $B$，并对这两个矩阵进行矩阵乘法运算，得到矩阵 $C$。

**输出**：$A,B,C$ 三个矩阵，及矩阵计算所消耗的时间 $t$。

**要求**：使用 OpenMP 多线程实现并行矩阵乘法，设置不同线程数量（1-16）、矩阵规模（128-2048）、调度模式（默认、静态、动态调度），通过实验分析程序的并行性能。

## 程序实现与性能分析

代码实现和串行实现几乎一样，但是在主要的计算循环前，加上了 OpenMP 指令：

```c
{
	// ......
  #pragma omp parallel for collapse(2) schedule(dynamic)
  for(int i = 0; i < m; i++) {
    for(int j = 0; j < n; j++) {
      for(int l = 0; l < k; l++) {
        C[i * n + j] += A[i * k + l] * B[l * k + j];
      }
    }
  }
	// ......
}
```

其中 `collapse(2)` 指令是展开 2 层嵌套的 for 循环。虽然总共是 3 层的 for 循环嵌套，但是我尝试使用 `collapse(3)` 则会产生数据竞争与冒险，出现死锁程序卡住的现象。

测试得到数据画图如下：

<!-- IMAGE -->

可以看到 16 线程并行计算的时间基本比单线程少了 10 倍的数量级，并行加速了 10 倍左右，符合预期的并行加速效果。

但是上述原始的矩阵乘法效率比较低，后来我还尝试调整循环顺序，代码修改为如下：

```c
{
	// ......
  #pragma omp parallel for collapse(2) schedule(dynamic)
  for(int i = 0; i < m; i++) {
    for(int l = 0; l < k; l++) {
      // 将 A[i][l] 存储在寄存器中，减少内存访问
      double a_il = A[i * k + l];
      for(int j = 0; j < n; j++) {
        C[i * n + j] += a_il * B[k * n + j];
      }
    }
  }
	// ......
}
```

但是这样测试得到的性能结果就会发现并行提升变得不再显著。如下所示：

<!-- IMAGE -->




# 2. 构造基于 Pthreads 的并行 for 循环分解、分配、执行机制

模仿 OpenMP 的 omp_parallel_for 构造基于 Pthreads 的并行 for 循环分解、分配及执行机制。

**问题描述**：生成一个包含 parallel_for 函数的动态链接库（.so）文件，该函数创建多个 Pthreads 线程，并行执行 parallel_for 函数的参数所指定的内容。

**函数参数**：parallel_for 函数的参数应当指明被并行循环的索引信息，循环中所需要执行的内容，并行构造等。以下为 parallel_for 函数的基础定义，实验实现应包括但不限于以下内容：

```c
parallel_for(int start, int end, int inc, void *(*functor)( int,void*), void *arg, int num_threads)
```

-	start, end, inc 分别为循环的开始、结束及索引自增量；
-	functor 为函数指针，定义了每次循环所执行的内容；
-	arg 为 functor 的参数指针，给出了 functor 执行所需的数据；
-	num_threads 为期望产生的线程数量。
-	选做：除上述内容外，还可以考虑调度方式等额外参数。

## 代码实现

在我的程序实现中，我设计了 2 种调度模式：
- 静态调度实现:
  - 计算总迭代次数
  - 将迭代次数平均分配给各线程(剩余迭代分配给前几个线程)
  - 为每个线程设置迭代范围( `start-end` )
- 动态调度实现:
  - 初始化互斥锁和共享变量 `next_iter`
  - 所有线程共享相同的循环范围
  - 线程通过互斥锁获取下一个迭代块
  - 执行完当前块后继续获取新块，直到迭代完成

按照上述设计，构建线程参数结构体 `thread_arg_t`：

```c
typedef struct {
  int start;          // 循环起始值
  int end;            // 循环结束值
  int inc;            // 循环增量
  void *(*functor)(int, void*); // 循环体函数指针
  void *arg;          // 传递给循环体函数的参数
  int chunk_size;     // 动态调度时的块大小
  int schedule_type;  // 调度方式(0:静态, 1:动态)
  pthread_mutex_t *mutex; // 动态调度使用的互斥锁
  int *next_iter;     // 动态调度使用的下一个迭代指针
} thread_arg_t;
```

对于静态调度，实现传递给 `pthread_create` 的工作函数 `thread_work_static`：

```c
static void* thread_work_static(void *arg) {
  thread_arg_t *t_arg = (thread_arg_t*)arg;
  
  // 简单地执行分配给该线程的迭代范围
  for (int i = t_arg->start; i < t_arg->end; i += t_arg->inc) {
    t_arg->functor(i, t_arg->arg);
  }
  
  return NULL;
}
```

对于动态调度，实现传递给 `pthread_create` 的工作函数 `thread_work_dynamic`：

```c
static void* thread_work_dynamic(void *arg) {
  thread_arg_t *t_arg = (thread_arg_t*)arg;
  int chunk = t_arg->chunk_size;
  int current_iter;
  
  while (1) {
    // 使用互斥锁保护共享变量next_iter
    pthread_mutex_lock(t_arg->mutex);
    current_iter = *(t_arg->next_iter);
    *(t_arg->next_iter) += chunk * t_arg->inc;
    pthread_mutex_unlock(t_arg->mutex);
    
    // 检查是否超出范围
    if (current_iter >= t_arg->end) break;
    
    // 执行当前迭代块
    int end_iter = current_iter + chunk * t_arg->inc;
    if (end_iter > t_arg->end) end_iter = t_arg->end;
    
    for (int i = current_iter; i < end_iter; i += t_arg->inc) {
      t_arg->functor(i, t_arg->arg);
    }
  }
  
  return NULL;
}
```

最后在主函数 `parallel_for` 中，：

```c
void parallel_for(
  int start, int end, int inc,
  void *(*functor)(int, void*), void *arg,
  int num_threads, int schedule_type, int chunk_size
) {
  // 绑定主线程到 CPU 0
  bind_thread_to_cpu(pthread_self(), 0);

  // 验证线程数
  // 这里我的机器最多支持 16 核，设置 MAX_NUM_THREADS = 16
  if (num_threads <= 0 || num_threads > MAX_NUM_THREADS) {
    num_threads = 1;
  }
  
  // 分配线程和参数数组
  pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
  thread_arg_t *thread_args = malloc(num_threads * sizeof(thread_arg_t));
  
  if (schedule_type == 0) {  // 静态调度
    // 计算总迭代次数
    int total_iters = (end - start) / inc;
    if ((end - start) % inc != 0) total_iters++;
    
    // 计算每个线程的迭代次数
    int iters_per_thread = total_iters / num_threads;
    int remaining_iters = total_iters % num_threads;
    
    // 分配迭代范围给各线程
    int current_start = start;
    for (int i = num_threads - 1; i >= 0; i--) {
      int iters = iters_per_thread;
      if (i < remaining_iters) iters++;
      
      thread_args[i].start = current_start;
      thread_args[i].end = current_start + iters * inc;
      if (thread_args[i].end > end) thread_args[i].end = end;
      thread_args[i].inc = inc;
      thread_args[i].functor = functor;
      thread_args[i].arg = arg;
      
      current_start = thread_args[i].end;
      
      // 创建线程(主线程执行i=0的任务)
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
    int next_iter = start;  // 共享变量
    
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
      
      // 创建线程(主线程执行i=0的任务)
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
```

## 测试分析

为了测试分析，我使用上一个实验 “OpenMP通用矩阵乘法”的代码，将其修改使其调用我实现的 `parallel_for` 函数。首先构建一个传递参数的结构体:

```cpp
typedef struct {
  double *A;
  double *B;
  double *C;
  int n;
  int k;
} GemmArg;
```

然后将原来的三重循环矩阵乘法代码的内部两层改为传递给 `parallel_for` 调用的函数：

```cpp
void* functor(int i, void *arg) {
  GemmArg *args = (GemmArg *)arg;
  double *A = args->A;
  double *B = args->B;
  double *C = args->C;
  int n = args->n;
  int k = args->k;

  for(int j = 0; j < n; j++) {
    for(int l = 0; l < k; l++) {
      C[i * n + j] += A[i * k + l] * B[l * k + j];
    }
  }

  return nullptr;
}
```

设置参数并调用 `parallel_for`：

```cpp
{
  // ......
  Timer *timer = new Timer();  // 开始计时
  
  GemmArg *arg = (GemmArg *)malloc(sizeof(GemmArg));
  arg->A = A;
  arg->B = B;
  arg->C = C;
  arg->n = n;
  arg->k = k;

  parallel_for(0, m, 1, functor, arg, num_threads);

  delete timer;  // 结束计时
  // ......
}
```

测试结果和先前 OpenMP 实现的程序作对比如下：

<!-- IMG -->

可以看到 pthread 实现的模拟 OpenMP 和实际的 OpenMP 在矩阵乘法计算表现的性能已经非常接近。
