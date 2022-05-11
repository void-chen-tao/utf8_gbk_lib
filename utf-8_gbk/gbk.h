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
	*   ��unicodeת��Ϊgbk����
	*
	*   ������
	*       conv        ת�����ṹ��
	*       r           һ��gbk�ַ��Ĵ洢�ռ�(2��char)
	*       wc          һ��unicode������ַ�
	*       n           2
	*
	*	����ֵ��
	*		��uniocdeת��Ϊgbk�����ת���ɹ���������2���ֽڡ�����gbk��С
	*		����ñ��벻���ڣ�����-1��������������̫С������-2��
	*/
	int gbk_wctomb(conv_t conv, unsigned char* r, ucs4_t wc, size_t n);

#ifdef __cplusplus
}
#endif	/*  __cplusplus  */

#endif	/*  __GBK_H__  */

