# 井字棋

一個簡單的 C 語言井字棋遊戲。

### [English](/README.md) | [繁體中文](../zh-TW/README.md) | [简体中文](../zh-CN/README.md)

## 構建

```bash
gcc -Wall -Wextra -std=c11 src/main.c src/game.c src/ui.c src/ai.c src/i18n.c -o tic_tac_toe.exe
