//#include <ctype.h>
//#include <malloc.h>
//#include <string.h>
//#include <stdbool.h>
//#include "converters.h"
//#include "iconv.h"
//
//#define gbk_mbtowc			(void*)0
//#define utf8_wctomb			(void*)0
//#define CODESCHEMELIMIT		20
//
//#ifdef DYNAMIC_ALLOCATION
//struct conv_struct cd;
//#endif
//
//
//
///*
//*	֧�ֵ�ת�뷽ʽ�ַ���
//*/
//static const char* TableOfCodeScheme[] = {
//	"UTF-8",
//	"GBK",
//};
//
///*
//*	֧�ֵĶ��ֽڱ���ת��Ϊ���ֽڵĺ���ָ��
//*/
//static const void* TableOfMBToWC[] = {
//	utf8_mbtowc,
//	gbk_mbtowc,
//};
//
///*
//*	֧�ֵĿ��ֽ�ת���ֽڵĺ���ָ��
//*/
//static const void* TableOfWCToMB[] = {
//	utf8_wctomb,
//	ces_gbk_wctomb,
//};
//
////	��̬����ʵ��
//iconv_t CreateConverter0(const char* tocode, const char* fromcode)
//{
//	return NULL;
//}
//
////	��̬�ڴ�ʵ��
//iconv_t CreateConverter1(const char* tocode, const char* fromcode)
//{
//	return NULL;
//}
//
//iconv_t CreateConverter(const char* tocode, const char* fromcode)
//{
//#ifdef DYNAMIC_ALLOCATION
//	//	��̬����ʵ��
//	return CreateConverter1(tocode, fromcode);
//#else
//	//	��̬����ʵ��
//	return CreateConverter0(tocode, fromcode);
//#endif
//}

