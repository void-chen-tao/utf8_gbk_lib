#ifndef __ASCII_H__
#define __ASCII_H__

#include "converters.h"


#ifdef __cplusplus
extern "C" {
#endif
	/*
	*   判断是否为以unicode编码方式的ascii码。
	*
	*   参数：
	*       conv        转换器结构体
	*       r           gbk缓冲区
	*       wc          一个unicode字符
	*       n           gbk缓冲区大小
	*
	*   返回值：
	*       如果是unicode编码的ascii码，返回1；否则返回-1。
	*
	*/
	int ascii_wctomb(conv_t conv, unsigned char* r, ucs4_t wc, size_t n);

#ifdef __cplusplus
}
#endif	/*  __cplusplus  */

#endif	/*  __ASCII_H__  */