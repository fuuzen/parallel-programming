# 并行程序设计实验课程作业

## OneAPI

cmake 查找和链接 MKL 需要确保有环境变量 `$MKLROOT`。

喜欢用 fish 的小朋友有福了（比如我），oneapi 提供的设置环境变量经典和 bash 语法不兼容...... 赶紧换个 shell 吧 (比如我换成 oh-my-zsh 了)：

```shell
cat /etc/shells
chsh -s /usr/bin/zsh
```

设置环境变量：

```shell
source /path/to/your/oneapi/setvars.sh
```

最好将如上内容加到你的 `~/.bashrc`（用 bash 的话） 或 `~/.zshrc`（用 zsh 的话） 里。

## clangd

C++ language server，配置 `.clangd` 也依赖于上面所说的环境变量 $`$MKLROOT`。

而且使用 VSCode 开发时，clangd 能读取这个环境变量要求 `$SHELL` 加载配置阶段就设置好环境变量 `$MKLROOT`，所以上面说“最好”将配置加到 `~/.bashrc` 或 `~/.zshrc`。
