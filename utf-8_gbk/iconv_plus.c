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
//*	支持的转码方式字符串
//*/
//static const char* TableOfCodeScheme[] = {
//	"UTF-8",
//	"GBK",
//};
//
///*
//*	支持的多字节编码转换为宽字节的函数指针
//*/
//static const void* TableOfMBToWC[] = {
//	utf8_mbtowc,
//	gbk_mbtowc,
//};
//
///*
//*	支持的宽字节转多字节的函数指针
//*/
//static const void* TableOfWCToMB[] = {
//	utf8_wctomb,
//	ces_gbk_wctomb,
//};
//
////	静态分配实现
//iconv_t CreateConverter0(const char* tocode, const char* fromcode)
//{
//	return NULL;
//}
//
////	动态内存实现
//iconv_t CreateConverter1(const char* tocode, const char* fromcode)
//{
//	return NULL;
//}
//
//iconv_t CreateConverter(const char* tocode, const char* fromcode)
//{
//#ifdef DYNAMIC_ALLOCATION
//	//	动态分配实现
//	return CreateConverter1(tocode, fromcode);
//#else
//	//	静态分配实现
//	return CreateConverter0(tocode, fromcode);
//#endif
//}

