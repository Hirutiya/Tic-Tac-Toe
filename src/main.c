#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "game.h"
#include "ui.h"
#include "ai.h"
#include "i18n.h"

#define MODE_PVP 1
#define MODE_PVE_EASY 2
#define MODE_PVE_HARD 3

static void select_language(void) {
    int choice;
    while (1) {
        printf("Select language / 選擇語言 / 选择语言:\n");
        printf("1. English\n");
        printf("2. 繁體中文\n");
        printf("3. 简体中文\n");
        printf("Choose (1/2/3): ");

        if (scanf("%d", &choice) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            printf("%s", i18n_get(STR_INVALID_LANG_CHOICE));
            continue;
        }

        if (choice < 1 || choice > 3) {
            printf("%s", i18n_get(STR_INVALID_LANG_CHOICE));
            continue;
        }

        switch (choice) {
            case 1: i18n_set_language(LANG_EN); break;
            case 2: i18n_set_language(LANG_ZH_TW); break;
            case 3: i18n_set_language(LANG_ZH_CN); break;
        }
        break;
    }
}

int main(void) {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif

    char board[9];
    char current;
    char again;
    int mode;
    char human_player = PLAYER_X;
    char ai_player = PLAYER_O;

    srand((unsigned)time(NULL));

    select_language();

    printf("%s\n", i18n_get(STR_GAME_TITLE));
    printf("%s\n", i18n_get(STR_MENU_MODE));
    printf("%s\n", i18n_get(STR_MODE_PVP));
    printf("%s\n", i18n_get(STR_MODE_PVE_EASY));
    printf("%s\n", i18n_get(STR_MODE_PVE_HARD));

    while (1) {
        printf("%s", i18n_get(STR_INPUT_MODE));
        if (scanf("%d", &mode) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            printf("%s", i18n_get(STR_INVALID_MODE_CHOICE));
            continue;
        }
        if (mode < 1 || mode > 3) {
            printf("%s", i18n_get(STR_INVALID_MODE_CHOICE));
            continue;
        }
        break;
    }

    if (mode == MODE_PVE_EASY || mode == MODE_PVE_HARD) {
        human_player = get_player_side_choice();
        ai_player = (human_player == PLAYER_X) ? PLAYER_O : PLAYER_X;
    }

    do {
        init_board(board);
        current = PLAYER_X;

        while (1) {
            print_board(board);
            printf(i18n_get(STR_TURN_PLAYER), current);

            int move;
            if ((mode == MODE_PVE_EASY || mode == MODE_PVE_HARD) && current == ai_player) {
                printf("%s", i18n_get(STR_COMPUTER_THINKING));
                if (mode == MODE_PVE_EASY) {
                    move = ai_simple_move(board, PLAYER_O, PLAYER_X);
                } else {
                    move = ai_minimax_move(board, PLAYER_O, PLAYER_X);
                }
            } else {
                move = get_move(board);
            }

            if (move < 0 || move > 8) {
                printf("%s", i18n_get(STR_AI_ERROR));
                break;
            }

            board[move] = current;

            char winner = check_winner(board);
            if (winner != EMPTY) {
                print_board(board);
                printf(i18n_get(STR_PLAYER_WINS), winner);
                break;
            }
            if (is_full(board)) {
                print_board(board);
                printf("%s", i18n_get(STR_DRAW));
                break;
            }

            current = (current == PLAYER_X) ? PLAYER_O : PLAYER_X;
        }

        printf("\n%s", i18n_get(STR_PLAY_AGAIN));
        scanf(" %c", &again);
    } while (again == 'y' || again == 'Y');

    printf("%s", i18n_get(STR_GOODBYE));
    return 0;
}