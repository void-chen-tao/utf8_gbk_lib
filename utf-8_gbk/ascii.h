#ifndef __ASCII_H__
#define __ASCII_H__

#include "converters.h"


#ifdef __cplusplus
extern "C" {
#endif
	/*
	*   �ж��Ƿ�Ϊ��unicode���뷽ʽ��ascii�롣
	*
	*   ������
	*       conv        ת�����ṹ��
	*       r           gbk������
	*       wc          һ��unicode�ַ�
	*       n           gbk��������С
	*
	*   ����ֵ��
	*       �����unicode�����ascii�룬����1�����򷵻�-1��
	*
	*/
	int ascii_wctomb(conv_t conv, unsigned char* r, ucs4_t wc, size_t n);

#ifdef __cplusplus
}
#endif	/*  __cplusplus  */

#endif	/*  __ASCII_H__  */