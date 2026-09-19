#include <stdio.h>
#include <stdlib.h>

#define EMPTY ' '
#define PLAYER_X 'X'
#define PLAYER_O 'O'

static const int WIN_PATTERNS[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

void init_board(char board[9])
{
    for (int i = 0; i < 9; i++)
    {
        board[i] = EMPTY;
    }
}

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
            printf("Input is invalid, please enter a number.\n");
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
                printf("Player %c Win！\n", winner);
                break;
            }
            if (is_full(board))
            {
                print_board(board);
                printf("Draw！\n");
                break;
            }

            current = (current == PLAYER_X) ? PLAYER_O : PLAYER_X;
        }

        printf("\nPlay Again? (y/n): ");
        scanf(" %c", &again);
    } while (again == 'y' || again == 'Y');

    printf("Bye Bye!\n");
    return 0;
}