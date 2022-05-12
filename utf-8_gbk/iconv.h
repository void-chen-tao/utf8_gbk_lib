#ifndef __ICONV_H__
#define __ICONV_H__

typedef void* iconv_t;
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif
	/*
	*	根据tocode和fromcode参数来配置转码结构体
	* 
	*	参数：
	*		tocode			目标编码方式
	*		fromcode		待转换的编码格式
	* 
	*	返回值：
	*		转换器结构体配置成功	有效指针
	*		失败					NULL指针
	*/
	iconv_t CreateConverter(const char* tocode, const char* fromcode);


	/*
	*	将fromcode编码输入的字符串str转换为tocode编码的字符串
	* 
	*	参数：
	*		icd				转换器结构体
	*		str				fromcode编码的字符串
	* 
	*	返回值：
	*		true			run function success
	*		false			run function failure
	*/
	bool RunConverter(iconv_t icd, const unsigned char* str);

	/*
	*	该函数用于获取RunConverter函数执行后的结果
	* 
	*	参数：
	*		icd			转换器
	*		
	*	返回值：
	*		转换器输出缓冲区指针，转换成功字符串有效，否则为空字符串。
	* 
	*/
	const unsigned char* ReadOutPutINFO(iconv_t icd);

	/*
	*	当前函数用于管理iconv_t的动态内存(heap)
	*/
	bool DestroyConverter(iconv_t icd);


#ifdef __cplusplus
}
#endif

#endif // !__ICONV_H__

