#include <windows.h>
#include <stdio.h>
#include "game.h"
#include "ai.h"

#define CELL_SIZE 150
#define BOARD_SIZE (3 * CELL_SIZE)

char board[9];
char current_player;
int game_over;

void init_game() {
    init_board(board);
    current_player = PLAYER_X;
    game_over = 0;
}

void check_game_over(HWND hwnd) {
    char winner = check_winner(board);
    if (winner != EMPTY) {
        game_over = 1;
        char msg[64];
        sprintf(msg, "Player %c wins!", winner);
        MessageBoxA(hwnd, msg, "Game Over", MB_OK | MB_ICONINFORMATION);
    } else if (is_full(board)) {
        game_over = 1;
        MessageBoxA(hwnd, "Draw!", "Game Over", MB_OK | MB_ICONINFORMATION);
    }
}

void make_ai_move(HWND hwnd) {
    int move = ai_simple_move(board, PLAYER_O, PLAYER_X);

    if (move >= 0 && move < 9 && board[move] == EMPTY) {
        board[move] = PLAYER_O;
        InvalidateRect(hwnd, NULL, TRUE);

        check_game_over(hwnd);
        if (!game_over) {
            current_player = PLAYER_X;
        }
    }
}

void make_player_move(HWND hwnd, int index) {
    if (game_over || current_player != PLAYER_X) return;
    if (board[index] != EMPTY) return;

    board[index] = PLAYER_X;
    InvalidateRect(hwnd, NULL, TRUE);

    check_game_over(hwnd);
    if (!game_over) {
        current_player = PLAYER_O;
        make_ai_move(hwnd);
    }
}

void DrawBoardLines(HDC hdc, int offsetX, int offsetY) {
    int i;
    HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    HPEN hOldPen = SelectObject(hdc, hPen);

    for (i = 1; i <= 2; i++) {
        MoveToEx(hdc, offsetX + i * CELL_SIZE, offsetY, NULL);
        LineTo(hdc, offsetX + i * CELL_SIZE, offsetY + BOARD_SIZE);
    }

    for (i = 1; i <= 2; i++) {
        MoveToEx(hdc, offsetX, offsetY + i * CELL_SIZE, NULL);
        LineTo(hdc, offsetX + BOARD_SIZE, offsetY + i * CELL_SIZE);
    }

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void DrawMarks(HDC hdc, int offsetX, int offsetY) {
    int i;
    for (i = 0; i < 9; i++) {
        if (board[i] == ' ') continue;

        int row = i / 3;
        int col = i % 3;
        int x = offsetX + col * CELL_SIZE;
        int y = offsetY + row * CELL_SIZE;

        if (board[i] == PLAYER_X) {
            HPEN hPen = CreatePen(PS_SOLID, 5, RGB(200, 0, 0));
            HPEN hOldPen = SelectObject(hdc, hPen);
            MoveToEx(hdc, x + 20, y + 20, NULL);
            LineTo(hdc, x + CELL_SIZE - 20, y + CELL_SIZE - 20);
            MoveToEx(hdc, x + CELL_SIZE - 20, y + 20, NULL);
            LineTo(hdc, x + 20, y + CELL_SIZE - 20);
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
        } else if (board[i] == PLAYER_O) {
            HPEN hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 200));
            HPEN hOldPen = SelectObject(hdc, hPen);
            HBRUSH hOldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Ellipse(hdc, x + 20, y + 20, x + CELL_SIZE - 20, y + CELL_SIZE - 20);
            SelectObject(hdc, hOldBrush);
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
        }
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: init_game(); return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int offsetX = ((clientRect.right - clientRect.left) - BOARD_SIZE) / 2;
            int offsetY = ((clientRect.bottom - clientRect.top) - BOARD_SIZE) / 2;

            DrawBoardLines(hdc, offsetX, offsetY);
            DrawMarks(hdc, offsetX, offsetY);

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_LBUTTONDOWN: {
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int offsetX = ((clientRect.right - clientRect.left) - BOARD_SIZE) / 2;
            int offsetY = ((clientRect.bottom - clientRect.top) - BOARD_SIZE) / 2;

            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);
            int col = (mouseX - offsetX) / CELL_SIZE;
            int row = (mouseY - offsetY) / CELL_SIZE;

            if (col >= 0 && col < 3 && row >= 0 && row < 3) {
                make_player_move(hwnd, row * 3 + col);
            }
            return 0;
        }
        case WM_KEYDOWN: {
            if (wParam == 'R') {
                init_game();
                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;
        }
        case WM_GETMINMAXINFO: {
            MINMAXINFO *mmi = (MINMAXINFO *)lParam;
            RECT minRect = {0, 0, BOARD_SIZE + 20, BOARD_SIZE + 20};
            AdjustWindowRect(&minRect, WS_OVERLAPPEDWINDOW, FALSE);
            mmi->ptMinTrackSize.x = minRect.right - minRect.left;
            mmi->ptMinTrackSize.y = minRect.bottom - minRect.top;
            return 0;
        }
        case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)lpCmdLine;

    const char CLASS_NAME[] = "TicTacToeWindowClass";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONERROR);
        return 0;
    }

    RECT rect = {0, 0, 500, 500};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Tic-Tac-Toe",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    SetForegroundWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}