#include <stdio.h>
#include "game.h"
#include "ui.h"

int main(void)
{
    char board[9];
    char current;
    char again;

    do
    {
        init_board(board);
        current = PLAYER_X;

        while (1)
        {
            print_board(board);
            printf("Player %c's turn\n", current);

            int move = get_move(board);
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

    printf("Bye Bye!\n");
    return 0;
}