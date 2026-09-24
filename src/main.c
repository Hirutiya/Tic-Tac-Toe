#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "app.h"
#include "game.h"
#include "render.h"

#define IDI_APP_ICON 201

#define ID_BTN_PVP 101
#define ID_BTN_EASY 102
#define ID_BTN_HARD 103
#define ID_BTN_RESTART 104

#define IDM_ABOUT 2001

HWND hStatus;
HFONT g_hStatusFont;
HWND hScore;
HFONT g_hScoreFont;

static void on_status_changed(const char *text) {
    SetWindowTextA(hStatus, text);
}

static void on_score_changed(int x, int o, int draw) {
    char buffer[64];
    sprintf(buffer, "X: %d    O: %d    Draw: %d", x, o, draw);
    SetWindowTextA(hScore, buffer);
}

static void show_about(HWND hwnd) {
    MessageBoxA(hwnd, "Tic-Tac-Toe v0.7\n\n"
                "A simple Tic-Tac-Toe game written in pure C.\n\n"
                "Shortcuts:\n"
                "- R: Restart\n"
                "- F1: About\n\n"
                "Author: Hirutiya\n\n"
                "Icon: Tic tac toe icons created by Magnific - Flaticon\n"
                "https://www.flaticon.com/free-icons/tic-tac-toe", "About", MB_OK | MB_ICONINFORMATION);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            hStatus = CreateWindowA("STATIC", "", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);
            hScore = CreateWindowA("STATIC", "", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);

            g_hStatusFont = CreateFontA(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Microsoft Yahei UI");
            SendMessage(hStatus, WM_SETFONT, (WPARAM)g_hStatusFont, TRUE);

            g_hScoreFont = CreateFontA(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Microsoft Yahei UI");
            SendMessage(hScore, WM_SETFONT, (WPARAM)g_hScoreFont, TRUE);

            CreateWindowA("BUTTON", "PVP", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 0, 0, 0, 0, hwnd, (HMENU)ID_BTN_PVP, NULL, NULL);
            CreateWindowA("BUTTON", "Easy Mode", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 0, 0, 0, 0, hwnd, (HMENU)ID_BTN_EASY, NULL, NULL);
            CreateWindowA("BUTTON", "Hard Mode", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 0, 0, 0, 0, hwnd, (HMENU)ID_BTN_HARD, NULL, NULL);
            CreateWindowA("BUTTON", "Restart", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0, hwnd, (HMENU)ID_BTN_RESTART, NULL, NULL);

            CheckRadioButton(hwnd, ID_BTN_PVP, ID_BTN_HARD, ID_BTN_PVP);

            HMENU hMenu = CreateMenu();
            HMENU hHelpMenu = CreatePopupMenu();
            AppendMenuA(hHelpMenu, MF_STRING, IDM_ABOUT, "About");
            AppendMenuA(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hHelpMenu, "Help");
            SetMenu(hwnd, hMenu);

            app_set_status_callback(on_status_changed);
            app_start_game(hwnd, MODE_PVP, PLAYER_X);
            app_set_status_callback(on_status_changed);
            app_set_score_callback(on_score_changed);
            return 0;
        }

        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            int btnWidth = 100;
            int btnHeight = 30;
            int gap = 10;

            int statusY = height - 125;
            int scoreY = height - 85;
            int rowY = height - 50;

            MoveWindow(hStatus, 0, statusY, width, 30, TRUE);
            MoveWindow(hScore,  0, scoreY,  width, 25, TRUE);

            int totalWidth = 4 * btnWidth + 3 * gap;
            int startX = (width - totalWidth) / 2;
            if (startX < 10)
                startX = 10;

            MoveWindow(GetDlgItem(hwnd, ID_BTN_PVP), startX, rowY, btnWidth, btnHeight, TRUE);
            MoveWindow(GetDlgItem(hwnd, ID_BTN_EASY), startX + btnWidth + gap, rowY, btnWidth, btnHeight, TRUE);
            MoveWindow(GetDlgItem(hwnd, ID_BTN_HARD), startX + 2 * (btnWidth + gap), rowY, btnWidth, btnHeight, TRUE);
            MoveWindow(GetDlgItem(hwnd, ID_BTN_RESTART), startX + 3 * (btnWidth + gap), rowY, btnWidth, btnHeight, TRUE);
            return 0;
        }

        case WM_COMMAND: {
            int id = LOWORD(wParam);
            switch (id) {
                case ID_BTN_PVP:
                    app_start_game(hwnd, MODE_PVP, PLAYER_X);
                    break;

                case ID_BTN_EASY:
                case ID_BTN_HARD: {
                    int mode = (id == ID_BTN_EASY) ? MODE_PVE_EASY : MODE_PVE_HARD;
                    char side = human_player;

                    if (game_mode == MODE_PVP) {
                        int result = MessageBoxA(hwnd,
                                                "Choose your side:\n\n"
                                                "YES = Play as X (First)\n"
                                                "NO  = Play as O (Second)", "Choose Your Side", MB_YESNO | MB_ICONQUESTION);
                        side = (result == IDYES) ? PLAYER_X : PLAYER_O;
                    }
                    app_start_game(hwnd, mode, side);
                    break;
                }

                case ID_BTN_RESTART:
                    app_start_game(hwnd, game_mode, human_player);
                    break;

                case IDM_ABOUT:
                    show_about(hwnd);
                    break;
            }
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            int offsetX, offsetY;
            render_get_offsets(hwnd, &offsetX, &offsetY);
            render_board_lines(hdc, offsetX, offsetY);
            render_marks(hdc, offsetX, offsetY);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_LBUTTONDOWN: {
            int index = render_hit_test(hwnd, LOWORD(lParam), HIWORD(lParam));
            if (index >= 0)
                app_player_move(hwnd, index);
            return 0;
        }

        case WM_MOUSEMOVE: {
            int index = render_hit_test(hwnd, LOWORD(lParam), HIWORD(lParam));
            if (index >= 0 && index != cursor_pos) {
                app_set_cursor(index);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;
        }

        case WM_KEYDOWN: {
            if (wParam == VK_F1) {
                show_about(hwnd);
            } else if (wParam == 'R' || wParam == 'r') {
                app_start_game(hwnd, game_mode, human_player);
                InvalidateRect(hwnd, NULL, TRUE);
            } else if (wParam == VK_UP) {
                app_move_cursor(0, -1);
                InvalidateRect(hwnd, NULL, TRUE);
            } else if (wParam == VK_DOWN) {
                app_move_cursor(0, 1);
                InvalidateRect(hwnd, NULL, TRUE);
            } else if (wParam == VK_LEFT) {
                app_move_cursor(-1, 0);
                InvalidateRect(hwnd, NULL, TRUE);
            } else if (wParam == VK_RIGHT) {
                app_move_cursor(1, 0);
                InvalidateRect(hwnd, NULL, TRUE);
            } else if (wParam == VK_RETURN || wParam == VK_SPACE) {
                app_player_move(hwnd, cursor_pos);
            }
            return 0;
        }

        case WM_TIMER: {
            if (wParam == ANIM_TIMER_ID) {
                app_on_animation_timer(hwnd);
            } else if (wParam == AI_TIMER_ID) {
                app_on_ai_timer(hwnd);
            }
            return 0;
        }

        case WM_GETMINMAXINFO: {
            MINMAXINFO *mmi = (MINMAXINFO *)lParam;
            RECT minRect = {0, 0, 460, BOARD_SIZE + BUTTON_AREA_HEIGHT + 50};
            AdjustWindowRect(&minRect, WS_OVERLAPPEDWINDOW, FALSE);
            mmi->ptMinTrackSize.x = minRect.right - minRect.left;
            mmi->ptMinTrackSize.y = minRect.bottom - minRect.top;
            return 0;
        }

        case WM_DESTROY:
            if (g_hStatusFont)
                DeleteObject(g_hStatusFont);
            if (g_hScoreFont)
                DeleteObject(g_hScoreFont);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)lpCmdLine;

    srand((unsigned)time(NULL));

    const char CLASS_NAME[] = "TicTacToeWindowClass";

    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_APP_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
    wc.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_APP_ICON), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_SHARED);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONERROR);
        return 0;
    }

    RECT rect = {0, 0, 700, 650};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "Tic-Tac-Toe", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

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