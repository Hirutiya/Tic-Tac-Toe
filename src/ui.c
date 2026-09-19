#include <stdio.h>
#include "ui.h"
#include "game.h"

void print_board(const char board[9])
{
    printf("\n");
    for (int i = 0; i < 9; i++)
    {
        char c = (board[i] == EMPTY) ? (char)('1' + i) : board[i];
        printf(" %c ", c);
        if (i % 3 != 2)
        {
            printf("|");
        }
        else if (i != 8)
        {
            printf("\n---+---+---\n");
        }
    }
    printf("\n\n");
}

int get_move(const char board[9])
{
    int pos;
    while (1)
    {
        printf("Please enter a position (1-9): ");
        if (scanf("%d", &pos) != 1)
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
            {
            }
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (pos < 1 || pos > 9)
        {
            printf("Position must be between 1 and 9.\n");
            continue;
        }
        if (board[pos - 1] != EMPTY)
        {
            printf("The position is already occupied. Please choose another.\n");
            continue;
        }
        return pos - 1;
    }
}