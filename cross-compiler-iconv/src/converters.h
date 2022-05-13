/*
* ���ļ������˾���ʵ�ֵĽӿ��Լ�����������
*/
#ifndef CONVERTERS_H
#define CONVERTERS_H

/*====================================================
*				converters.h�ļ���������
*====================================================*/
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>



/*====================================================
*				����ת����������������
*====================================================*/
/*  �Զ�����ַ�����  */
typedef unsigned int ucs4_t;
/*  ת����״̬����  */
typedef int state_t;
/*  ת�����ṹ��  */
typedef struct conv_struct* conv_t;

/*
 * Data type for conversion multibyte -> unicode
 * �ӿڶ���
 */
struct mbtowc_funcs {
	/*  
	*	���еĶ��ֽڱ���ת��Ϊ���ֽڱ��뺯���Ľӿ�ģ��
	*	�����Ҫ��ָ��utf8_mbtowc����
	* 
	*	������
	*		conv_t					ת�����ṹ��ָ��
	*		ucs4_t*					ת���ɹ�����ֽڵĴ洢��ַ
	*		unisgned char const*	��ת���Ķ��ֽڱ����ַ���
	*		size_t					strlen(s)
	* 
	*	����ֵ��
	*		����ɹ�ת��Ϊһ�����ֽڣ����ض��ٸ����ֽڱ����ַ��������˸ÿ��ַ�
	*		�����磺"��"��utf8�����ǣ�E4 B8 ADת���ɹ��󽫷���3����Ϊ���ַ���3
	*		��char���ɡ�
	*		�����Ҫת���ı�������Ч�ı��뷽ʽ��������-1��
	*		����ַ�������strlen(s)��ƥ�䣬������-2��
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
*	�ṹ��mbtowc_funcs��Ա�Ĵ�����Ϣ��
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
 *	ת������������
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
	*	�������ֽڱ���ת��Ϊ���ֽڱ��뺯���Ľӿ�ģ��
	*	�����Ҫ��gbk_wctomb����
	*
	*		������
	*			conv		ת�����ṹ��
	*			r			gbk�洢�ռ�
	*			wc			��Ҫת���ĵ���unicode�����ַ�
	*			n			2������Ϊgbk������ʹ��2��char���洢�ı��뷽ʽ
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
*	�ṹ��wctomb_funcs��Ա�Ĵ�����Ϣ
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
	state_t lfstate;								//	the state of struct loop_funcs: 1 --- waitting��0 --- no error occur��-1 --- error
	/* Input (conversion multibyte -> unicode) */
	struct mbtowc_funcs ifuncs;						//	struct mbtowc_func --- which include utf8_mbtowc function
	state_t istate;									//	the state of struct mbtowc_func: 1 --- waitting��0 --- no error occur��-1 --- error
	/* Output (conversion unicode -> multibyte) */
	struct wctomb_funcs ofuncs;						//	struct wctomb_funcs --- which include gbk_wctomb function								
	state_t ostate;									//	the state of struct mbtowc_func: 1 --- waitting��0 --- no error occur��-1 --- error
	/* error buffer */
	state_t error_info;								//	store error return of function
	unsigned char* outbuf;
	size_t outbytesleft;
};


#ifdef __cplusplus
extern "C" {
#endif
	/*
	*	����������������񡪡���ǰ�Ǿ�̬�����������Բ���ִ��������
	* 
	*		����ֵ��
	*			true		function is called no error
	*			false		function is called error
	*/
	bool LoopReset(conv_t cd, char** outbuf, size_t* outbytesleft);

	/*
	*	ת�����񡪡�
	* 
	*		����ֵ��
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
