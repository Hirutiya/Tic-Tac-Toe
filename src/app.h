#ifndef APP_H
#define APP_H

#include <windows.h>

#define MODE_PVP 1
#define MODE_PVE_EASY 2
#define MODE_PVE_HARD 3

#define ANIM_FRAMES 8
#define ANIM_TIMER_ID 1
#define ANIM_INTERVAL 25
#define AI_TIMER_ID 2
#define AI_THINK_DELAY 500

extern int game_mode;
extern char human_player;
extern char ai_player;
extern char board[9];
extern char current_player;
extern int game_over;
extern int win_line[3];
extern int anim_frames[9];
extern int score_x;
extern int score_o;
extern int score_draw;
extern int cursor_pos;

typedef void (*ScoreCallback)(int x, int o, int draw);

void app_set_score_callback(ScoreCallback cb);

typedef void (*StatusCallback)(const char *text);

void app_set_status_callback(StatusCallback cb);

void app_start_game(HWND hwnd, int mode, char human_side);

void app_player_move(HWND hwnd, int index);

void app_on_animation_timer(HWND hwnd);

void app_on_ai_timer(HWND hwnd);

void app_move_cursor(int dx, int dy);

#endif