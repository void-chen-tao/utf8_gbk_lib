#include "ces_gbk.h"


int ces_gbk_wctomb(conv_t conv, unsigned char* r, ucs4_t wc, size_t n)
{
	//  bufÓÃÓÚ´æ´¢gbk×Ö·û
	unsigned char buf[2];
	int ret;

	/* Code set 0 (ASCII or GB 1988-89) */
	ret = ascii_wctomb(conv, r, wc, n);
	if (ret != RET_ILUNI)
		return ret;

	/* Code set 1 (GBK) */
	ret = gbk_wctomb(conv, buf, wc, 2);
	if (ret != RET_ILUNI)
	{
		if (ret != 2)
			return DECODE_ERROR;
		if (n < 2)
			return RET_TOOSMALL;
		r[0] = buf[0];
		r[1] = buf[1];
		return 2;
	}

	return RET_ILUNI;
}