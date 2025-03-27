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
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">1-基于MPI的并行矩阵乘法</td>
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
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">北实验楼B202</td>
      </tr>
        <tr style="font-weight:normal;"> 
            <td style="width:20%;text-align:center;">实验成绩</td>
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋"></td>
      </tr>
      <tr style="font-weight:normal;"> 
            <td style="width:20%;text-align:center;">报告时间</td>
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">2025年03月19日</td>
      </tr>
    </tbody>              
    </table>
</div>



<!-- 注释语句：导出PDF时会在这里分页，使用 Typora Newsprint 主题放大 125% -->


# 实验要求


使用MPI点对点通信方式实现并行通用矩阵乘法(MPI-v1)，并通过实验分析不同进程数量、矩阵规模时该实现的性能。


**输入**：$m,n,k$ 三个整数，每个整数的取值范围均为 $[128, 2048]$。


**问题描述：**随机生成 $m\times n$ 的矩阵 $A$ 及 $n\times k$ 的矩阵 $B$，并对这两个矩阵进行矩阵乘法运算，得到矩阵 $C$。


**输出**： $A,B,C$ 三个矩阵，及矩阵计算所消耗的时间 $t$。

**要求**：


1. 使用 MPI 点对点通信实现并行矩阵乘法，调整并记录不同进程数量（1-16）及矩阵规模（128-2048）下的时间开销，填写表格，并分析其性能。


2. 根据当前实现，在实验报告中讨论两个优化方向：
   1. 在内存有限的情况下，如何进行大规模矩阵乘法计算？
   2. 如何提高大规模稀疏矩阵乘法性能？


# 代码介绍


`Makefile` 中定义了开发、构建、测试，使用如下：


```shell
# 生成 LSP 配置文件
make dev

# 只构建不测试
make build

# 运行单次测试，使用的单线程矩阵乘法为调整循环顺序且循环展开的版本
make test1 NP=16

# 运行单次测试，使用的单线程矩阵乘法为 Intel MKL 版本
make test2 NP=16

# 运行表格上的全部测试，输出表格形式的结果
make test1-all
make test2-all

# 清空已构建内容(build 目录)
make clean
```


手动单次测试时 `make` 指令需要设置 `NP` 变量来指定进程数量。在我的代码中，主进程会需要输入 3 个矩阵的规模参数：


- `M`，范围 $[128,2048]$，必须整除 `NP`。
- `N`，范围 $[128,2048]$。
- `K`，范围 $[128,2048]$。


我的代码逻辑中，矩阵分块是依据 A 矩阵的 M 行进行 NP 等分实现。考虑这样的逻辑下 `NP` 需要整除 `M`，所以上面我对 `M` 进行了整除 `NP` 的检查。


# 测试分析


如下为（不使用 Intel MKL）不同进程数量、不同矩阵规模下的计算时间开销（单位：秒）统计表，我在计时中算入了进程同步矩阵的开销。具体包括：

- rank 0 进程依次向所有其他进程分发矩阵 A 的开销
- rank 0 进程依次向所有其他进程传递矩阵 B 的开销
- rank 0 进程依次从所有其他进程收集矩阵 C 的开销

我的测试平台处理器是 Intel Xeon E7 处理器，单槽 16 核，Intel 给出的性能信息如下：

| Processor Group                                              | GFLOPS | APP     |
| ------------------------------------------------------------ | ------ | ------- |
| Intel® Xeon® Processor E7-4830 v3 (30M Cache, 2.10 GHz) E7-4830V3 | 403.2  | 0.12096 |

<table>
  <caption>
    不同进程数量、不同矩阵规模下的计算时间开销（单位：秒）
  </caption>
  <thead>
    <tr>
      <th rowspan="2">进程数</th>
      <th colspan="5">矩阵规模</th>
    </tr>
    <tr>
      <th>128</th>
      <th>256</th>
      <th>512</th>
      <th>1024</th>
      <th>2048</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>1</td>
      <td>1.6773e-03</td>
      <td>1.3492e-02</td>
      <td>9.1496e-02</td>
      <td>6.3955e-01</td>
      <td>5.1550e+00</td>
    </tr>
    <tr>
      <td>2</td>
      <td>1.3016e-03</td>
      <td>8.0740e-03</td>
      <td>5.0516e-02</td>
      <td>3.8267e-01</td>
      <td>3.5781e+00</td>
    </tr>
    <tr>
      <td>4</td>
      <td>1.0488e-03</td>
      <td>4.5793e-03</td>
      <td>3.0674e-02</td>
      <td>2.2155e-01</td>
      <td>1.9060e+00</td>
    </tr>
    <tr>
      <td>8</td>
      <td>1.0279e-03</td>
      <td>3.1639e-03</td>
      <td>1.6784e-02</td>
      <td>1.4670e-01</td>
      <td>1.1252e+00</td>
    </tr>
    <tr>
      <td>16</td>
      <td>2.1990e-03</td>
      <td>4.3904e-03</td>
      <td>1.3505e-02</td>
      <td>1.4987e-01</td>
      <td>8.4506e-01</td>
    </tr>
  </tbody>
