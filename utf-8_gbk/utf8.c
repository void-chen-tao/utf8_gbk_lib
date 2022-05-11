#include "utf8.h"

int utf8_mbtowc(conv_t conv, ucs4_t* pwc, const unsigned char* s, size_t n)
{
	unsigned char c = s[0];

	if (c < 0x80) {
		*pwc = c;
		return 1;
	}
	else if (c < 0xc2) {
		return RET_ILSEQ;
	}
	else if (c < 0xe0) {
		if (n < 2)
			return RET_TOOFEW(0);
		if (!((s[1] ^ 0x80) < 0x40))
			return RET_ILSEQ;
		*pwc = ((ucs4_t)(c & 0x1f) << 6)
			| (ucs4_t)(s[1] ^ 0x80);
		return 2;
	}
	else if (c < 0xf0) {
		if (n < 3)
			return RET_TOOFEW(0);
		if (!((s[1] ^ 0x80) < 0x40 && (s[2] ^ 0x80) < 0x40
			&& (c >= 0xe1 || s[1] >= 0xa0)
			&& (c != 0xed || s[1] < 0xa0)))
			return RET_ILSEQ;
		*pwc = ((ucs4_t)(c & 0x0f) << 12)
			| ((ucs4_t)(s[1] ^ 0x80) << 6)
			| (ucs4_t)(s[2] ^ 0x80);
		return 3;
	}
	else if (c < 0xf8 && sizeof(ucs4_t) * 8 >= 32) {
		if (n < 4)
			return RET_TOOFEW(0);
		if (!((s[1] ^ 0x80) < 0x40 && (s[2] ^ 0x80) < 0x40
			&& (s[3] ^ 0x80) < 0x40
			&& (c >= 0xf1 || s[1] >= 0x90)
			&& (c < 0xf4 || (c == 0xf4 && s[1] < 0x90))))
			return RET_ILSEQ;
		*pwc = ((ucs4_t)(c & 0x07) << 18)
			| ((ucs4_t)(s[1] ^ 0x80) << 12)
			| ((ucs4_t)(s[2] ^ 0x80) << 6)
			| (ucs4_t)(s[3] ^ 0x80);
		return 4;
	}
	else
		return RET_ILSEQ;
}