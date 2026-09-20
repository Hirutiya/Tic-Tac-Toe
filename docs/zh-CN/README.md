# 井字棋

一个简单的 C 语言井字棋游戏。

### [English](/README.md) | [繁體中文](../zh-TW/README.md) | [简体中文](../zh-CN/README.md)

## 构建

### 前置需求

- **Windows 操作系统**
- **GCC 编译器** (推荐使用 MinGW-w64, 或者 MSYS2)
- **终端** (PowerShell, CMD, 或者 Git Bash)

### 编译

```bash
gcc -Wall -Wextra -std=c11 src/main.c src/game.c src/ui.c src/ai.c src/i18n.c -o tic_tac_toe.exe
```

### 执行

```bash
./tic_tac_toe.exe
```
