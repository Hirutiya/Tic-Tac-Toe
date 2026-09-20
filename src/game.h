#ifndef GAME_H
#define GAME_H

#define EMPTY ' '
#define PLAYER_X 'X'
#define PLAYER_O 'O'

void init_board(char board[9]);

char check_winner(const char board[9]);

int is_full(const char board[9]);

extern const int WIN_PATTERNS[8][3];

#endif