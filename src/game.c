#include "game.h"

static const int WIN_PATTERNS[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

void init_board(char board[9])
{
    for (int i = 0; i < 9; i++)
    {
        board[i] = EMPTY;
    }
}

char check_winner(const char board[9])
{
    for (int i = 0; i < 8; i++)
    {
        int a = WIN_PATTERNS[i][0];
        int b = WIN_PATTERNS[i][1];
        int c = WIN_PATTERNS[i][2];
        if (board[a] != EMPTY &&
            board[a] == board[b] &&
            board[a] == board[c])
        {
            return board[a];
        }
    }
    return EMPTY;
}

int is_full(const char board[9])
{
    for (int i = 0; i < 9; i++)
    {
        if (board[i] == EMPTY)
            return 0;
    }
    return 1;
}