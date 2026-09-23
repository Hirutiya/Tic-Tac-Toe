#include <windows.h>
#include <stdio.h>
#include "game.h"
#include "ai.h"

#define CELL_SIZE 150
#define BOARD_SIZE (3 * CELL_SIZE)
#define BUTTON_AREA_HEIGHT 120

#define ID_BTN_PVP 101
#define ID_BTN_EASY 102
#define ID_BTN_HARD 103
#define ID_BTN_X 104
#define ID_BTN_O 105
#define ID_BTN_RESTART 106

#define MODE_PVP 1
#define MODE_PVE_EASY 2
#define MODE_PVE_HARD 3

void make_ai_move(HWND hwnd);

HWND hStatus;
int game_mode = MODE_PVP;
char human_player = PLAYER_X;
char ai_player = PLAYER_O;
char board[9];
char current_player;
int game_over;
int win_line[3] = {-1, -1, -1};
HWND hStatus;
HFONT g_hStatusFont;

void update_status(void) {
    char buf[64];
    const char *test = "";
    if (game_over) {
        test = "Game Over - Press R or click Restart";
    } else {
        if (current_player == human_player || game_mode == MODE_PVP) {
            sprintf(buf, "Player %c's turn", current_player);
            test = buf;
        } else {
            test = "Computer is thinking...";
        }
    }
    SetWindowTextA(hStatus, test);
}

void init_game(HWND hwnd) {
    init_board(board);
    current_player = PLAYER_X;
    game_over = 0;
    win_line[0] = win_line[1] = win_line[2] = -1;
    update_status();

    if (game_mode != MODE_PVP && ai_player == PLAYER_X) {
        make_ai_move(hwnd);
    }
}

void check_game_over(HWND hwnd) {
    char winner = check_winner(board);
    if (winner != EMPTY) {
        game_over = 1;

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
        MessageBoxA(hwnd, "Draw!", "Game Over", MB_OK | MB_ICONINFORMATION);
    }
    update_status();
}

void make_ai_move(HWND hwnd) {
    int move = -1;
    if (game_mode == MODE_PVE_EASY) {
        move = ai_simple_move(board, ai_player, human_player);
    } else if (game_mode == MODE_PVE_HARD) {
        move = ai_minimax_move(board, ai_player, human_player);
    }

    if (move >= 0 && move < 9 && board[move] == EMPTY) {
        board[move] = ai_player;
        InvalidateRect(hwnd, NULL, TRUE);

        check_game_over(hwnd);
        if (!game_over) {
            current_player = human_player;
            update_status();
        }
    }
}

