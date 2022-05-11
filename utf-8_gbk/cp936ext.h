#ifndef __CP936EXT_H__
#define __CP936EXT_H__

#include "converters.h"

/*
 * CP936 extensions
 */

static const unsigned short cp936ext_2uni_pagea6[181 - 159] = {
	/* 0xa6 */
															0xfe35,
	0xfe36, 0xfe39, 0xfe3a, 0xfe3f, 0xfe40, 0xfe3d, 0xfe3e, 0xfe41,
	0xfe42, 0xfe43, 0xfe44, 0xfffd, 0xfffd, 0xfe3b, 0xfe3c, 0xfe37,
	0xfe38, 0xfe31, 0xfffd, 0xfe33, 0xfe34,
};
static const unsigned short cp936ext_2uni_pagea8[128 - 122] = {
	/* 0xa8 */
					0x0251, 0xfffd, 0x0144, 0x0148, 0xfffd, 0x0261,
};
static const unsigned short cp936ext_page01[16] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0xa8bd, 0x0000, 0x0000, 0x0000, /*0x40-0x47*/
  0xa8be, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /*0x48-0x4f*/
};
static const unsigned short cp936ext_page02[24] = {
  0x0000, 0xa8bb, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /*0x50-0x57*/
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /*0x58-0x5f*/
  0x0000, 0xa8c0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /*0x60-0x67*/
};
static const unsigned short cp936ext_pagefe[24] = {
  0x0000, 0xa6f2, 0x0000, 0xa6f4, 0xa6f5, 0xa6e0, 0xa6e1, 0xa6f0, /*0x30-0x37*/
  0xa6f1, 0xa6e2, 0xa6e3, 0xa6ee, 0xa6ef, 0xa6e6, 0xa6e7, 0xa6e4, /*0x38-0x3f*/
  0xa6e5, 0xa6e8, 0xa6e9, 0xa6ea, 0xa6eb, 0x0000, 0x0000, 0x0000, /*0x40-0x47*/
};



#ifdef __cplusplus
extern "C" {
#endif

	int cp936ext_mbtowc(conv_t conv, ucs4_t* pwc, const unsigned char* s, size_t n);
	int cp936ext_wctomb(conv_t conv, unsigned char* r, ucs4_t wc, size_t n);
#ifdef __cplusplus
}
#endif	/*  __cplusplus  */

#endif	/*  __CP936EXT_H__  */

