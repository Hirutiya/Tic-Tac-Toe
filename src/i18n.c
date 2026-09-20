#include "i18n.h"

static const char *strings[LANG_COUNT][STR_COUNT] = {
    {
        "=== Tic-Tac-Toe ===",
        "Select mode:",
        "1. Player vs Player",
        "2. Player vs Computer (You are X, AI is O)",
        "Choose (1/2): ",
        "Player %c's turn\n",
        "Computer is thinking...\n",
        "Enter position (1-9): ",
        "Invalid input. Please enter a number.\n",
        "Position must be between 1 and 9.\n",
        "That position is already occupied. Please choose another.\n",
        "Player %c wins!\n",
        "Draw!\n",
        "AI error. Game over.\n",
        "Play again? (y/n): ",
        "Goodbye!\n"
    },
    {
        "=== 井字棋 ===",
        "請選擇模式:",
        "1. 雙人對戰",
        "2. 人機對戰 (玩家 X，電腦 O)",
        "請選擇 (1/2): ",
        "輪到玩家 %c\n",
        "電腦思考中...\n",
        "請輸入位置 (1-9): ",
        "輸入無效，請輸入數字。\n",
        "位置必須在 1~9 之間。\n",
        "該位置已被佔用，請重新輸入。\n",
        "玩家 %c 獲勝！\n",
        "平手！\n",
        "AI 發生錯誤，遊戲結束。\n",
        "要再玩一次嗎？(y/n): ",
        "再見！\n"
    },
    {
        "=== 井字棋 ===",
        "请选择模式:",
        "1. 双人对战",
        "2. 人机对战 (玩家 X，电脑 O)",
        "请选择 (1/2): ",
        "轮到玩家 %c\n",
        "电脑思考中...\n",
        "请输入位置 (1-9): ",
        "输入无效，请输入数字。\n",
        "位置必须在 1~9 之间。\n",
        "该位置已被占用，请重新输入。\n",
        "玩家 %c 获胜！\n",
        "平局！\n",
        "AI 发生错误，游戏结束。\n",
        "要再玩一次吗？(y/n): ",
        "再见！\n"
    }
};

static Language current_lang = LANG_EN;

void i18n_set_language(Language lang) {
    if (lang >= 0 && lang < LANG_COUNT) current_lang = lang;
}

const char *i18n_get(StringID id) {
    if (id < 0 || id >= STR_COUNT) return "???";
    return strings[current_lang][id];
}