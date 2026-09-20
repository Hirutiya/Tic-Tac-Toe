#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "ui.h"
#include "ai.h"

#define MODE_PVP 1
#define MODE_PVE 2

int main(void)
{
    char board[9];
    char current;
    char again;
    int mode;

    srand((unsigned)time(NULL));

    printf("Tic Tac Toe\n");
    printf("1. Player vs Player\n");
    printf("2. Player vs Computer\n");
    printf("Please select mode (1/2): ");
    scanf("%d", &mode);
    if (mode != MODE_PVP && mode != MODE_PVE)
    {
        mode = MODE_PVE;
    }

    do
    {
        init_board(board);
        current = PLAYER_X;

        while (1)
        {
            print_board(board);
            printf("Player %c's turn\n", current);

            int move;
            if (mode == MODE_PVE && current == PLAYER_O)
            {
                printf("Computer is thinking...\n");
                move = ai_random_move(board);
            }
            else
            {
                move = get_move(board);
            }

            if (move < 0 || move > 8)
            {
                printf("AI encountered an error, game over.\n");
                break;
            }

            board[move] = current;

            char winner = check_winner(board);
            if (winner != EMPTY)
            {
                print_board(board);
                printf("Player %c wins!\n", winner);
                break;
            }
            if (is_full(board))
            {
                print_board(board);
                printf("Draw!\n");
                break;
            }

            current = (current == PLAYER_X) ? PLAYER_O : PLAYER_X;
        }

        printf("\nPlay again? (y/n): ");
        scanf(" %c", &again);
    } while (again == 'y' || again == 'Y');

    printf("Goodbye!\n");
    return 0;
}