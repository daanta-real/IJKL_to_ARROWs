#define UNICODE
#define _UNICODE
#include <windows.h>
#include <shellapi.h>

#define ID_TRAY_APP_ICON                1001
#define ID_TRAY_TITLE                   1002
#define ID_TRAY_GITHUB                  1003
#define ID_TRAY_EXIT_CONTEXT_MENU       1004
#define WM_TRAYICON                     (WM_USER + 1)
#define IDI_APP_ICON                    101

HHOOK hhkLowLevelKybd = NULL;
NOTIFYICONDATA nid = {0};

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;
        if (!(p->flags & LLKHF_INJECTED)) {
            DWORD newVk = 0;
            switch (p->vkCode) {
                case 'I': case 'i': newVk = VK_UP;    break;
                case 'J': case 'j': newVk = VK_LEFT;  break;
                case 'K': case 'k': newVk = VK_DOWN;  break;
                case 'L': case 'l': newVk = VK_RIGHT; break;
            }
            if (newVk != 0) {
                INPUT in = {0};
                in.type = INPUT_KEYBOARD;
                in.ki.wVk = (WORD)newVk;
                in.ki.wScan = (WORD)MapVirtualKey(newVk, MAPVK_VK_TO_VSC);
                in.ki.dwFlags = (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) ? KEYEVENTF_KEYUP : 0;
                SendInput(1, &in, sizeof(INPUT));
                return 1;
            }
        }
    }
    return CallNextHookEx(hhkLowLevelKybd, nCode, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_TRAYICON && lParam == WM_RBUTTONUP) {
        POINT curPoint;
        GetCursorPos(&curPoint);
        HMENU hMenu = CreatePopupMenu();
        AppendMenu(hMenu, MF_STRING | MF_DISABLED, ID_TRAY_TITLE, L"IJKL to ARROW KEYs");
        AppendMenu(hMenu, MF_STRING | MF_DISABLED, ID_TRAY_GITHUB, L"https://github.com/daanta-real");
        AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU, L"종료");

        SetForegroundWindow(hWnd);
        TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, curPoint.x, curPoint.y, 0, hWnd, NULL);
        DestroyMenu(hMenu);
    } else if (message == WM_COMMAND && LOWORD(wParam) == ID_TRAY_EXIT_CONTEXT_MENU) {
        PostQuitMessage(0);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 더미 윈도우 등록
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"IJKL_Remapper_Class";
    RegisterClass(&wc);
    HWND hWnd = CreateWindow(wc.lpszClassName, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, hInstance, NULL);

    // WinMain 내부의 아이콘 로드 로직을 아래로 교체하십시오
    HICON hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_APP_ICON), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

    // 리소스 로드 실패 시 파일에서 직접 로드 시도
    if (hIcon == NULL) {
        hIcon = (HICON)LoadImage(NULL, L"IJKL_2_ARROWs.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
    }

    // 그래도 실패하면 시스템 기본 아이콘
    if (hIcon == NULL) {
        hIcon = LoadIcon(NULL, IDI_APPLICATION);
    }

    // 트레이 설정
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd;
    nid.uID = ID_TRAY_APP_ICON;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = hIcon;
    lstrcpy(nid.szTip, L"IJKL to ARROW KEYs");
    Shell_NotifyIcon(NIM_ADD, &nid);

    hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance, 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hhkLowLevelKybd);
    Shell_NotifyIcon(NIM_DELETE, &nid);
    return (int)msg.wParam;
}
