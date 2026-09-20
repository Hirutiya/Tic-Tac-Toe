# 井字棋

一個簡單的 C 語言井字棋遊戲。

### [English](/README.md) | [繁體中文](../zh-TW/README.md) | [简体中文](../zh-CN/README.md)

## 构建

### 前置需求

- **Windows 操作系統**
- **GCC 編譯器** (推薦使用 MinGW-w64, 或者 MSYS2)
- **終端** (PowerShell, CMD, 或者 Git Bash)

### 編譯

```bash
gcc -Wall -Wextra -std=c11 src/main.c src/game.c src/ui.c src/ai.c src/i18n.c -o tic_tac_toe.exe
```

### 執行

```bash
./tic_tac_toe.exe
```
