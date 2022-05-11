#ifndef __CES_GBK_H__
#define __CES_GBK_H__

#include "converters.h"
#include "ascii.h"
#include "gbk.h"


#ifdef __cplusplus
extern "C" {
#endif
	/*
	*
	*
	*   参数：
	*       conv        转化器结构体
	*       r           gbk缓冲区
	*       wc          一个unicode字符
	*       n           gbk缓冲区大小
	*
	*   返回值：
	*       转换成功，返回1或者2。ascii表示1、gbk表示2
	*       转换失败，返回-1或者-2。-1表示编码转换失败、-2表示剩余空间不足
	*
	*/
	int ces_gbk_wctomb(conv_t conv, unsigned char* r, ucs4_t wc, size_t n);

#ifdef __cplusplus
}
#endif	/*  __cplusplus  */

#endif	/*  __CES_GBK_H__  */

