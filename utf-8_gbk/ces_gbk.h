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
	*   ������
	*       conv        ת�����ṹ��
	*       r           gbk������
	*       wc          һ��unicode�ַ�
	*       n           gbk��������С
	*
	*   ����ֵ��
	*       ת���ɹ�������1����2��ascii��ʾ1��gbk��ʾ2
	*       ת��ʧ�ܣ�����-1����-2��-1��ʾ����ת��ʧ�ܡ�-2��ʾʣ��ռ䲻��
	*
	*/
	int ces_gbk_wctomb(conv_t conv, unsigned char* r, ucs4_t wc, size_t n);

#ifdef __cplusplus
}
#endif	/*  __cplusplus  */

#endif	/*  __CES_GBK_H__  */

