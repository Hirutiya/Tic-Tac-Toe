#include <stdlib.h>
#include "ai.h"
#include "game.h"

int ai_random_move(const char board[9]) {
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