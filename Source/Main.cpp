/*
    - Autor SicdeX-
    - Created date: 05.27.2022
*/

#define WINVER 0x0500
#include <windows.h>
#include <iostream>
#include <string>
using namespace std;

string gameWindowName = "Tiger - 64"; // window game name title

int keyCode = 0x46; // https://learn.microsoft.com/ru-ru/windows/win32/inputdev/virtual-key-codes
int max_prepared_time = 2; /** sec */
int delay_check_window = max_prepared_time * 1000; /** sec */
int counts_attempts_find = 0;
int max_attempts_find_window = 10;

bool console_clear_setup = false;
bool console_clear_game_found = false;

void send_input() {
    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = keyCode;
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = keyCode;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs)) {
        printf("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    }
}

string GetActiveWindowTitle() {
    char a[256] = {};
    HWND hw = GetForegroundWindow();
    GetWindowTextA(hw, a, sizeof(a));
    return (string)a;
}

void LaunchedInfo() {
    printf("- Launched...\n");
    printf("- Use LSHIFT + F.\n");
    printf("- Use 'Escape' for exit.\n");
}

bool IsHoldKeyState() {
    return (GetAsyncKeyState(keyCode) & 0x8000) && (GetAsyncKeyState(VK_LSHIFT) & 0x8000);
}

int main() {
    printf("- Please await %ds...\n", max_prepared_time);
    Sleep(max_prepared_time * 1000);
    system("CLS");
    console_clear_setup = true;
    LaunchedInfo();

    while (true) {
        if (counts_attempts_find >= max_attempts_find_window || (GetAsyncKeyState(VK_ESCAPE) & 1)) {
            exit(0);
            break;
        }
        if (GetActiveWindowTitle().c_str() != gameWindowName) {
            if (!console_clear_game_found) {
                system("CLS");
                console_clear_game_found = true;
            }
            printf("- Game window not found, open or expand the game...\n- App will close after [%d/%d]\n", counts_attempts_find + 1, max_attempts_find_window);
            console_clear_setup = false;
            counts_attempts_find++;
            Sleep(delay_check_window);
            continue;
        }
        if (!console_clear_setup) {
            system("CLS");
            LaunchedInfo();
            console_clear_setup = true;
            console_clear_game_found = false;
            counts_attempts_find = 0;
        }
        if (IsHoldKeyState()) {
            for (size_t i = 0; i < 7; i++)
                send_input();
        }
        Sleep(1);
    }
    return 0;
}
