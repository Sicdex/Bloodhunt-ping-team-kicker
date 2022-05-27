/*
    - Autor SicdeX-
    - Created date: 05.27.2022
*/

#define WINVER 0x0500
#include <windows.h>
#include <iostream>
#include <string>
using namespace std;

int max_prepared_time = 5; /** sec */

int counts_attempts_find = 0;
int max_attempts_find_window = 10;

bool console_clear_setup = false;
bool console_clear_game_found = false;
string gameWindowName = "Tiger - 64";

void send_input() {

    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    // https://docs.microsoft.com/ru-ru/windows/win32/inputdev/virtual-key-codes
    ip.ki.wVk = 0x04; // MOUSE3 hotkey
    ip.ki.dwFlags = 0;
    SendInput(1, &ip, sizeof(INPUT));

    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
}

string GetActiveWindowTitle()
{
    char a[256] = {};
    HWND hw = GetForegroundWindow();
    GetWindowTextA(hw, a, sizeof(a));
    return string(a);
}

void LaunchedInfo() {
    printf("- Launched...\n");
    printf("- Use 'Escape' for exit.\n");
    printf("- if not working set key MOUSE3 (ping settings)\n");
}

int main()
{
    printf("- Please await %ds...\n", max_prepared_time);
    printf("- Set key ping MOUSE3 in game\n");

    Sleep(max_prepared_time * 1000);
    system("CLS");
    console_clear_setup = true;

    LaunchedInfo();

    while (1) {

        /** user exit */
        if (counts_attempts_find >= max_attempts_find_window || (GetAsyncKeyState(VK_ESCAPE) & 1)) {
            exit(0);
            break;
        }

        auto isFocus = GetActiveWindowTitle().compare(gameWindowName) == 1;

        if (!isFocus) {

            if (!console_clear_game_found) {
                system("CLS");
                console_clear_game_found = true;
            }

            printf("- Game window not found, open or expand the game...\n- App will close after [%d/%d]\n", counts_attempts_find + 1, max_attempts_find_window);
            console_clear_setup = false;
            counts_attempts_find++;
            Sleep(2000);
            continue;
        }

        if (!console_clear_setup) {
            system("CLS");
            LaunchedInfo();
            console_clear_setup = true;
            console_clear_game_found = false;
            counts_attempts_find = 0;
        }

        for (size_t i = 0; i < 7; i++)
            send_input();

        Sleep(1);
    }
    return 0; /** n exit*/
}