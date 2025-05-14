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
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">7-MPI并行应用</td>
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
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">2025年05月11日</td>
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

- `lib` 目录下为 `parallel_for` 和相关辅助代码的实现。
- `src` 目录下为 heated plate 问题的模拟程序实现。

`Makefile` 中定义了开发、构建、测试，使用如下：

```shell
# 生成 LSP 配置文件，本实验 Pthreads 不需要链接所以这个不太需要
make dev

# 构建带符号表的程序用于 debug
make debug

# 构建程序
make release

# 运行全部测试
make test

# 单独运行程序
./build/heated_plate

# 清空已构建内容
make clean
```

使用 jupyter notebook 脚本 `draw.ipynb` 根据 `make test` 输出的结果 (`build/result.md`) 画图，直观展示测试结果随相关参数的变化情况。实验报告中的曲线图由该脚本生成。

# 1. MPI 并行应用

## 实验要求

使用 MPI 对快速傅里叶变换进行并行化。

**问题描述**：阅读参考文献中的串行傅里叶变换代码 (`fft_serial.cpp`)，并使用MPI对其进行并行化。

**要求**：并行化：使用 MPI 多进程对 `fft_serial.cpp` 进行并行化。为适应 MPI 的消息传递机制，可能需要对 `fft_serial` 代码进行一定调整。

## 并行化实现

对于从 0 开始标记的 $p=2^k$ 个进程，只需要进行通信 $k$ 次，倒数第 t 次通信，第 $i$ 号进程一定是和第 $i + 2^{t-1} \bmod 2^t$ 号进程进行通信交换数据的。那么第 t 次通信，第 $i$ 号进程一定是和第 $i + 2^{k-t} \bmod 2^{k-t+1}$ 号进程进行通信交换数据的。


## 

# 2. parallel_for 并行应用分析

## 实验要求

对于 Lab6 实现的 parallel_for 版本 heated_plate_openmp 应用：
- 改变并行规模（线程数）及问题规模（N），分析程序的并行性能，例如问题规模 N，值为 2，4，6，8，16，32，64，128，……；并行规模，值为 1，2，4，8 进程/线程。
- 使用 Valgrind massif 工具集采集并分析并行程序的内存消耗。注意 Valgrind 命令中增加 `--stacks=yes` 参数采集程序运行栈内内存消耗。Valgrind massif 输出日志（massif.out.pid）经过 `ms_print` 打印“统计图”，x 轴为程序运行时间，y 轴为内存消耗量。