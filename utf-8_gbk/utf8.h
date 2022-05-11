#ifndef __UTF8_H__
#define __UTF8_H__

#include "converters.h"


#ifdef __cplusplus
extern "C" {
#endif
	/*
	*   utf8多字节转换为unicode宽字节
	*
	*   参数：
	*       conv        转换器结构体
	*       pwc         宽字节(unsigned int)的地址
	*       s           需要转换的字符串
	*       n           需要转换的字符串的长度——strlen(s)
	*
	*   返回值：
	*       如果输入的是有效的utf-8字符串，返回该utf-8字符的字节长度
	*       无效的utf-8编码，返回-1；长度不匹配，返回-2；
	*/
	int utf8_mbtowc(conv_t conv, ucs4_t* pwc, const unsigned char* s, size_t n);

	/*
	*	该函数用于获取输入的utf8字符串由多少个字符组成
	*
	*	参数：
	*		cd			转换器结构体
	*		s			utf8字符串
	*
	*	返回值：
	*		计算成功	大于等于0的字符串
	*		计算失败	小于0
	*
	*/
	int utf8_mbRead(conv_t cd, const unsigned char* s);

#ifdef __cplusplus
}
#endif	/*  __cplusplus  */

#endif	/*  __UTF8_H__  */

