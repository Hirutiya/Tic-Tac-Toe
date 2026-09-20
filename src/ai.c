#include <stdlib.h>
#include "ai.h"
#include "game.h"

static int find_winning_move(const char board[9], char player) {
    for (int i = 0; i < 8; i++) {
        int a = WIN_PATTERNS[i][0];
        int b = WIN_PATTERNS[i][1];
        int c = WIN_PATTERNS[i][2];

        if (board[a] == player && board[b] == player && board[c] == EMPTY) return c;
        if (board[a] == player && board[c] == player && board[b] == EMPTY) return b;
        if (board[b] == player && board[c] == player && board[a] == EMPTY) return a;
    }
    return -1;
}

static int find_empty_corner(const char board[9]) {
    const int corners[4] = {0, 2, 6, 8};
    for (int i = 0; i < 4; i++) {
        if (board[corners[i]] == EMPTY) return corners[i];
    }
    return -1;
}

static int ai_random_move(const char board[9]) {
    int empty[9];
    int count = 0;

    for (int i = 0; i < 9; i++) {
        if (board[i] == EMPTY) {
            empty[count++] = i;
        }
    }

    if (count == 0) return -1;
    return empty[rand() % count];
}

int ai_move(const char board[9], char ai_player, char component) {
    int move;

    move = find_winning_move(board, ai_player);
    if (move != -1) return move;

    move = find_winning_move(board, component);
    if (move != -1) return move;

    if (board[4] == EMPTY) return 4;

    move = find_empty_corner(board);
    if (move != -1) return move;

    return ai_random_move(board);
}