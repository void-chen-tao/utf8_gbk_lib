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


int utf8_mbRead(conv_t cd, const unsigned char* s)
{
	if (!s || cd->ifuncs.xxx_mbtowc != utf8_mbtowc)
		return -1;

	int step;							//	���ڸ���ָ���utf8_remain_countʣ��Ĳ���
	int utf8_count;						//	utf8�ַ�������ͳ��
	char* move_p = s;					//	�ƶ�ָ��
	int utf8_remain_count = strlen(s);	//	��ȡstep���Ⱥ�ʣ��char�ռ�ĳ��ȼ���

	for (utf8_count = 0; *s;)
	{
		unsigned char c = s[0];

		if (c < 0x80) {
			step = 1;
		}
		else if (c < 0xc2) {
			return -1;
		}
		else if (c < 0xe0) {
			if (utf8_remain_count < 2)
				return -1;
			if (!((s[1] ^ 0x80) < 0x40))
				return -1;
			step = 2;
		}
		else if (c < 0xf0) {
			if (utf8_remain_count < 3)
				return -1;
			if (!((s[1] ^ 0x80) < 0x40 && (s[2] ^ 0x80) < 0x40
				&& (c >= 0xe1 || s[1] >= 0xa0)
				&& (c != 0xed || s[1] < 0xa0)))
				return -1;
			step = 3;
		}
		else if (c < 0xf8 && sizeof(ucs4_t) * 8 >= 32) {
			if (utf8_remain_count < 4)
				return -1;
			if (!((s[1] ^ 0x80) < 0x40 && (s[2] ^ 0x80) < 0x40
				&& (s[3] ^ 0x80) < 0x40
				&& (c >= 0xf1 || s[1] >= 0x90)
				&& (c < 0xf4 || (c == 0xf4 && s[1] < 0x90))))
				return -1;
			step = 4;
		}
		else
			return -1;

		s += step;
		utf8_remain_count -= step;
		++utf8_count;
	}

	return utf8_count;
}