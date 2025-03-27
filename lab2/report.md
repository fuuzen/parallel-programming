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
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">2-基于MPI的并行矩阵乘法（进阶）</td>
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
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">2025年03月26日</td>
      </tr>
    </tbody>              
    </table>
</div>


<!-- 注释语句：导出PDF时会在这里分页，使用 Typora Newsprint 主题放大 125% -->


# 实验要求


改进上次实验中的MPI并行矩阵乘法(MPI-v1)，并讨论不同通信方式对性能的影响。


**输入**：$m,n,k$ 三个整数，每个整数的取值范围均为 $[128, 2048]$。


**问题描述：**随机生成 $m\times n$ 的矩阵 $A$ 及 $n\times k$ 的矩阵 $B$，并对这两个矩阵进行矩阵乘法运算，得到矩阵 $C$。


**输出**：$A,B,C$ 三个矩阵，及矩阵计算所消耗的时间 $t$。

**要求**：


1. 采用 MPI 集合通信实现并行矩阵乘法中的进程间通信；使用 `mpi_type_create_struct` 聚合 MPI 进程内变量后通信（例如矩阵尺寸`m`、`n`、`k` 或者其他变量）；
2. 对于不同实现方式，调整并记录不同线程数量（1-16）及矩阵规模（128-2048）下的时间开销，填写表格，并分析其性能及扩展性。
3. （选做）尝试不同数据/任务划分方式、稀疏矩阵、瘦长矩阵等特殊情况。


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


测试时 `make` 指令需要设置 `NP` 变量来指定进程数量。在我的代码中，主进程会需要输入 3 个矩阵的规模参数：


- `M`，范围 $[128,2048]$，必须整除 `NP`。
- `N`，范围 $[128,2048]$。
- `K`，范围 $[128,2048]$。


我的代码逻辑中，矩阵分块是依据 A 矩阵的 M 行进行 NP 等分实现。考虑这样的逻辑下 `NP` 需要整除 `M`，所以上面我对 `M` 进行了整除 `NP` 的检查。


# 测试分析


如下为（不使用 Intel MKL）不同进程数量、不同矩阵规模下的计算时间开销（单位：秒）统计表。

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