</table>


当矩阵规模较小时，比如128x128或256x256，增加进程数对计算时间的改善并不明显，甚至可能因为进程间通信开销的增加而导致时间变长。比如，128x128矩阵在16个进程时的时间开销是0.00219898秒，比8个进程时的0.00102786秒还要大。这是因为小规模矩阵的计算量本身就不大，通信开销反而成了主要的时间消耗，增加进程数并不能显著减少计算时间，反而可能让通信开销变得更明显。


当矩阵规模增大到中等规模，比如512x512或1024x1024时，增加进程数对计算时间的改善就变得显著了。例如，512x512矩阵在16个进程时的时间开销是0.0135054秒，比单进程时的0.0914963秒减少了近7倍。这是因为中等规模矩阵的计算量已经足够大，增加进程数可以有效地分摊计算负载，而通信开销相对于计算时间来说就显得不那么重要了。


对于大规模矩阵，比如2048x2048，增加进程数带来的时间减少更加明显。2048x2048矩阵在16个进程时的时间开销是0.845056秒，比单进程时的5.155秒减少了近6倍。这是因为大规模矩阵的计算量非常大，增加进程数可以显著减少每个进程的计算负载，而通信开销在这种情况下的影响几乎可以忽略不计。


随着矩阵规模的增大，计算时间会呈指数级增长。比如在单进程的情况下，矩阵规模从128x128增加到2048x2048，计算时间从0.00167728秒增加到5.155秒。因为矩阵乘法的计算复杂度是 $O(n^3)$ ，矩阵规模越大，计算量增长得越快。不过，随着进程数的增加，矩阵规模对计算时间的影响会逐渐减弱。比如2048x2048矩阵在16个进程时的时间开销是0.845056秒，比单进程时的5.155秒减少了近6倍。这是因为更多的进程可以分摊计算负载，从而减少每个进程的计算时间。


# 讨论


当矩阵规模远大于内存容量时，对于单台机器，数据存储在磁盘上，通过分批加载到内存计算的方式完成矩阵乘法。这种方法性能瓶颈主要出现在磁盘I/O操作上，需要优化磁盘 I/O 操作，以减少读写开销。


若允许同时使用多台机器，可以通过 Open MPI 或 MapReduce 等并行计算框架，将矩阵分块并分配到不同节点上计算，最后汇总结果。相当于本次实验的 Open MPI 实现的多进程并行从一台机器改为分布到多台不同机器上。


对于大规模稀疏矩阵，有专用硬件可以加速，例如 TPU 或 FPGA 等。


可以采用压缩存储格式存储大规模稀疏矩阵来减少内存占用。在计算时，只对非零元素进行操作，从而节省内存和计算时间。常见的稀疏矩阵存储格式有：


- CSR（Compressed Sparse Row）：按行存储非零元素，适合行优先访问。
- CSC（Compressed Sparse Column）：按列存储非零元素，适合列优先访问。
- COO（Coordinate Format）：存储非零元素的行、列索引和值，适合灵活的访问模式。


还可以对稀疏矩阵的行或列进行预排序，使得非零元素分布更加集中，提高数据局部性，减少缓存未命中，从而提高计算效率。常用的重排序方法包括：


- RCM（Reverse Cuthill-McKee）：基于图的排序算法，将稀疏矩阵视为图（矩阵的非零元素表示图的边）。通过重新排列节点（矩阵的行和列），使得图的带宽最小化。带宽最小化后，非零元素会集中在对角线附近，从而提高数据局部性。
- AMD（Approximate Minimum Degree）：基于节点度的排序算法，优先处理度数较小的节点。通过减少填充元，降低矩阵分解的计算复杂度和存储需求。
