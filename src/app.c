#include <stdio.h>
#include <windows.h>
#include "app.h"
#include "game.h"
#include "ai.h"

int game_mode = MODE_PVP;
char human_player = PLAYER_X;
char ai_player = PLAYER_O;
char board[9];
char current_player;
int game_over;
int win_line[3] = {-1, -1, -1};
int anim_frames[9];
int score_x = 0;
int score_o = 0;
int score_draw = 0;
int cursor_pos = 4;

static StatusCallback g_status_cb = NULL;
static ScoreCallback g_score_cb = NULL;

void app_set_status_callback(StatusCallback cb) {
    g_status_cb = cb;
}

void app_set_score_callback(ScoreCallback cb) {
    g_score_cb = cb;
}

static void notify_status(void) {
    if (!g_status_cb)
        return;

    char buffer[64];
    if (game_over) {
        g_status_cb("Game Over - Press R or click Restart");
    } else if (game_mode == MODE_PVP) {
        sprintf(buffer, "Player %c's turn", current_player);
        g_status_cb(buffer);
    } else {
        if (current_player == human_player) {
            sprintf(buffer, "You are %c - Your turn", human_player);
        } else {
            sprintf(buffer, "You are %c - Computer is thinking...", human_player);
        }
        g_status_cb(buffer);
    }
}

static void notify_score(void) {
    if (g_score_cb)
        g_score_cb(score_x, score_o, score_draw);
}

static void check_game_over(HWND hwnd) {
    char winner = check_winner(board);
    if (winner != EMPTY) {
        game_over = 1;
        if (winner == PLAYER_X) {
            score_x++;
        } else if (winner == PLAYER_O) {
            score_o++;
        }
        for (int i = 0; i < 8; i++) {
            int a = WIN_PATTERNS[i][0];
            int b = WIN_PATTERNS[i][1];
            int c = WIN_PATTERNS[i][2];
            if (board[a] != EMPTY && board[a] == board[b] && board[a] == board[c]) {
                win_line[0] = a;
                win_line[1] = b;
                win_line[2] = c;
                break;
            }
        }
        char msg[64];
        sprintf(msg, "Player %c wins!", winner);
        MessageBoxA(hwnd, msg, "Game Over", MB_OK | MB_ICONINFORMATION);
    } else if (is_full(board)) {
        game_over = 1;
        score_draw++;
        MessageBoxA(hwnd, "Draw!", "Game Over", MB_OK | MB_ICONINFORMATION);
    }
    notify_score();
    notify_status();
}

static void make_ai_move(HWND hwnd) {
    int move = -1;
    if (game_mode == MODE_PVE_EASY) {
        move = ai_simple_move(board, ai_player, human_player);
    } else if (game_mode == MODE_PVE_HARD) {
        move = ai_minimax_move(board, ai_player, human_player);
    }

    if (move >= 0 && move < 9 && board[move] == EMPTY) {
        board[move] = ai_player;
        anim_frames[move] = 1;
        SetTimer(hwnd, ANIM_TIMER_ID, ANIM_INTERVAL, NULL);
        InvalidateRect(hwnd, NULL, TRUE);

        check_game_over(hwnd);
        if (!game_over) {
            current_player = human_player;
            notify_status();
        }
    }
}

void app_start_game(HWND hwnd, int mode, char human_side) {
    if (mode != game_mode) {
        score_x = 0;
        score_o = 0;
        score_draw = 0;
    }

    game_mode = mode;
    human_player = human_side;
    ai_player = (human_side == PLAYER_X) ? PLAYER_O : PLAYER_X;

    init_board(board);
    current_player = PLAYER_X;
    game_over = 0;
    win_line[0] = win_line[1] = win_line[2] = -1;
    cursor_pos = 4;

    for (int i = 0; i < 9; i++)
        anim_frames[i] = 0;
    KillTimer(hwnd, ANIM_TIMER_ID);
    KillTimer(hwnd, AI_TIMER_ID);

    notify_status();
    notify_score();

    if (game_mode != MODE_PVP && ai_player == PLAYER_X) {
        SetTimer(hwnd, AI_TIMER_ID, AI_THINK_DELAY, NULL);
    }
}

void app_player_move(HWND hwnd, int index) {
    if (game_over)
        return;
    if (game_mode != MODE_PVP && current_player != human_player)
        return;
    if (board[index] != EMPTY)
        return;

    board[index] = current_player;
    anim_frames[index] = 1;
    SetTimer(hwnd, ANIM_TIMER_ID, ANIM_INTERVAL, NULL);
    InvalidateRect(hwnd, NULL, TRUE);

    check_game_over(hwnd);
    if (!game_over) {
        if (game_mode == MODE_PVP) {
            current_player = (current_player == PLAYER_X) ? PLAYER_O : PLAYER_X;
            notify_status();
        } else {
            current_player = ai_player;
            notify_status();
            SetTimer(hwnd, AI_TIMER_ID, AI_THINK_DELAY, NULL);
        }
    }
}

void app_on_animation_timer(HWND hwnd) {
    int any_active = 0;
    for (int i = 0; i < 9; i++) {
        if (anim_frames[i] > 0) {
            anim_frames[i]++;
            if (anim_frames[i] > ANIM_FRAMES) {
                anim_frames[i] = 0;
            } else {
                any_active = 1;
            }
        }
    }
    if (!any_active)
        KillTimer(hwnd, ANIM_TIMER_ID);
    InvalidateRect(hwnd, NULL, TRUE);
}

void app_on_ai_timer(HWND hwnd) {
    KillTimer(hwnd, AI_TIMER_ID);
    make_ai_move(hwnd);
}

void app_move_cursor(int dx, int dy) {
    if (game_over)
        return;
    if (game_mode != MODE_PVP && current_player != human_player)
        return;

    int row = cursor_pos / 3;
    int col = cursor_pos % 3;

    col += dx;
    row += dy;

    if (col < 0) 
        col = 0;
    if (col > 2) 
        col = 2;
    if (row < 0) 
        row = 0;
    if (row > 2) 
        row = 2;

    cursor_pos = row * 3 + col;
}