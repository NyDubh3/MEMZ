void killWindows();
void killWindowsInstant();

DWORD WINAPI watchdogThread(LPVOID);
LRESULT CALLBACK watchdogWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);