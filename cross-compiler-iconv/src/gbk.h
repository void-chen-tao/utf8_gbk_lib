#ifndef __GBK_H__
#define __GBK_H__

#include "converters.h"
#include "gb2312.h"
#include "gbkext_inv.h"
#include "cp936ext.h"

#define DECODE_ERROR	-4

#ifdef __cplusplus
extern "C" {
#endif
	/*
	*   将unicode转换为gbk编码
	*
	*   参数：
	*       conv        转换器结构体
	*       r           一个gbk字符的存储空间(2个char)
	*       wc          一个unicode编码的字符
	*       n           2
	*
	*	返回值：
	*		将uniocde转换为gbk，如果转换成功，将返回2个字节——即gbk大小
	*		如果该编码不存在，返回-1；如果输出缓冲区太小，返回-2；
	*/
	int gbk_wctomb(conv_t conv, unsigned char* r, ucs4_t wc, size_t n);

#ifdef __cplusplus
}
#endif	/*  __cplusplus  */

#endif	/*  __GBK_H__  */