void make_player_move(HWND hwnd, int index) {
    if (game_over) 
        return;
    if (game_mode != MODE_PVP && current_player != human_player) 
        return;
    if (board[index] != EMPTY) 
        return;

    board[index] = current_player;
    InvalidateRect(hwnd, NULL, TRUE);

    check_game_over(hwnd);
    if (!game_over) {
        if (game_mode == MODE_PVP) {
            current_player = (current_player == PLAYER_X) ? PLAYER_O : PLAYER_X;
            update_status();
        } else {
            current_player = ai_player;
            update_status();
            make_ai_move(hwnd);
        }
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

    if (game_over && win_line[0] != -1) {
        HBRUSH hHightlight = CreateSolidBrush(RGB(120, 230, 120));
        for (i = 0; i < 3; i++) {
            int index = win_line[i];
            int row = index / 3;
            int col = index % 3;
            int x = offsetX + col * CELL_SIZE;
            int y = offsetY + row * CELL_SIZE;

            RECT r = {x + 1, y + 1, x + CELL_SIZE - 1, y + CELL_SIZE - 1};
            FillRect(hdc, &r, hHightlight);
        }
        DeleteObject(hHightlight);
    }

    for (i = 0; i < 9; i++) {
        if (board[i] == ' ')
            continue;

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
        case WM_CREATE: {
            hStatus = CreateWindowA("STATIC", "", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);

            g_hStatusFont = CreateFontA(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Microsoft Yahei UI");
            SendMessage(hStatus, WM_SETFONT, (WPARAM)g_hStatusFont, TRUE);

            CreateWindowA("BUTTON", "PVP", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 0, 0, 0, 0, hwnd, (HMENU)ID_BTN_PVP, NULL, NULL);
            CreateWindowA("BUTTON", "Easy Mode", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 0, 0, 0, 0, hwnd, (HMENU)ID_BTN_EASY, NULL, NULL);
            CreateWindowA("BUTTON", "Hard Mode", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 0, 0, 0, 0, hwnd, (HMENU)ID_BTN_HARD, NULL, NULL);
            CreateWindowA("BUTTON", "Player X", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 0, 0, 0, 0, hwnd, (HMENU)ID_BTN_X, NULL, NULL);
            CreateWindowA("BUTTON", "Player O", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 0, 0, 0, 0, hwnd, (HMENU)ID_BTN_O, NULL, NULL);
            CreateWindowA("BUTTON", "Restart", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0, hwnd, (HMENU)ID_BTN_RESTART, NULL, NULL);

            CheckRadioButton(hwnd, ID_BTN_PVP, ID_BTN_HARD, ID_BTN_PVP);
            CheckRadioButton(hwnd, ID_BTN_X, ID_BTN_O, ID_BTN_X);

            init_game(hwnd);
            return 0;
        }

        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            int btnWidth = 100;
            int btnHeight = 30;
            int gap = 10;

            MoveWindow(hStatus, 0, height - BUTTON_AREA_HEIGHT - 30, width, 30, TRUE);

            int row1y = height - BUTTON_AREA_HEIGHT + 10;
            int row1TotalWidth = 3 * btnWidth + 2 * gap;
            int row1StartX = (width - row1TotalWidth) / 2;
            if (row1StartX < 10)
                row1StartX = 10;

            MoveWindow(GetDlgItem(hwnd, ID_BTN_PVP), row1StartX, row1y, btnWidth, btnHeight, TRUE);
            MoveWindow(GetDlgItem(hwnd, ID_BTN_EASY), row1StartX + btnWidth + gap, row1y, btnWidth, btnHeight, TRUE);
            MoveWindow(GetDlgItem(hwnd, ID_BTN_HARD), row1StartX + 2 * (btnWidth + gap), row1y, btnWidth, btnHeight, TRUE);

            int row2y = row1y + btnHeight + gap;
            int row2TotalWidth = 3 * btnWidth + 2 * gap;
            int row2StartX = (width - row2TotalWidth) / 2;
            if (row2StartX < 10)
                row2StartX = 10;

            MoveWindow(GetDlgItem(hwnd, ID_BTN_X), row2StartX, row2y, btnWidth, btnHeight, TRUE);
            MoveWindow(GetDlgItem(hwnd, ID_BTN_O), row2StartX + btnWidth + gap, row2y, btnWidth, btnHeight, TRUE);
            MoveWindow(GetDlgItem(hwnd, ID_BTN_RESTART), row2StartX + 2 * (btnWidth + gap), row2y, btnWidth, btnHeight, TRUE);

            return 0;
        }

        case WM_COMMAND: {
            int id = LOWORD(wParam);
            switch (id) {
                case ID_BTN_PVP:
                    game_mode = MODE_PVP;
                    init_game(hwnd);
                    break;
                case ID_BTN_EASY:
                    game_mode = MODE_PVE_EASY;
                    init_game(hwnd);
                    break;
                case ID_BTN_HARD:
                    game_mode = MODE_PVE_HARD;
                    init_game(hwnd);
                    break;
                case ID_BTN_X:
                    human_player = PLAYER_X;
                    ai_player = PLAYER_O;
                    init_game(hwnd);
                    break;
                case ID_BTN_O:
                    human_player = PLAYER_O;
                    ai_player = PLAYER_X;
                    init_game(hwnd);
                    break;
                case ID_BTN_RESTART:
                    init_game(hwnd);
                    break;
            }
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int availHeight = clientRect.bottom - clientRect.top - BUTTON_AREA_HEIGHT;
            int offsetX = ((clientRect.right - clientRect.left) - BOARD_SIZE) / 2;
            int offsetY = (availHeight - BOARD_SIZE) / 2;

            DrawBoardLines(hdc, offsetX, offsetY);
            DrawMarks(hdc, offsetX, offsetY);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_LBUTTONDOWN: {
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int availHeight = clientRect.bottom - clientRect.top - BUTTON_AREA_HEIGHT;
            int offsetX = ((clientRect.right - clientRect.left) - BOARD_SIZE) / 2;
            int offsetY = (availHeight - BOARD_SIZE) / 2;

            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);
            int col = (mouseX - offsetX) / CELL_SIZE;
            int row = (mouseY - offsetY) / CELL_SIZE;

            if (col >= 0 && col < 3 && row >= 0 && row < 3)
                make_player_move(hwnd, row * 3 + col);
            return 0;
        }

        case WM_KEYDOWN: {
            if (wParam == 'R' || wParam == 'r') {
                init_game(hwnd);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;
        }

        case WM_GETMINMAXINFO: {
            MINMAXINFO *mmi = (MINMAXINFO *)lParam;
            RECT minRect = {0, 0, 400, BOARD_SIZE + BUTTON_AREA_HEIGHT + 50};
            AdjustWindowRect(&minRect, WS_OVERLAPPEDWINDOW, FALSE);
            mmi->ptMinTrackSize.x = minRect.right - minRect.left;
            mmi->ptMinTrackSize.y = minRect.bottom - minRect.top;
            return 0;
        }

        case WM_DESTROY:
            if (g_hStatusFont)
                DeleteObject(g_hStatusFont);
            PostQuitMessage(0);
            return 0;
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

    RECT rect = {0, 0, 700, 650};
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