#ifndef I18N_H
#define I18N_H

typedef enum {
    LANG_EN = 0,
    LANG_ZH_TW,
    LANG_ZH_CN,
    LANG_COUNT
} Language;

typedef enum {
    STR_GAME_TITLE = 0,
    STR_MENU_MODE,
    STR_MODE_PVP,
    STR_MODE_PVE,
    STR_INPUT_MODE,
    STR_TURN_PLAYER,
    STR_COMPUTER_THINKING,
    STR_INPUT_POSITION,
    STR_INVALID_NUMBER,
    STR_OUT_OF_RANGE,
    STR_OCCUPIED,
    STR_PLAYER_WINS,
    STR_DRAW,
    STR_AI_ERROR,
    STR_PLAY_AGAIN,
    STR_GOODBYE,
    STR_INVALID_LANG_CHOICE,
    STR_INVALID_MODE_CHOICE,
    STR_COUNT
} StringID;

void i18n_set_language(Language lang);

const char *i18n_get(StringID id);

#endif