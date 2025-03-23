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
