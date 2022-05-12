/*
* 该文件定义了具体实现的接口以及依赖的类型
*/
#ifndef CONVERTERS_H
#define CONVERTERS_H

/*====================================================
*				converters.h文件的依赖项
*====================================================*/
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>



/*====================================================
*				具体转换函数依赖的类型
*====================================================*/
/*  自定义宽字符类型  */
typedef unsigned int ucs4_t;
/*  转换器状态类型  */
typedef int state_t;
/*  转换器结构体  */
typedef struct conv_struct* conv_t;

/*
 * Data type for conversion multibyte -> unicode
 * 接口定义
 */
struct mbtowc_funcs {
	/*  
	*	所有的多字节编码转换为宽字节编码函数的接口模板
	*	这儿主要是指向utf8_mbtowc函数
	* 
	*	参数：
	*		conv_t					转换器结构体指针
	*		ucs4_t*					转换成功后宽字节的存储地址
	*		unisgned char const*	待转换的多字节编码字符串
	*		size_t					strlen(s)
	* 
	*	返回值：
	*		如果成功转换为一个宽字节，返回多少个多字节编码字符串构成了该宽字符
	*		——如："中"的utf8编码是：E4 B8 AD转换成功后将返回3。因为该字符由3
	*		个char构成。
	*		如果需要转换的编码是无效的编码方式，将返回-1；
	*		如果字符长度与strlen(s)不匹配，将返回-2；
	*/
	int (*xxx_mbtowc) (conv_t conv, ucs4_t* pwc, unsigned char const* s, size_t n);
	/*
	 * int xxx_mbtowc (conv_t conv, ucs4_t *pwc, unsigned char const *s, size_t n)
	 * converts the byte sequence starting at s to a wide character. Up to n bytes
	 * are available at s. n is >= 1.
	 * Result is number of bytes consumed (if a wide character was read),
	 * or -1 if invalid, or -2 if n too small,
	 * or RET_SHIFT_ILSEQ(number of bytes consumed) if invalid input after a shift
	 * sequence was read,
	 * or RET_TOOFEW(number of bytes consumed) if only a shift sequence was read.
	 */
	int (*xxx_flushwc) (conv_t conv, char* pwc);
	//int (*xxx_flushwc) (conv_t conv, ucs4_t* pwc);
	/*
	 * int xxx_flushwc (conv_t conv, ucs4_t *pwc)
	 * returns to the initial state and stores the pending wide character, if any.
	 * Result is 1 (if a wide character was read) or 0 if none was pending.
	 */
};

/*
*	结构体mbtowc_funcs成员的错误信息宏
*/
/*  Return code if invalid input after a shift sequence of n bytes was read.(xxx_mbtowc)  */
#define RET_SHIFT_ILSEQ(n)  (-1-2*(n))
   /* Return code if invalid. (xxx_mbtowc) */
#define RET_ILSEQ           RET_SHIFT_ILSEQ(0)
/* Return code if only a shift sequence of n bytes was read. (xxx_mbtowc) */
#define RET_TOOFEW(n)       (-2-2*(n))
/* Retrieve the n from the encoded RET_... value. */
#define DECODE_SHIFT_ILSEQ(r)  ((unsigned int)(RET_SHIFT_ILSEQ(0) - (r)) / 2)
#define DECODE_TOOFEW(r)       ((unsigned int)(RET_TOOFEW(0) - (r)) / 2)
/* Maximum value of n that may be used as argument to RET_SHIFT_ILSEQ or RET_TOOFEW. */
#define RET_COUNT_MAX       ((INT_MAX / 2) - 1)


/*
 *	Converter's Application Task struct
 *	转换器的任务函数
 */
struct loop_funcs {
	bool(*loop_convert) (conv_t cd, const char** inbuf, size_t* inbytesleft, char** outbuf, size_t* outbytesleft);
	bool(*loop_reset) (conv_t cd, char** outbuf, size_t* outbytesleft);
	//bool(*loop_reset)(conv_t cd);
};


