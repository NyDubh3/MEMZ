#define UNICODE

#include "../Build/Mode.h"

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


#include <windows.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <psapi.h>
#include <commctrl.h>

#include "../Build/Data.h"
#include "Payloads/Payloads.h"
#include "Utils/Utils.h"

#ifdef CLEAN
	#include "Clean/Clean.h"
#else
	#include "Destructive/Destructive.h"
#endif

void start();

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);