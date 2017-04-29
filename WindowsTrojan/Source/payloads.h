#pragma once
#include "memz.h"

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
#define PAYLOADFUNCTIONDEFAULT(name) int name (int times, int runtime, BOOLEAN skip)
#define PAYLOADFUNCTIONVISUAL(name) int name (int times, int runtime, BOOLEAN skip, HWND hwnd, HDC hdc, LPRECT rekt, int w, int h)
#define PAYLOADHEAD if (skip) goto out;
#else
#define PAYLOADFUNCTIONDEFAULT(name) int name (int times, int runtime)
#define PAYLOADFUNCTIONVISUAL(name) int name (int times, int runtime, HWND hwnd, HDC hdc, LPRECT rekt, int w, int h)
#define PAYLOADHEAD
#endif

extern PAYLOAD payloads[];
extern const size_t nPayloads;