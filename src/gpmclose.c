#define INCL_WIN
#define INCL_WINWINDOWMGR
#include <os2.h>
#include <string.h>

int main(void)
{
    HAB  hab;
    HMQ  hmq;
    HWND hwnd;

    hab = WinInitialize(0);
    if (hab == 0)
        return 2;
    hmq = WinCreateMsgQueue(hab, 0);

    hwnd = WinQueryWindow(HWND_DESKTOP, QW_TOP);
    while (hwnd != NULLHANDLE) {
        char szText[128];
        if (WinQueryWindowText(hwnd, sizeof(szText) - 1, szText) > 0) {
            szText[sizeof(szText) - 1] = '\0';
            if (strstr(szText, "Maze") != NULL) {
                WinPostMsg(hwnd, WM_CLOSE, 0L, 0L);
                break;
            }
        }
        hwnd = WinQueryWindow(hwnd, QW_NEXT);
    }

    WinDestroyMsgQueue(hmq);
    WinTerminate(hab);
    return 0;
}