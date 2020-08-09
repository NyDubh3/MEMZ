#include "../MEMZ.h"

#ifdef CLEAN
HWND mainWindow;
HFONT font;
HWND dialog;

void start() {
	InitCommonControls();

	dialog = NULL;

	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	font = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	WNDCLASSEXW c;
	c.cbSize = sizeof(WNDCLASSEX);
	c.lpfnWndProc = mainWindowProc;
	c.lpszClassName = L"MEMZPanel";
	c.style = CS_HREDRAW | CS_VREDRAW;
	c.cbClsExtra = 0;
	c.cbWndExtra = 0;
	c.hInstance = NULL;
	c.hIcon = 0;
	c.hCursor = 0;
	c.hbrBackground = (HBRUSH)(COLOR_3DFACE+1);
	c.lpszMenuName = NULL;
	c.hIconSm = 0;

	RegisterClassExW(&c);

	RECT rect;
	rect.left = 0;
	rect.right = WINDOWWIDTH;
	rect.top = 0;
	rect.bottom = WINDOWHEIGHT;

	AdjustWindowRect(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);

	mainWindow = CreateWindowExW(0, L"MEMZPanel", L"MEMZ Clean Version - Payload Panel", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		50, 50, rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, GetModuleHandle(NULL), NULL);

	for (int p = 0; p < nPayloads; p++) {
		payloads[p].btn = CreateWindowW(L"BUTTON", payloads[p].name, (p==0?WS_GROUP:0) | WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHLIKE | BS_AUTOCHECKBOX | BS_NOTIFY,
			(p%COLUMNS)*BTNWIDTH+SPACE*(p%COLUMNS+1), (p/COLUMNS)*BTNHEIGHT + SPACE*(p/COLUMNS+1), BTNWIDTH, BTNHEIGHT,
			mainWindow, NULL, (HINSTANCE)GetWindowLong(mainWindow, GWL_HINSTANCE), NULL);
		SendMessage(payloads[p].btn, WM_SETFONT, (WPARAM)font, TRUE);

		CreateThread(NULL, NULL, payloads[p].payloadHost, &payloads[p], NULL, NULL);
		//CreateThread(NULL, NULL, &payloadThread, &payloads[p], NULL, NULL);
	}

	SendMessage(mainWindow, WM_SETFONT, (WPARAM)font, TRUE);

	ShowWindow(mainWindow, SW_SHOW);
	UpdateWindow(mainWindow);
	
	CreateThread(NULL, NULL, &keyboardThread, NULL, NULL, NULL);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		if (dialog == NULL || !IsDialogMessage(dialog, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;
	
	if (msg == WM_ACTIVATE) {
		if (wParam == NULL)
			dialog = NULL;
		else
			dialog = hwnd;
	} else if (msg == WM_DESTROY) {
		ExitProcess(0);
	} else if (msg == WM_COMMAND) {
		if (wParam == BN_CLICKED && SendMessage((HWND)lParam, BM_GETCHECK, 0, NULL) == BST_CHECKED) {
			for (int p = 0; p < nPayloads; p++) {
				if (payloads[p].btn == (HWND)lParam && !payloads[p].safe) {
					SendMessage((HWND)lParam, BM_SETCHECK, BST_UNCHECKED, NULL);
					// Most ugly formatting EVER
					if (MessageBoxA(hwnd,
						"This payload is considered semi-harmful.\r\nThis means, it should be safe to use, but can still cause data loss or other things you might not want.\r\n\r\n\
If you have productive data on your system or signed in to online accounts, it is recommended to run this payload inside a \
virtual machine in order to prevent potential data loss or changed things you might not want.\r\n\r\n\
Do you still want to enable it?",
"MEMZ", MB_YESNO | MB_ICONWARNING) == IDYES) {
						SendMessage((HWND)lParam, BM_SETCHECK, BST_CHECKED, NULL);
					}
				}
			}
		}
	} else if (msg == WM_PAINT) {
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, font);
		LPWSTR str;
		LPWSTR state = enablePayloads ? L"ENABLED" : L"DISABLED";
		FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY,
			L"Payloads are currently %1. Press SHIFT+ESC to toggle all payloads!", 0, 0, (LPWSTR)&str, 1024, (va_list*)&state);

		TextOut(hdc, 10, WINDOWHEIGHT - 36, str, lstrlen(str));
		TextOut(hdc, 10, WINDOWHEIGHT - 20, L"Press CTRL+SHIFT+S to skip some time (makes some payloads faster)", 65);

		EndPaint(hwnd, &ps);
	} else {
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

DWORD WINAPI keyboardThread(LPVOID lParam) {
	for (;;) {
		if ((GetKeyState(VK_SHIFT) & GetKeyState(VK_ESCAPE)) & 0x8000) {
			enablePayloads = !enablePayloads;

			if (!enablePayloads) {
				RECT rect;
				HWND desktop = GetDesktopWindow();
				GetWindowRect(desktop, &rect);

				RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);

				EnumWindows(&CleanWindowsProc, NULL);
			} else {
				RedrawWindow(mainWindow, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
			}

			while ((GetKeyState(VK_SHIFT) & GetKeyState(VK_ESCAPE)) & 0x8000) {
				Sleep(100);
			}
		} else if ((GetKeyState(VK_SHIFT) & GetKeyState(VK_CONTROL) & GetKeyState('S')) & 0x8000) {
			if (enablePayloads) {
				for (int p = 0; p < nPayloads; p++) {
					if (SendMessage(payloads[p].btn, BM_GETCHECK, 0, NULL) == BST_CHECKED) {
						payloads[p].delay = ((PAYLOADFUNCTIONDEFAULT((*)))payloads[p].payloadFunction)(payloads[p].times++, payloads[p].runtime += payloads[p].delay, TRUE);
					}
				}
			}
		}

		Sleep(10);
	}

	return 0;
}

BOOL CALLBACK CleanWindowsProc(HWND hwnd, LPARAM lParam) {
	DWORD pid;
	if (GetWindowThreadProcessId(hwnd, &pid) && pid == GetCurrentProcessId() && hwnd != mainWindow) {
		SendMessage(hwnd, WM_CLOSE, 0, 0);
	}
	return TRUE;
}
#endif