DWORD WINAPI keyboardThread(LPVOID lParam);
extern BOOLEAN enablePayloads;

LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CleanWindowsProc(HWND hwnd, LPARAM lParam);