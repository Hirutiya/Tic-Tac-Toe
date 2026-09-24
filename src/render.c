#include "render.h"
#include "app.h"
#include "game.h"

void render_get_offsets(HWND hwnd, int *offsetX, int *offsetY) {
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int availHeight = clientRect.bottom - clientRect.top - BUTTON_AREA_HEIGHT;
    *offsetX = ((clientRect.right - clientRect.left) - BOARD_SIZE) / 2;
    *offsetY = (availHeight - BOARD_SIZE) / 2;
}

int render_hit_test(HWND hwnd, int mouseX, int mouseY) {
    int offsetX, offsetY;
    render_get_offsets(hwnd, &offsetX, &offsetY);

    int col = (mouseX - offsetX) / CELL_SIZE;
    int row = (mouseY - offsetY) / CELL_SIZE;

    if (col < 0 || col >= 3 || row < 0 || row >= 3)
        return -1;
    return row * 3 + col;
}

void render_board_lines(HDC hdc, int offsetX, int offsetY) {
    HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    HPEN hOldPen = SelectObject(hdc, hPen);

    for (int i = 1; i <= 2; i++) {
        MoveToEx(hdc, offsetX + i * CELL_SIZE, offsetY, NULL);
        LineTo(hdc, offsetX + i * CELL_SIZE, offsetY + BOARD_SIZE);
    }
    for (int i = 1; i <= 2; i++) {
        MoveToEx(hdc, offsetX, offsetY + i * CELL_SIZE, NULL);
        LineTo(hdc, offsetX + BOARD_SIZE, offsetY + i * CELL_SIZE);
    }

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void render_marks(HDC hdc, int offsetX, int offsetY) {
    if (game_over && win_line[0] != -1) {
        HBRUSH hHighlight = CreateSolidBrush(RGB(120, 230, 120));
        for (int i = 0; i < 3; i++) {
            int index = win_line[i];
            int row = index / 3;
            int col = index % 3;
            int x = offsetX + col * CELL_SIZE;
            int y = offsetY + row * CELL_SIZE;
            RECT r = {x + 1, y + 1, x + CELL_SIZE - 1, y + CELL_SIZE - 1};
            FillRect(hdc, &r, hHighlight);
        }
        DeleteObject(hHighlight);
    }

    for (int i = 0; i < 9; i++) {
        if (board[i] == EMPTY)
            continue;

        int row = i / 3;
        int col = i % 3;
        int cx = offsetX + col * CELL_SIZE + CELL_SIZE / 2;
        int cy = offsetY + row * CELL_SIZE + CELL_SIZE / 2;

        int half = CELL_SIZE / 2 - 20;
        if (anim_frames[i] > 0) {
            int progress = anim_frames[i] * 100 / ANIM_FRAMES;
            half = half * (30 + 70 * progress / 100) / 100;
        }

        int left = cx - half;
        int top = cy - half;
        int right = cx + half;
        int bottom = cy + half;

        if (board[i] == PLAYER_X) {
            HPEN hPen = CreatePen(PS_SOLID, 5, RGB(200, 0, 0));
            HPEN hOldPen = SelectObject(hdc, hPen);
            MoveToEx(hdc, left, top, NULL);
            LineTo(hdc, right, bottom);
            MoveToEx(hdc, right, top, NULL);
            LineTo(hdc, left, bottom);
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
        } else if (board[i] == PLAYER_O) {
            HPEN hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 200));
            HPEN hOldPen = SelectObject(hdc, hPen);
            HBRUSH hOldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Ellipse(hdc, left, top, right, bottom);
            SelectObject(hdc, hOldBrush);
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
        }
    }

    if (!game_over && (game_mode == MODE_PVP || current_player == human_player)) {
        int row = cursor_pos / 3;
        int col = cursor_pos % 3;
        int x = offsetX + col * CELL_SIZE;
        int y = offsetY + row * CELL_SIZE;

        HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0, 150, 255));
        HPEN hOldPen = SelectObject(hdc, hPen);
        HBRUSH hOldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, x + 5, y + 5, x + CELL_SIZE - 5, y + CELL_SIZE - 5);
        SelectObject(hdc, hOldBrush);
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }
}