#include "ascii.h"

int ascii_wctomb(conv_t conv, unsigned char* r, ucs4_t wc, size_t n)
{
	if (wc < 0x0080)
	{
		*r = wc;
		return 1;
	}
	return RET_ILUNI;
}