/*
 * Data type for conversion unicode -> multibyte
 */
struct wctomb_funcs {
	/*
	*
	*	单个宽字节编码转换为多字节编码函数的接口模板
	*	这儿主要是gbk_wctomb函数
	*
	*		参数：
	*			conv		转换器结构体
	*			r			gbk存储空间
	*			wc			需要转换的单个unicode编码字符
	*			n			2——因为gbk编码是使用2个char来存储的编码方式
	*
	*/
	int (*xxx_wctomb) (conv_t conv, unsigned char* r, ucs4_t wc, size_t n);
	/*
	 * int xxx_wctomb (conv_t conv, unsigned char *r, ucs4_t wc, size_t n)
	 * converts the wide character wc to the character set xxx, and stores the
	 * result beginning at r. Up to n bytes may be written at r. n is >= 1.
	 * Result is number of bytes written, or -1 if invalid, or -2 if n too small.
	 */
	int (*xxx_reset) (conv_t conv, unsigned char* r, size_t n);
	/*
	 * int xxx_reset (conv_t conv, unsigned char *r, size_t n)
	 * stores a shift sequences returning to the initial state beginning at r.
	 * Up to n bytes may be written at r. n is >= 0.
	 * Result is number of bytes written, or -2 if n too small.
	 */
};

/*
*	结构体wctomb_funcs成员的错误信息
*/
/* Return code if invalid. (xxx_wctomb) */
#define RET_ILUNI      -1
/* Return code if output buffer is too small. (xxx_wctomb, xxx_reset) */
#define RET_TOOSMALL   -2
/* Return code if output buffer not enough  */
#define RET_NOENOUGH   -3

/*
 * Contents of a conversion descriptor.
 */
struct conv_struct {
	struct loop_funcs lfuncs;						//	struct loop_funcs --- which include LoopConvert and LoopReset function
	state_t lfstate;								//	the state of struct loop_funcs: 1 --- waitting、0 --- no error occur、-1 --- error
	/* Input (conversion multibyte -> unicode) */
	struct mbtowc_funcs ifuncs;						//	struct mbtowc_func --- which include utf8_mbtowc function
	state_t istate;									//	the state of struct mbtowc_func: 1 --- waitting、0 --- no error occur、-1 --- error
	/* Output (conversion unicode -> multibyte) */
	struct wctomb_funcs ofuncs;						//	struct wctomb_funcs --- which include gbk_wctomb function								
	state_t ostate;									//	the state of struct mbtowc_func: 1 --- waitting、0 --- no error occur、-1 --- error
	/* error buffer */
	state_t error_info;								//	store error return of function
	unsigned char* outbuf;
	size_t outbytesleft;
};


#ifdef __cplusplus
extern "C" {
#endif
	/*
	*	清理输出缓冲区任务——当前是静态缓冲区，所以不会执行清理工作
	* 
	*		返回值：
	*			true		function is called no error
	*			false		function is called error
	*/
	bool LoopReset(conv_t cd, char** outbuf, size_t* outbytesleft);

	/*
	*	转码任务——
	* 
	*		返回值：
	*			true			no error
	*			flase			error
	*/
	bool LoopConvert(conv_t cd, const char** inbuf, size_t* inbytesleft, char** outbuf, size_t* outbytesleft);

#ifdef __cplusplus
}
#endif // !cplusplus

/*
*	Include all the converters
*/
/* CJK character sets [CCS = coded character set] [CJKV.INF chapter 3] */
typedef struct {
	unsigned short indx; /* index into big table */
	unsigned short used; /* bitmask of used entries */
} Summary16;



/*  Include all converter  */
#include "ascii.h"
#include "ces_gbk.h"
#include "cp936ext.h"
#include "gb2312.h"
#include "gbk.h"
#include "gbkext_inv.h"
#include "utf8.h"

#endif // !CONVERTERS_H
