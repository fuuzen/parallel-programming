# 并行程序设计实验课程作业

```shell
# 进入对应实验项目目录
cd labx

# 只构建不测试
make build

# 生成 LSP 配置文件
make dev

# 运行测试，具体看 Makefile 内容
make test

# 清空已构建内容(build 目录)
make clean
```

## OneAPI
 
cmake 查找和链接 MKL 需要确保有环境变量 `$MKLROOT`。加载环境变量：

```bash
source /path/to/your/oneapi/setvars.sh
```

## CUDA

对于 LSP-Clang 需要 `.clangd` 增加：
- `--cuda-path` 指定 cuda 安装目录
- `-L` link lib 目录
- `-I` include 头文件目录

Windows 上 CUDA 11.8 默认安装路径为例如下：

```yaml
CompileFlags:
  Add:
    - --cuda-path=C://Program Files//NVIDIA GPU Computing Toolkit//CUDA//v11.8
    - -LC://Program Files//NVIDIA GPU Computing Toolkit//CUDA//v11.8//lib
    - -IC://Program Files//NVIDIA GPU Computing Toolkit//CUDA//v11.8//include
```

在 Linux 上 archlinux 默认安装在 `/opt` 下为例如下：

```yaml
CompileFlags:
  Add:
    - --cuda-path=/opt/cuda
    - -L/opt/cuda/lib
    - -I/opt/cuda/include
```

CUDA 编程时想用 LSP 似乎只能这样，将需要的头文件、库手动写在 `.clangd`，让 LSP 把 `.cu` 当作 cpp 文件来做语法检查......