#ifndef __UTF8_H__
#define __UTF8_H__

#include "converters.h"


#ifdef __cplusplus
extern "C" {
#endif
	/*
	*   utf8���ֽ�ת��Ϊunicode���ֽ�
	*
	*   ������
	*       conv        ת�����ṹ��
	*       pwc         ���ֽ�(unsigned int)�ĵ�ַ
	*       s           ��Ҫת�����ַ���
	*       n           ��Ҫת�����ַ����ĳ��ȡ���strlen(s)
	*
	*   ����ֵ��
	*       ������������Ч��utf-8�ַ��������ظ�utf-8�ַ����ֽڳ���
	*       ��Ч��utf-8���룬����-1�����Ȳ�ƥ�䣬����-2��
	*/
	int utf8_mbtowc(conv_t conv, ucs4_t* pwc, const unsigned char* s, size_t n);

#ifdef __cplusplus
}
#endif	/*  __cplusplus  */

#endif	/*  __UTF8_H__  */

