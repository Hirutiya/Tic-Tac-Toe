#ifndef RENDER_H
#define RENDER_H

#include <windows.h>

#define CELL_SIZE 150
#define BOARD_SIZE (3 * CELL_SIZE)
#define BUTTON_AREA_HEIGHT 150

void render_get_offsets(HWND hwnd, int *offsetX, int *offsetY);

int render_hit_test(HWND hwnd, int mouseX, int mouseY);

void render_board_lines(HDC hdc, int offsetX, int offsetY);

void render_marks(HDC hdc, int offsetX, int offsetY);

#endif