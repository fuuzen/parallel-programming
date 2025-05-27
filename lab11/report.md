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
            <td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">10-CUDA并行矩阵乘法</td>
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

# 单独运行程序测试
./build-release/task1

# 运行全部测试
make test

# 清空已构建内容
make clean
```

使用 jupyter notebook 脚本 `draw.ipynb` 根据 `make test` 输出的结果画图，直观展示测试结果随相关参数的变化情况。实验报告中的曲线图由该脚本生成。

# 任务1

## 实验要求

在信号处理、图像处理和其他工程/科学领域，卷积是一种使用广泛的技术。在深度学习领域，卷积神经网络(CNN)这种模型架构就得名于这种技术。在本实验中，我们将在GPU上实现卷积操作，注意这里的卷积是指神经网络中的卷积操作，与信号处理领域中的卷积操作不同，它不需要对 Filter 进行翻转，不考虑 bias

任务一通过 CUDA 实现直接卷积（滑窗法），输入从 `256*256` 增加至 `4096*4096` 或者输入从 `32*32` 增加至 `512*512`。

**输入**：Input matrix size 和 Kernel size, 例如 32 和 3

**问题描述**：用直接卷积的方式对 Input 进行卷积，这里只需要实现 2D, `height*width`，通道 channel(depth) 设置为 `3`，Kernel (Filter) 大小设置为 `3*3`，kernel channel(depth) 设置为 `3`，步幅 (stride) 分别设置为 `1`，`2`，`3`，可能需要通过填充 (padding) 配合步幅 (stride) 完成 CNN 操作。

> 注：实验的卷积操作不需要考虑 bias(b)，bias 设置为 `0`。

**输出**：输出卷积结果以及计算时间

## 程序实现





# 任务2

## 实验要求

任务二使用 im2col 方法结合上次实验实现的 GEMM 实现卷积操作。输入从 `256*256` 增加至 `4096*4096` 或者输入从 `32*32` 增加至 `512*512`，具体实现的过程可以参考下面的图片和参考资料。

**输入**：Input matrix size 和 Kernel size, 例如 32 和 3

**问题描述**：用直接卷积的方式对 Input 进行卷积，这里只需要实现 2D, `height*width`，通道 channel(depth) 设置为 `3`，Kernel (Filter) 大小设置为 `3*3`，kernel channel(depth) 设置为 `3`，步幅 (stride) 分别设置为 `1`，`2`，`3`，可能需要通过填充 (padding) 配合步幅 (stride) 完成 CNN 操作。

> 注：实验的卷积操作不需要考虑 bias(b)，bias 设置为 `0`。

**输出**：输出卷积结果以及计算时间

## 程序实现





# 任务3

## 实验要求

NVIDIA cuDNN 是用于深度神经网络的 GPU 加速库。它强调性能、易用性和低内存开销。

使用 cuDNN 提供的卷积方法进行卷积操作，记录其相应 Input 的卷积时间，与自己实现的卷积操作进行比较。如果性能不如 cuDNN，用文字描述可能的改进方法。

## 程序实现


