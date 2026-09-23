#include <stdlib.h>
#include "ai.h"
#include "game.h"

static int find_winning_move(const char board[9], char player)
{
    for (int i = 0; i < 8; i++)
    {
        int a = WIN_PATTERNS[i][0];
        int b = WIN_PATTERNS[i][1];
        int c = WIN_PATTERNS[i][2];

        if (board[a] == player && board[b] == player && board[c] == EMPTY)
            return c;
        if (board[a] == player && board[c] == player && board[b] == EMPTY)
            return b;
        if (board[b] == player && board[c] == player && board[a] == EMPTY)
            return a;
    }
    return -1;
}

static int find_empty_corner(const char board[9])
{
    const int corners[4] = {0, 2, 6, 8};
    for (int i = 0; i < 4; i++)
    {
        if (board[corners[i]] == EMPTY)
            return corners[i];
    }
    return -1;
}

static int ai_random_move(const char board[9])
{
    int empty[9];
    int count = 0;

    for (int i = 0; i < 9; i++)
    {
        if (board[i] == EMPTY)
            empty[count++] = i;
    }

    if (count == 0)
        return -1;
    return empty[rand() % count];
}

int ai_simple_move(const char board[9], char ai_player, char component)
{
    int move;

    move = find_winning_move(board, ai_player);
    if (move != -1)
        return move;

    move = find_winning_move(board, component);
    if (move != -1)
        return move;

    if (board[4] == EMPTY)
        return 4;

    move = find_empty_corner(board);
    if (move != -1)
        return move;

    return ai_random_move(board);
}

static int evaluate(const char board[9], char ai_player)
{
    char winner = check_winner(board);
    if (winner == ai_player)
        return 1;
    if (winner != EMPTY)
        return -1;
    if (is_full(board))
        return 0;
    return 2;
}

static int minimax(char board[9], char current_player, char ai_player, int depth)
{
    int score = evaluate(board, ai_player);

    if (score != 2)
    {
        if (score == 1)
            return 10 - depth;
        if (score == -1)
            return depth - 10;
        return 0;
    }

    if (current_player == ai_player)
    {
        int best = -1000;
        for (int i = 0; i < 9; i++)
        {
            if (board[i] == EMPTY)
            {
                board[i] = current_player;
                int val = minimax(board, (current_player == PLAYER_X) ? PLAYER_O : PLAYER_X, ai_player, depth + 1);
                board[i] = EMPTY;
                if (val > best)
                    best = val;
            }
        }
        return best;
    }
    else
    {
        int best = 1000;
        for (int i = 0; i < 9; i++)
        {
            if (board[i] == EMPTY)
            {
                board[i] = current_player;
                int val = minimax(board, (current_player == PLAYER_X) ? PLAYER_O : PLAYER_X, ai_player, depth + 1);
                board[i] = EMPTY;
                if (val < best)
                    best = val;
            }
        }
        return best;
    }
}

int ai_minimax_move(const char board[9], char ai_player, char opponent)
{
    int best_score = -1000;
    int best_move = -1;

    for (int i = 0; i < 9; i++)
    {
        if (board[i] == EMPTY)
        {
            char temp[9];
            for (int j = 0; j < 9; j++)
                temp[j] = board[j];

            temp[i] = ai_player;
            int score = minimax(temp, opponent, ai_player, 0);

            if (score > best_score)
            {
                best_score = score;
                best_move = i;
            }
        }
    }
    return best_move;
}