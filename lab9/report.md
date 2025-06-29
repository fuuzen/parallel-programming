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
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">9-CUDA矩阵转置</td>
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
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">2025年05月19日</td>
      </tr>
    </tbody>              
    </table>
</div>


<!-- 注释语句：导出PDF时会在这里分页，使用 Typora Newsprint 主题放大 125% -->



# 实验环境

我的测试平台环境信息如下：

|              CPU            |            GPU           |   CUDA  |     OS     |
|-----------------------------|--------------------------|---------|------------|
| AMD Ryzen 9 7950X3D 16-Core | NVIDIA GeForce GTX 1050  | 12.9.41 | Windows 11 |

# 代码介绍

`Makefile` 中定义了开发、构建、测试，使用如下：

```shell
# 生成 LSP 配置文件，本实验 Pthreads 不需要链接所以这个不太需要
make dev

# 构建带符号表的程序用于 debug
make debug

# 构建程序
make release

# 运行 task1 测试
./build-release/task1

# 运行全部 task2 测试
make task2

# 清空已构建内容
make clean
```

使用 jupyter notebook 脚本 `draw.ipynb` 根据 `make task2` 输出的结果画图，直观展示测试结果随相关参数的变化情况。实验报告中 task2 的曲线图由该脚本生成。

# 1. CUDA Hello World

## 实验要求

本实验为 CUDA 入门练习，由多个线程并行输出 “Hello World！”。

**输入**：三个整数 `n,m,k`，其取值范围为 `[1,32]`

**问题描述**：创建 n 个线程块，每个线程块的维度为 $m\times k$，每个线程均输出线程块编号、二维块内线程编号及 `Hello World!`（如，`Hello World from Thread (1, 2) in Block 10!`）。主线程输出 `Hello World from the host!`。

**要求**：完成上述内容，观察输出，并回答线程输出顺序是否有规律？

## 程序实现

核函数实现如下：

```cpp
__global__ void helloWorldKernel() {
  int blockId = blockIdx.x;
  int threadIdX = threadIdx.x;
  int threadIdY = threadIdx.y;
  printf(
    "Hello World from Thread (%d, %d) in Block %d!\n", 
    threadIdX, threadIdY, blockId
  );
}
```

host 执行代码如下：

```cpp
printf("Hello World from the host!\n");
  
dim3 blockDim(m, k);
dim3 gridDim(n, 1);

helloWorldKernel<<<gridDim, blockDim>>>();

cudaDeviceSynchronize();
```

## 程序测试

测试运行截图如下：

![1](images/1.png)

# 2. CUDA 矩阵转置

## 实验要求

使用 CUDA 对矩阵进行并行转置。

**输入**：整数 n，其取值范围均为 `[512, 2048]`

**问题描述**：随机生成 $n\times n$ 的矩阵 $A$，对其进行转置得到 $A^T$。转置矩阵中第 i 行 j 列上的元素为原矩阵中 j 行 i 列元素，即 $A_{ij}^T=A_{ji}$。

**输出**：矩阵 A 及其转置矩阵 $A^T$，及计算所消耗的时间 t。

**要求**：使用 CUDA 实现并行矩阵转置，分析不同线程块大小，矩阵规模，访存方式，任务/数据划分方式，对程序性能的影响。

## 全局内存实现

全局内存是 GPU 上容量最大但延迟较高的内存类型。每个线程直接从全局内存读取输入矩阵 A 的元素 $A[x][y]$，并将其写入输出矩阵 A^T 的对应位置 $A^T[y][x]$。

函数代码：

```cpp
__global__ void transposeGlobal(float *A, float *AT, int n) {
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;
  if (x < n && y < n) {
    AT[y * n + x] = A[x * n + y];
  }
}
```

每个线程计算其全局索引 `(x, y)`，读取 $A[x][y]$（地址：`x * n + y`），写入 $A^T[y][x]$（地址：`y * n + x`）。

## 共享内存实现

共享内存是每个线程块私有的高速内存，位于 GPU 片上，访问延迟远低于全局内存。这里虽说是“共享内存实现”，但实际上不可能不使用全局内存，只是在全局内存的基础上，使用共享内存试图实现加速。

函数代码：

```cpp
__global__ void transposeShared(double *A, double *AT, int n, int tile_size) {
  __shared__ double tile[32][32];
  int x = blockIdx.x * tile_size + threadIdx.x;
  int y = blockIdx.y * tile_size + threadIdx.y;

  if (x < n && y < n) {
    tile[threadIdx.y][threadIdx.x] = A[y * n + x];
  }
  __syncthreads();

  if (x < n && y < n) {
    AT[x * n + y] = tile[threadIdx.x][threadIdx.y];
  }
}
```

每个线程块分配一个 `tile_size × tile_size` 的共享内存数组 `tile[32][32]`（固定为最大 32×32 以支持 tile_size<=32）。每个线程根据全局索引 `(x, y)` 从全局内存读取 $A[y][x]$（地址 `y * n + x`），并将其存储到共享内存的 `tile[threadIdx.y][threadIdx.x]`，这一步存到共享内存的就是转置后的数据。然后在传回全局内存前先 `__syncthreads()` 确保线程块内的所有线程完成共享内存的加载。共享内存的访问是连续的（合并访问），因为线程块内的线程按顺序加载数据。

## 验证正确性

为了验证矩阵是否准确转置，用如下主机上运行的串行化实现验证：

```cpp
#define EPSILON 0.000001

int verifyTransposed(double *A, double *AT, int n) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (abs(A[i * n + j] - AT[j * n + i]) > EPSILON) {
        return 1;
      } 
    }
  }
  return 0;
}
```

## 程序测试

全局内存实现下，GPU 时间随着矩阵大小（左图）、tile 大小（右图）的变化情况分别如下：

<div style="text-align: center;">
  <img src="images/global_matrix_size.png" alt="global_matrix_size" style="zoom:30%;" />
  <img src="images/global_tile_size.png" alt="global_tile_size" style="zoom:30%;" />
</div>

共享内存实现下，GPU 时间随着矩阵大小（左图）、tile 大小（右图）的变化情况分别如下：

<div style="text-align: center;">
  <img src="images/shared_matrix_size.png" alt="shared_matrix_size" style="zoom:30%;" />
  <img src="images/shared_tile_size.png" alt="shared_tile_size" style="zoom:30%;" />
</div>

可以看出总体上 tile 大小越大 (block 数量越大)，并行性能越好。但是 tile 大小到达足够大之后，并行加速提升就变小甚至消失。例如对于 256x256 规模的矩阵转置，当 tile 大小达到 4x4 之后，无论再怎么扩大 tile，都无法带来显著的加速了；对于 2048x2048 规模的矩阵，这个 tile size 的阈值抬升为 8x8，也就是当 tile 大小达到 8x8 之后即使继续扩大 tile 也无法带来显著的加速。

最后考虑两种访存实现的对比，将二者 GPU 时间随着矩阵大小变化情况画在同一张对比图如下：

<img src="images/comparison.png" alt="comparison" style="zoom:40%;" />

这里可以看到纯全局内存的实现比增加共享内存后的性能要稍微好一些。可能是因为这里共享内存的实现涉及的内存操作更多，而且矩阵转置这一操作没有任何计算量，至多就是计算新下标的计算量，甚至可以认为是纯内存操作，所以单纯的全局内存就足够了。