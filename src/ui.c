#include <stdio.h>
#include "ui.h"
#include "game.h"
#include "i18n.h"

void print_board(const char board[9]) {
    printf("\n");
    for (int i = 0; i < 9; i++) {
        char c = (board[i] == EMPTY) ? (char)('1' + i) : board[i];
        printf(" %c ", c);
        if (i % 3 != 2) {
            printf("|");
        } else if (i != 8) {
            printf("\n---+---+---\n");
        }
    }
    printf("\n\n");
}

int get_move(const char board[9]) {
    int pos;
    while (1) {
        printf("%s", i18n_get(STR_INPUT_POSITION));
        if (scanf("%d", &pos) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            printf("%s", i18n_get(STR_INVALID_NUMBER));
            continue;
        }
        if (pos < 1 || pos > 9) {
            printf("%s", i18n_get(STR_OUT_OF_RANGE));
            continue;
        }
        if (board[pos - 1] != EMPTY) {
            printf("%s", i18n_get(STR_OCCUPIED));
            continue;
        }
        return pos - 1;
    }
}